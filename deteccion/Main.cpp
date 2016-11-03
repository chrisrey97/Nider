#include "ClienteSocket.hpp"
#include "Sistema.hpp"

/*
Faltan (Aparte)
Opcion para limitar los fps sin recompilar
Opcion para cambiar el archivo
En video_test 3.25m (suposicion) * 7 (autos) =  22.75m  distancia entre la camara y la union de las barreras de contencion
*/

int main(int argc, char const *argv[])
{
    std::string camara_alta = "../deteccion/data/corregido_alta.mp4";
    std::string camara_baja = "../deteccion/data/corregido_baja2.mp4";
    bool debug = false;
    nider::sistema sistema;
    nider::clientesocket socket;
    sistema.ImprimirInfoNider();
    nider::calibracion calibrador(camara_alta);
    calibrador.Calibrar();
    nider::detector detector(debug,camara_alta,camara_baja,sistema,calibrador,socket);
    detector.Iniciar();
    detector.LoopPrincipalDeteccion();
    return EXIT_SUCCESS;
}
