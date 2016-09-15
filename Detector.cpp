#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudalegacy.hpp>

int main(int argc, char const *argv[])
{
    cv::Mat frame;
    cv::VideoCapture video_input;
    video_input.open("../data/video_test.mp4");
    cv::cuda::GpuMat gpu_frame(frame);
    while(true)
    {
        int64_t start_time = cv::getTickCount();
        video_input >> frame;
        if(frame.empty())
        {
            std::cout << "Fin del Video" << std::endl;
            break;
        }
        gpu_frame.upload(frame);
        gpu_frame.download(frame);
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start_time);
        std::cout << "\rFPS: " << fps << std::flush;
    }
    return EXIT_SUCCESS;
}