#include "Nider.hpp"

int main(int argc, char const *argv[])
{
    std::string video_path = "../data/video_test.mp4";
    nider::sistema sistema;
    nider::calibracion calibrador(video_path);
    calibrador.Calibrar();
    nider::detector detector(video_path,sistema,calibrador);
    detector.Iniciar(false);
    detector.LoopPrincipalDeteccion();
    return EXIT_SUCCESS;
}