#include "Nider.hpp"

int main(int argc, char const *argv[])
{
    std::string video_path = "../data/video_test.mp4";
    nider::sistema sistema;
    nider::calibracion calibrador(video_path);
    auto DatosCalibracion = calibrador.Calibrar();
    nider::detector detector(video_path,sistema);
    detector.Iniciar(false);
    detector.LoopPrincipalDeteccion();
    return EXIT_SUCCESS;
}