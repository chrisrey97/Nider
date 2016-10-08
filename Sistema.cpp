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
