#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudalegacy.hpp>
#include "Calibracion.hpp"

int main(int argc, char const *argv[])
{
    cv::Mat frame;
    cv::VideoCapture video_input;
    video_input.open("../data/video_test.mp4");
    video_input >> frame;
    CalibracionInfo Calibracion_Data = Calibrar(frame);
    cv::namedWindow("Input",cv::WINDOW_NORMAL);
    while(true)
    {
        int64_t start_time = cv::getTickCount();
        video_input >> frame;
        if(frame.empty())
        {
            std::cout << "Fin del Video" << std::endl;
            break;
        }
        cv::line(frame,Calibracion_Data.points_selection[0],Calibracion_Data.points_selection[1],cv::Scalar(255,0,0));
        cv::line(frame,Calibracion_Data.points_selection[1],Calibracion_Data.points_selection[2],cv::Scalar(255,0,0));
        cv::line(frame,Calibracion_Data.points_selection[2],Calibracion_Data.points_selection[3],cv::Scalar(255,0,0));
        cv::line(frame,Calibracion_Data.points_selection[3],Calibracion_Data.points_selection[0],cv::Scalar(255,0,0));
        cv::imshow("Input",frame);
        cv::waitKey(1);
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start_time);
		std::cout << "\rFPS: " << fps << std::flush;
    }
    return EXIT_SUCCESS;
}