#include "Deteccion.hpp"

namespace nider
{
    detector::detector(bool debug, std::string video_p, nider::sistema &sistema_r, nider::calibracion &calibrador_r) : sistema_ref(sistema_r), calibrador_ref(calibrador_r)
    {
        video_path = video_p;
        modo_debug = debug;
    }

    void detector::Iniciar()
    {
        video_input.open(video_path);
        if(!video_input.isOpened())
        {
            video_input.release();
            std::cout << "ERROR: No existe el archivo" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
        structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
        structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));
        cv::namedWindow("Input",cv::WINDOW_NORMAL);
        cv::namedWindow("Output",cv::WINDOW_NORMAL);
        cv::namedWindow("Ultimo",cv::WINDOW_NORMAL);
        fps_video_target = 1/video_input.get(CV_CAP_PROP_FPS);
        fps_sleep_target = fps_video_target;
        if(modo_debug)
        {
            std::cout << "FPS del Video: " << video_input.get(CV_CAP_PROP_FPS) << std::endl;
            cv::namedWindow("Contornos 1 (FIND_CONTOURS)",cv::WINDOW_NORMAL);
            cv::namedWindow("Contornos 2 (CONVEX_HULLS)",cv::WINDOW_NORMAL);
            cv::namedWindow("Procesar Contornos",cv::WINDOW_NORMAL);
        }
        /*if(opcion_declarada --sleep_time (???))
        {
            fps_sleep_target = 1/opcion;
        }*/
    }

    void detector::LoopPrincipalDeteccion()
    {
        while(true)
        {
            sistema_ref.ActualizarTiempoInicio(cv::getTickCount());
            video_input >> currentFrame;
            video_input >> nextFrame;
            if(nextFrame.empty())
            {
                video_input.release();
                std::cout << "\nFin del Video" << std::endl;
                break;
            }
            currentFrame.copyTo(originalCurrentFrame);
            originalCurrentFrame.copyTo(outputFrame);
            cv::imshow("Input",currentFrame);
            TransformacionesMorfologicasFrame(currentFrame);
            TransformacionesMorfologicasFrame(nextFrame);
            TransformacionesMorfologicasDiferenciaFrames(currentFrame,nextFrame);
            DetectarContornosDiferenciasFrames(deteccionesFrames.clone());
            ProcesarContornosDetectados();
            ProcesarAutosDetectados();
            CalcularVelocidadAutosDetectados();
            GenerarOutputFrame();
            cv::imshow("Output",outputFrame);
            cv::waitKey(sistema_ref.GetDetectorLoopSleepTime(fps_sleep_target));
            sistema_ref.ImprimirFPS();
        }
    }

    void detector::TransformacionesMorfologicasFrame(cv::Mat &frame)
    {
        cv::warpPerspective(frame,frame,calibrador_ref.getCalibracionData().transformation_matrix,calibrador_ref.getCalibracionData().output_size);
        cv::cvtColor(frame, frame, CV_BGR2GRAY);
        cv::GaussianBlur(frame,frame,cv::Size(5,5),0);
    }

    void detector::ProcesarContornosDetectados()
    {
        autos_detectados_frame.clear();
        autos_detectados_frame.resize(convex_hulls.size());
        for(auto &contorno : convex_hulls)
        {
            nider::seguimiento::Auto candidato = nider::seguimiento::Auto(contorno);
            if(candidato.boundingRect.area() > AREA_MINIMA_RECTANGULOS_DETECCION)
            {
                autos_detectados_frame.push_back(candidato);
            }
        }
        if(modo_debug)
        {
            cv::Mat autosFrame(deteccionesFrames.size(),CV_8UC3,nider::utilidades::COLOR_NEGRO);
            for(auto autod : autos_detectados_frame)
            {
                cv::rectangle(autosFrame,autod.boundingRect.tl(),autod.boundingRect.br(),nider::utilidades::COLOR_ROJO);
                cv::circle(autosFrame,autod.centro,5,nider::utilidades::COLOR_VERDE);
            }
            cv::imshow("Procesar Contornos",autosFrame);
        }
    }

    void detector::ProcesarAutosDetectados()
    {
        if(autos_detectados_movimiento.empty())
        {
            for(auto autod : autos_detectados_frame)
            {
                if(autod.centro != cv::Point(0,0))
                {
                    autod.id = GenerarRandomAutoID();
                    autos_detectados_movimiento.push_back(autod);
                    autos_detectados_frame.erase(std::remove(autos_detectados_frame.begin(),autos_detectados_frame.end(),autod),autos_detectados_frame.end());
                }
            }
        }
        else
        {
            //Con ::iterator tenemos un puntero a los valores del vector, por lo que podemos cambiar los valores
            for(std::vector<nider::seguimiento::Auto>::iterator iter = autos_detectados_movimiento.begin(); iter != autos_detectados_movimiento.end(); ++iter)
            {
                auto& autom = *iter;
                autom.distancia_previa_al_origen = nider::utilidades::Calcular_Distancia_Punto_Origen(autom.centro);
            }
            for(std::vector<nider::seguimiento::Auto>::iterator iter = autos_detectados_movimiento.begin(); iter != autos_detectados_movimiento.end(); ++iter)
            {
                auto& autom = *iter;
                for(auto autod : autos_detectados_frame)
                {
                    if(nider::utilidades::Calcular_Distancia_Puntos2d(autom.centro,autod.centro) < nider::utilidades::Calcular_Distancia_Puntos2d(autom.centro,autom.boundingRect.br()))
                    {
                        autom.prevCentro = autom.centro;
                        autom.centro = autod.centro;
                        autom.boundingRect = autod.boundingRect;
                        autos_detectados_frame.erase(std::remove(autos_detectados_frame.begin(),autos_detectados_frame.end(),autod),autos_detectados_frame.end());
                    }
                }
                autom.distancia_actual_al_origen = nider::utilidades::Calcular_Distancia_Punto_Origen(autom.centro);
            }
            for(auto autod : autos_detectados_frame)
            {
                if(autod.centro != cv::Point(0,0))
                {
                    autod.id = GenerarRandomAutoID();
                    autos_detectados_movimiento.push_back(autod);
                    autos_detectados_frame.erase(std::remove(autos_detectados_frame.begin(),autos_detectados_frame.end(),autod),autos_detectados_frame.end());
                }
            }
        }
        for(auto autom : autos_detectados_movimiento)
        {
            if(autom.distancia_previa_al_origen == autom.distancia_actual_al_origen)
            {
                ProcesarAlSalir(autom);
                autos_detectados_movimiento.erase(std::remove(autos_detectados_movimiento.begin(),autos_detectados_movimiento.end(),autom),autos_detectados_movimiento.end());
            }
        }
        autos_detectados_frame.clear();
    }

    void detector::CalcularVelocidadAutosDetectados()
    {
        for(std::vector<nider::seguimiento::Auto>::iterator iter = autos_detectados_movimiento.begin(); iter != autos_detectados_movimiento.end(); ++iter)
        {
            auto& autom = *iter;
            autom.velocidad_frame = nider::utilidades::Calcular_Distancia_Puntos2d(autom.centro,autom.prevCentro) / fps_video_target;
            if(autom.muestras != 0)
            {
                autom.velocidad_sum += autom.velocidad_frame;
            }
            autom.muestras += 1;
            autom.velocidad_promedio = (autom.velocidad_sum/autom.muestras);
            std::vector<cv::Point2f> inputs;
            inputs.push_back(autom.boundingRect.tl());
            inputs.push_back(cv::Point2f(autom.boundingRect.tl().x + autom.boundingRect.width,autom.boundingRect.tl().y));
            inputs.push_back(autom.boundingRect.br());
            inputs.push_back(cv::Point2f(autom.boundingRect.br().x - autom.boundingRect.width,autom.boundingRect.br().y));
            inputs.push_back(autom.centro);
            cv::perspectiveTransform(inputs,autom.contornoNormal,calibrador_ref.getCalibracionData().transformation_matrix_inversa);
            autom.boundingRectNormal = cv::minAreaRect(autom.contornoNormal).boundingRect();
            cv::Rect MatRect(0, 0, originalCurrentFrame.cols, originalCurrentFrame.rows);
            bool inside = (autom.boundingRectNormal & MatRect) == autom.boundingRectNormal;
            if((autom.boundingRectNormal.area() > autom.boundingRectNormalMax.area()) && inside)
            {
                autom.boundingRectNormalMax = autom.boundingRectNormal;
                autom.ultimaImagen = cv::Mat(originalCurrentFrame,autom.boundingRectNormalMax);
            }
        }
    }

    void detector::ProcesarAlSalir(nider::seguimiento::Auto autom)
    {
        if(!(autom.velocidad_promedio == 0 || autom.muestras <= 6))
        {
            //std::cout << "Se elimina [Id: " << autom.id << " Vpf: " << autom.velocidad_promedio << "px/s (N: " << autom.muestras << ")]" << std::endl;
            cv::imshow("Ultimo",autom.ultimaImagen);
            cv::imwrite("outputs/"+ObtenerFechaNombreImagen(autom.id),autom.ultimaImagen);
        }
    }

    void detector::GenerarOutputFrame()
    {
        for(auto autod : autos_detectados_movimiento)
        {
            cv::circle(outputFrame,autod.contornoNormal.at(4),6,nider::utilidades::COLOR_VERDE,CV_FILLED);
            cv::putText(outputFrame,"Id: "+std::to_string(autod.id),autod.contornoNormal.at(4) + cv::Point2f(0,25),CV_FONT_NORMAL,1.0,nider::utilidades::COLOR_ROJO,2.0);
            cv::putText(outputFrame,"Vf: "+std::to_string((int)autod.velocidad_frame)+" px/s",autod.contornoNormal.at(4),CV_FONT_NORMAL,1.0,nider::utilidades::COLOR_ROJO,2.0);
            cv::rectangle(outputFrame,autod.boundingRectNormal,nider::utilidades::COLOR_ROJO);
            //cv::putText(outputFrame,"Vp: "+std::to_string((int)autod.velocidad_promedio)+" px/s",outputs.at(4) + cv::Point2f(0,50),CV_FONT_NORMAL,1.0,nider::utilidades::COLOR_ROJO,2.0);
            //cv::putText(outputFrame,"Mu: "+std::to_string(autod.muestras),outputs.at(4) + cv::Point2f(0,75),CV_FONT_NORMAL,1.0,nider::utilidades::COLOR_ROJO,2.0);
        }
    }

    std::string detector::ObtenerFechaNombreImagen(int id)
    {
        std::stringstream stream;
        auto time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        const std::string format = "%Y%m%d_%H%M%S_"+std::to_string(id)+".png";
        stream << std::put_time(std::localtime(&time_t),format.c_str());
        return stream.str();
    }

    int detector::GenerarRandomAutoID()
    {
        std::uniform_int_distribution<int> rand_dis(1,1000);
        return rand_dis(random_generator);
    }

    void detector::TransformacionesMorfologicasDiferenciaFrames(cv::Mat &currentFrame, cv::Mat &nextFrame)
    {
        cv::absdiff(currentFrame,nextFrame,deteccionesFrames);
        cv::threshold(deteccionesFrames,deteccionesFrames,30,255,CV_THRESH_BINARY);
        cv::dilate(deteccionesFrames,deteccionesFrames,structuringElement7x7);
        cv::erode(deteccionesFrames,deteccionesFrames,structuringElement3x3);
        cv::dilate(deteccionesFrames,deteccionesFrames,structuringElement5x5);
        cv::dilate(deteccionesFrames,deteccionesFrames,structuringElement5x5);
        cv::erode(deteccionesFrames,deteccionesFrames,structuringElement5x5);
    }

    void detector::DetectarContornosDiferenciasFrames(cv::Mat frame)
    {
        cv::findContours(frame,contornos,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
        if(modo_debug)
        {
            cv::Mat contornosFrame(deteccionesFrames.size(),CV_8UC3,nider::utilidades::COLOR_NEGRO);
            cv::drawContours(contornosFrame,contornos,-1,nider::utilidades::COLOR_BLANCO,-1);
            cv::imshow("Contornos 1 (FIND_CONTOURS)",contornosFrame);
        }
        convex_hulls.clear();
        convex_hulls.resize(contornos.size());
        for(size_t i = 0; i < contornos.size(); i++)
        {
            cv::convexHull(contornos[i],convex_hulls[i]);
        }
        if(modo_debug)
        {
            cv::Mat contornosFrame(deteccionesFrames.size(),CV_8UC3,nider::utilidades::COLOR_NEGRO);
            cv::drawContours(contornosFrame,convex_hulls,-1,nider::utilidades::COLOR_BLANCO,-1);
            cv::imshow("Contornos 2 (CONVEX_HULLS)",contornosFrame);
        }
    }
}
