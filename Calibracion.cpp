#include "Calibracion.hpp"

CalibracionInfo Calibrar(cv::Mat frame)
{
    CalibracionInfo data;
    data.calibrando = true;
    data.frame = frame;
    data.points_selection = std::vector<cv::Point>();
    cv::namedWindow("Calibrar",cv::WINDOW_NORMAL);
    cv::imshow("Calibrar",data.frame);
    cv::setMouseCallback("Calibrar", MouseCallbackFunc,static_cast<void*>(&data));
    while(data.calibrando)
    {
        cv::waitKey(30);
        cv::imshow("Calibrar",data.frame);
    }
    cv::destroyWindow("Calibrar");
    return data;
}

void MouseCallbackFunc(int type, int x, int y, int flags, void* data)
{
    CalibracionInfo* calib_info = static_cast<CalibracionInfo*>(data);
    if(type == cv::EVENT_LBUTTONDOWN)
    {
        calib_info->points_selection.push_back(cv::Point(x,y));
        cv::line(calib_info->frame,cv::Point(0,y),cv::Point(x,y),cv::Scalar(0,255,0));
        cv::line(calib_info->frame,cv::Point(x,y),cv::Point(calib_info->frame.size().width,y),cv::Scalar(0,255,0));
    }
    else if(type == cv::EVENT_RBUTTONDOWN)
    {
        calib_info->points_selection.pop_back();
    }
    if(calib_info->points_selection.size() == 4)
    {
        calib_info->calibrando = false;
    }
}