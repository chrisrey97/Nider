#ifndef DETECCION_HEADER
#define DETECCION_HEADER

#include "ClienteSocket.hpp"
#include "Sistema.hpp"
#include "Utilidades.hpp"
#include "Seguimiento.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

namespace nider
{
    //Forward Declarations...
    class sistema;
    namespace seguimiento
    {
        class Auto;
    }
    //Clase
    class detector
    {
    public:
        detector(bool debug, std::string camara_alta, std::string camara_baja, nider::sistema &sistema_ref, nider::calibracion &calibrador_ref, nider::clientesocket &cliente);
        void Iniciar();
        void LoopPrincipalDeteccion();
    private:
        bool modo_debug;
        const int AREA_MINIMA_RECTANGULOS_DETECCION = 10000;
        double fps_video_target;
        double fps_sleep_target;
        void TransformacionesMorfologicasFrame(cv::Mat &frame);
        void TransformacionesMorfologicasDiferenciaFrames(cv::Mat &currentFrame, cv::Mat &nextFrame);
        void DetectarContornosDiferenciasFrames(cv::Mat frame);
        void ProcesarContornosDetectados();
        void ProcesarAutosDetectados();
        void GenerarOutputFrame();
        void CalcularVelocidadAutosDetectados();
        void ProcesarAlSalir(nider::seguimiento::Auto);
        int GenerarRandomAutoID();
        std::string ObtenerFechaNombreImagen(int id);
        nider::sistema &sistema_ref;
        nider::calibracion &calibrador_ref;
        nider::clientesocket &cliente;
        std::string video_path_alta;
        std::string video_path_baja;
        std::vector<std::vector<cv::Point>> contornos;
        std::vector<std::vector<cv::Point>> convex_hulls;
        std::vector<nider::seguimiento::Auto> autos_detectados_frame;
        std::vector<nider::seguimiento::Auto> autos_detectados_movimiento;
        std::default_random_engine random_generator;
        cv::VideoCapture video_input;
        cv::VideoCapture video_input_baja;
        cv::Mat originalCurrentFrame;
        cv::Mat currentFrame;
        cv::Mat currentFrame_baja;
        cv::Mat nextFrame;
        cv::Mat deteccionesFrames;
        cv::Mat outputFrame;
        cv::Mat structuringElement3x3;
        cv::Mat structuringElement5x5;
        cv::Mat structuringElement7x7;
    };
}

#endif
