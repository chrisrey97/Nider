#include "Sistema.hpp"

namespace nider
{
    void sistema::ImprimirInfoNider()
    {
        std::cout << "Nider v" << NIDER_VERSION_MAYOR << "." << NIDER_VERSION_MENOR << std::endl;
        #ifdef COMPILAR_CON_CUDA
        std::cout << "Compilado Con CUDA" << std::endl;
        #endif
    }

    double sistema::GetDetectorLoopSleepTime(double fps_target)
    {
        double time_passed = (cv::getTickCount() - start_time)/cv::getTickFrequency();
        //std::cout << "TimePassed: " << time_passed << "s TARGET: " << fps_target << "s SLEEP: " << fps_target-time_passed << "s" << std::endl;
        double sleep = fps_target-time_passed;
        if(sleep <= 0 || (sleep < 1 && sleep >= 0))
        {
            return 1;
        }
        else
        {
            return (fps_target-time_passed)*1000;
        }
    }

    double sistema::GetDeltaTimeNOW()
    {
        return (cv::getTickCount() - start_time)/cv::getTickFrequency();
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
