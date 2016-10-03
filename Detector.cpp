#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/cudaarithm.hpp>
#include "Calibracion.hpp"

int main(int argc, char const *argv[])
{
    cv::Mat frame;
    cv::Mat frame_wrp;
    cv::Mat frame_fgm;
    cv::cuda::GpuMat gpu_frame;
    cv::cuda::GpuMat gpu_warp_output;
    cv::cuda::GpuMat gpu_frame_foreground_mask;
    cv::cuda::GpuMat gpu_frame_foreground_image;
    cv::VideoCapture video_input;
    video_input.open("../data/video_test.mp4");
    video_input >> frame;
    CalibracionInfo Calibracion_Data = Calibrar(frame);
    cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> mog_filter = cv::cuda::createBackgroundSubtractorMOG2();
    mog_filter->setDetectShadows(false);
    cv::namedWindow("Input",cv::WINDOW_NORMAL);
    cv::namedWindow("Output",cv::WINDOW_NORMAL);
    while(true)
    {
        if(frame.empty())
        {
            std::cout << "\nFin del Video" << std::endl;
            break;
        }
        int64_t start_time = cv::getTickCount();
        video_input >> frame;
        cv::imshow("Input",frame);
        gpu_frame.upload(frame);
        cv::cuda::warpPerspective(gpu_frame,gpu_warp_output,Calibracion_Data.transformation_matrix,Calibracion_Data.output_size);
        mog_filter->apply(gpu_warp_output,gpu_frame_foreground_mask);
        cv::imshow("Output",frame);
        cv::waitKey(1);//buscar un cv::updateWindows() sin wait o sleep
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start_time);
		std::cout << "\rFPS: " << fps << std::flush;
    }
    return EXIT_SUCCESS;
}