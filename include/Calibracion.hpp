#ifndef CALIBRACION_HEADER
#define CALIBRACION_HEADER

#include <iostream>
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Utilidades.hpp"

namespace nider
{
    class calibracion
    {
    public:
        struct CalibracionStruct
        {
            bool calibrando;
            cv::Mat frame;
            cv::Mat transformation_matrix;
            std::vector<cv::Point2f> selected_points;
            cv::Size output_size;
        };
        void Calibrar();
        CalibracionStruct getCalibracionData();
        calibracion(std::string video_path);
    private:
        CalibracionStruct CalibracionData;
        std::string video_path;
        void Calcular_Transformation_Matrix(CalibracionStruct &data);
        void Ordenar_Puntos_SentidoReloj(std::vector<cv::Point2f> &points);
        static void MouseCallbackFunc(int type, int x, int y, int flags, void* data);
    };
}

#endif