#include "Utilidades.hpp"

double Calcular_Distancia_Puntos2d(cv::Point2f Q, cv::Point2f P)
{
    return std::sqrt(std::pow((P-Q).x,2) + std::pow((P-Q).y,2));
}

double Calcular_Distancia_Punto_Origen(cv::Point2f Q)
{
    return std::sqrt(std::pow(Q.x,2) + std::pow(Q.y,2));
}