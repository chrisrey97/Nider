#ifndef SEGUIMIENTO_HEADER
#define SEGUIMIENTO_HEADER

#include "Sistema.hpp"

namespace nider
{
    namespace seguimiento
    {
        class Auto
        {
        public:
            Auto(){}//Por alguna razon std::vector en C++11 requiere de un constructor "defualt"
            Auto(std::vector<cv::Point> contornos);
            int id;
            cv::Rect boundingRect;
            cv::Rect boundingRectNormal;
            cv::Rect boundingRectNormalMax;
            cv::Point centro;
            cv::Point prevCentro;
            cv::Mat ultimaImagen;
            std::vector<cv::Point2f> contornoNormal;
            double distancia_actual_al_origen;
            double distancia_previa_al_origen;
            double velocidad_frame;
            double velocidad_promedio;
            double velocidad_sum;
            int muestras;
            bool operator == (const Auto &a1); //Requerido para usar std::find()
        private:
            std::vector<cv::Point> contorno;
        };
    }
}

#endif
