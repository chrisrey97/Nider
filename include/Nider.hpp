#ifndef NIDER_HEADER
#define NIDER_HEADER

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include "Calibracion.hpp"
#include "Deteccion.hpp"

namespace nider
{
    class sistema
    {
    public:
        void Iniciar(bool debug);
        void ImprimirFPS();
        void ActualizarTiempoInicio(int64_t time);
    private:
        int64_t start_time;
    };
}

#endif
