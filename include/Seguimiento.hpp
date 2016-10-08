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
            cv::Rect boundingRect;
            cv::Point centro;
        private:
            std::vector<cv::Point> contorno;
        };
    }
}

#endif