#include "Utilidades.hpp"

namespace nider
{
    //OpenCV usa un formate BGR en vez del tipico RGB
    const cv::Scalar utilidades::COLOR_NEGRO = cv::Scalar(0, 0, 0);
    const cv::Scalar utilidades::COLOR_BLANCO = cv::Scalar(255, 255, 255);
    const cv::Scalar utilidades::COLOR_ROJO = cv::Scalar(0, 0, 255);
    const cv::Scalar utilidades::COLOR_VERDE = cv::Scalar(0, 255, 0);
    
    double utilidades::Calcular_Distancia_Puntos2d(cv::Point2f Q, cv::Point2f P)
    {
        return std::sqrt(std::pow((P-Q).x,2) + std::pow((P-Q).y,2));
    }

    double utilidades::Calcular_Distancia_Punto_Origen(cv::Point2f Q)
    {
        return std::sqrt(std::pow(Q.x,2) + std::pow(Q.y,2));
    }
}
