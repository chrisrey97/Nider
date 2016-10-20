#include "Sistema.hpp"

/*
Faltan (Aparte)
Opcion para limitar los fps sin recompilar
Opcion para cambiar el archivo
*/

int main(int argc, char const *argv[])
{
    std::string video_path = "../data/video_test.mp4";
    bool debug = false;
    nider::sistema sistema;
    sistema.ImprimirInfoNider();
    nider::calibracion calibrador(video_path);
    calibrador.Calibrar();
    nider::detector detector(debug,video_path,sistema,calibrador);
    detector.Iniciar();
    detector.LoopPrincipalDeteccion();
    return EXIT_SUCCESS;
}
