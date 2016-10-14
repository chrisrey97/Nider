#ifndef SISTEMA_HEADER
#define SISTEMA_HEADER


#ifndef NIDER_VERSION_MAYOR
#define NIDER_VERSION_MAYOR -1
#endif

#ifndef NIDER_VERSION_MENOR
#define NIDER_VERSION_MENOR -1
#endif

#include <iostream>
#include <vector>
#include <random>
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
        void ImprimirInfoNider();
        void ImprimirFPS();
        void ActualizarTiempoInicio(int64_t time);
        double GetDetectorLoopSleepTime(double fps_target);
        double GetDeltaTimeNOW();
    private:
        int64_t start_time;
    };
}

#endif
