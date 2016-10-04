#include "Nider.hpp"

namespace nider
{
    void sistema::Iniciar(bool debug)
    {
        cv::namedWindow("Input",cv::WINDOW_NORMAL);
        cv::namedWindow("Output",cv::WINDOW_NORMAL);
    }
    void sistema::ImprimirFPS()
    {
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start_time);
		std::cout << "\rFPS: " << fps << std::flush;
    }
    void sistema::ActualizarTiempoInicio(int64_t time)
    {
        start_time = time;
    }
}
