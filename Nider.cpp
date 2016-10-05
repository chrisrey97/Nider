#include "Nider.hpp"

namespace nider
{
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
