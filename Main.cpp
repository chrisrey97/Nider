#include "Sistema.hpp"

int main(int argc, char const *argv[])
{
    std::string video_path = "../data/video_test.mp4";
    nider::sistema sistema;
    sistema.ImprimirInfoNider();
    nider::calibracion calibrador(video_path);
    calibrador.Calibrar();
    nider::detector detector(false,video_path,sistema,calibrador);
    detector.Iniciar();
    detector.LoopPrincipalDeteccion();
    return EXIT_SUCCESS;
}