#include "Deteccion.hpp"

namespace nider
{
    detector::detector(std::string video_p, nider::sistema &sistema_r, nider::calibracion &calibrador_r) : sistema_ref(sistema_r), calibrador_ref(calibrador_r)
    {
        video_path = video_p;
    }

    void detector::Iniciar(bool debug)
    {
        video_input.open(video_path);
        if(!video_input.isOpened())
        {
            video_input.release();
            std::cout << "ERROR: No existe el archivo" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
        cv::namedWindow("Input",cv::WINDOW_NORMAL);
        cv::namedWindow("Output",cv::WINDOW_NORMAL);
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
            cv::imshow("Input",currentFrame);
            cv::warpPerspective(currentFrame,currentFrame,calibrador_ref.getCalibracionData().transformation_matrix,calibrador_ref.getCalibracionData().output_size);
            cv::warpPerspective(nextFrame,nextFrame,calibrador_ref.getCalibracionData().transformation_matrix,calibrador_ref.getCalibracionData().output_size);
            cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY);
            cv::cvtColor(nextFrame, nextFrame, CV_BGR2GRAY);
            cv::GaussianBlur(currentFrame,currentFrame,cv::Size(5,5),0);
            cv::GaussianBlur(nextFrame,nextFrame,cv::Size(5,5),0);
            cv::absdiff(currentFrame,nextFrame,frameDifference);
            cv::threshold(frameDifference,frameDifferenceThreshold,30,255,CV_THRESH_BINARY);
            cv::dilate(frameDifferenceThreshold,frameDifferenceThreshold,structuringElement5x5);
            cv::dilate(frameDifferenceThreshold,frameDifferenceThreshold,structuringElement5x5);
            cv::erode(frameDifferenceThreshold,frameDifferenceThreshold,structuringElement5x5);
            cv::imshow("Output",frameDifferenceThreshold);
            cv::waitKey(1);
            sistema_ref.ImprimirFPS();
        }
    }
}