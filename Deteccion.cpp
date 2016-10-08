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
        if(modo_debug)
        {
            cv::namedWindow("Contornos 1 (FIND_CONTOURS)",cv::WINDOW_NORMAL);
            cv::namedWindow("Contornos 2 (CONVEX_HULLS)",cv::WINDOW_NORMAL);
            cv::namedWindow("Procesar Contornos",cv::WINDOW_NORMAL);
        }
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
            originalCurrentFrame = currentFrame;
            cv::imshow("Input",currentFrame);
            TransformacionesMorfologicasFrame(currentFrame);
            TransformacionesMorfologicasFrame(nextFrame);
            TransformacionesMorfologicasDiferenciaFrames(currentFrame,nextFrame);
            DetectarContornosDiferenciasFrames(deteccionesFrames.clone());
            ProcesarContornosDetectados();
            GenerarOutputFrame();
            cv::imshow("Output",outputFrame);
            cv::waitKey(1);
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
        autos_detectados.clear();
        autos_detectados.resize(convex_hulls.size());
        for(auto &contorno : convex_hulls)
        {
            nider::seguimiento::Auto candidato = nider::seguimiento::Auto(contorno);
            if(candidato.boundingRect.area() > AREA_MINIMA_RECTANGULOS_DETECCION)
            {
                autos_detectados.push_back(candidato);
            }
        }
        if(modo_debug)
        {
            cv::Mat autosFrame(deteccionesFrames.size(),CV_8UC3,nider::utilidades::COLOR_NEGRO);
            for(auto autod : autos_detectados)
            {
                cv::rectangle(autosFrame,autod.boundingRect.tl(),autod.boundingRect.br(),nider::utilidades::COLOR_ROJO);
            }
            cv::imshow("Procesar Contornos",autosFrame);
        }
    }

    void detector::GenerarOutputFrame()
    {
        cv::warpPerspective(currentFrame,currentFrame,calibrador_ref.getCalibracionData().transformation_matrix.inv(),currentFrame.size(),CV_WARP_INVERSE_MAP);
        outputFrame = currentFrame;
        /*for(auto autod : autos_detectados)
        {
            std::vector<cv::Point2f> rectPoints;
            std::vector<cv::Point2f> rectPointsNormal;
            cv::Point2f p_tl = autod.boundingRect.tl();
            cv::Point2f p_br = autod.boundingRect.br();
            rectPoints.push_back(p_tl);
            rectPoints.push_back(p_br);
            cv::transform(rectPoints,rectPointsNormal,calibrador_ref.getCalibracionData().transformation_matrix.inv());
            cv::rectangle(outputFrame,rectPoints.at(0),rectPoints.at(1),nider::utilidades::COLOR_ROJO);
        }*/
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