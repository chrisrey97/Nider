#include "Deteccion.hpp"

namespace nider
{
    detector::detector(std::string video_p, nider::sistema &sistema_r) : sistema_ref(sistema_r)
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
        //cv::namedWindow("Input",cv::WINDOW_NORMAL);
        //cv::namedWindow("Output",cv::WINDOW_NORMAL);
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
            sistema_ref.ImprimirFPS();
        }
    }
}