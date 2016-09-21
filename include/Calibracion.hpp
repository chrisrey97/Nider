#ifndef CALIBRACION_HEADER
#define CALIBRACION_HEADER

#include <iostream>
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Utilidades.hpp"

//public
struct CalibracionStruct
{
    bool calibrando;
    cv::Mat frame;
    cv::Mat transformation_matrix;
    std::vector<cv::Point2f> selected_points;
    cv::Size output_size;
};
typedef CalibracionStruct CalibracionInfo;
CalibracionInfo Calibrar(cv::Mat frame);

//private
void Calcular_Transformation_Matrix(CalibracionInfo &data);
void Ordenar_Puntos_SentidoReloj(std::vector<cv::Point2f> &points);
void MouseCallbackFunc(int type, int x, int y, int flags, void* data);

#endif