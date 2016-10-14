#include "Seguimiento.hpp"

namespace nider
{
    namespace seguimiento
    {
        Auto::Auto(std::vector<cv::Point> contornos_ref) : contorno{std::move(contornos_ref)}
        {
            boundingRect = cv::boundingRect(contorno);
            centro = cv::Point(boundingRect.x + (boundingRect.width)/2, boundingRect.y + (boundingRect.height)/2);
        }

        bool Auto::operator==(const Auto &a1)
        {
            return this->centro == a1.centro;
        }
    }
}
