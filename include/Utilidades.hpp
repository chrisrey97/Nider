#ifndef UTILIDADES_HEADER
#define UTILIDADES_HEADER

#include <cmath>
#include <opencv2/core.hpp>

namespace nider
{
    class utilidades
    {
    public:
        double Calcular_Distancia_Puntos2d(cv::Point2f Q, cv::Point2f P);
        double Calcular_Distancia_Punto_Origen(cv::Point2f Q);
        static const cv::Scalar COLOR_NEGRO;
        static const cv::Scalar COLOR_BLANCO;
        static const cv::Scalar COLOR_ROJO;
        static const cv::Scalar COLOR_VERDE;
    };
}

#endif
