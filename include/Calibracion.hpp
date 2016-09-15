#ifndef CALIBRACION_HEADER
#define CALIBRACION_HEADER

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

struct CalibracionStruct
{
    bool calibrando;
    cv::Mat frame;
    cv::Mat transformation_matrix;
    std::vector<cv::Point> points_selection;
};
typedef CalibracionStruct CalibracionInfo;

CalibracionInfo Calibrar(cv::Mat frame);
void MouseCallbackFunc(int type, int x, int y, int flags, void* data);

#endif