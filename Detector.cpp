#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include "Calibracion.hpp"

int main(int argc, char const *argv[])
{
    cv::Mat frame;
    cv::VideoCapture video_input;
    video_input.open("../data/video_test.mp4");
    video_input >> frame;
    CalibracionInfo DatosCalibracion = Calibrar(frame);
    cv::namedWindow("Input",cv::WINDOW_NORMAL);
    cv::namedWindow("Output",cv::WINDOW_NORMAL);
    while(true)
    {
        int64_t start_time = cv::getTickCount();
        video_input >> frame;
        if(frame.empty())
        {
            std::cout << "\nFin del Video" << std::endl;
            break;
        }
        cv::imshow("Input",frame);
        cv::warpPerspective(frame,frame,DatosCalibracion.transformation_matrix,DatosCalibracion.output_size);
        cv::imshow("Output",frame);
        cv::waitKey(1);//buscar un cv::updateWindows() sin wait o sleep
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start_time);
		std::cout << "\rFPS: " << fps << std::flush;
    }
    return EXIT_SUCCESS;
}
