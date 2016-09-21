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
    cv::RNG random(12345);
    cv::Mat frame;
    cv::Mat framebg_delta;
    cv::Mat framebg_delta_treshold;
    cv::Mat output_frame;
    cv::Mat background_image;
    cv::Mat canny_detected_edges;
    cv::cuda::GpuMat gpu_frame;
    cv::cuda::GpuMat gpu_framebg_delta;
    cv::cuda::GpuMat gpu_framebg_delta_treshold;
    cv::cuda::GpuMat gpu_warp_output_frame;
    cv::cuda::GpuMat gpu_background_image;
    cv::cuda::GpuMat gpu_canny_detected_edges;
    cv::VideoCapture video_input;
    video_input.open("../data/video_test.mp4");
    video_input >> frame;
    CalibracionInfo Calibracion_Data = Calibrar(frame);
    cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> mog_filter = cv::cuda::createBackgroundSubtractorMOG2();
    cv::Ptr<cv::cuda::Filter> blur_filter = cv::cuda::createGaussianFilter(0,-1,cv::Size(7,7),7);
    cv::Ptr<cv::cuda::CannyEdgeDetector> canny_detector = cv::cuda::createCannyEdgeDetector(30,200);
    cv::Ptr<cv::cuda::Filter> dilate_filter = cv::cuda::createMorphologyFilter(cv::MORPH_DILATE,0,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
    cv::namedWindow("Input",cv::WINDOW_NORMAL);
    cv::namedWindow("Output",cv::WINDOW_NORMAL);
    cv::namedWindow("Frame-Background Delta",cv::WINDOW_NORMAL);
    //cv::namedWindow("Frame-Background Delta Treshold",cv::WINDOW_NORMAL);
    cv::namedWindow("Mean Background",cv::WINDOW_NORMAL);
    //cv::namedWindow("Draw Contours",cv::WINDOW_NORMAL);
    while(true)
    {
        if(frame.empty())
        {
            std::cout << "\nFin del Video" << std::endl;
            break;
        }
        int64_t start_time = cv::getTickCount();
        video_input >> frame;
        gpu_frame.upload(frame);
        cv::cuda::warpPerspective(gpu_frame,gpu_warp_output_frame,Calibracion_Data.transformation_matrix,Calibracion_Data.output_size);
        mog_filter->apply(gpu_warp_output_frame,gpu_framebg_delta,0.6);
        mog_filter->getBackgroundImage(gpu_background_image);
        cv::cuda::absdiff(gpu_background_image,gpu_warp_output_frame,gpu_framebg_delta);
        cv::cuda::cvtColor(gpu_framebg_delta,gpu_framebg_delta,CV_BGR2GRAY);
        cv::cuda::threshold(gpu_framebg_delta,gpu_framebg_delta,5,255,CV_THRESH_BINARY);
        dilate_filter->apply(gpu_framebg_delta,gpu_framebg_delta);
        blur_filter->apply(gpu_framebg_delta,gpu_framebg_delta);
        canny_detector->detect(gpu_framebg_delta,gpu_canny_detected_edges);
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        gpu_canny_detected_edges.download(canny_detected_edges);
        cv::findContours(canny_detected_edges,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE);
        std::vector<std::vector<cv::Point>> contours_aprox_ploy(contours.size());
        std::vector<cv::Rect> bounding_rects(contours.size());
        for(int i = 0; i < contours.size(); i++)
        {
            cv::approxPolyDP(contours[i],contours_aprox_ploy[i],3,true);
            bounding_rects[i] = cv::boundingRect(contours_aprox_ploy[i]);
        }
        gpu_warp_output_frame.download(output_frame);
        /*for(int i = 0; i < bounding_rects.size(); i++)
        {
            cv::Point2f rect_points[4];
            cv::Scalar color = cv::Scalar(random.uniform(0, 255), random.uniform(0,255), random.uniform(0,255));
            bounding_rects[i].points(rect_points);
            for(int j = 0; j < 4; j++)
            {
                cv::line(output_frame,rect_points[j],rect_points[(j+1)%4],color,1,8);
            }
        }*/
        gpu_framebg_delta_treshold.download(framebg_delta_treshold);
        gpu_framebg_delta.download(framebg_delta);
        gpu_background_image.download(background_image);
        cv::imshow("Input",frame);
        cv::imshow("Frame-Background Delta",framebg_delta);
        cv::imshow("Output",output_frame);
        cv::imshow("Mean Background",background_image);
        //cv::imshow("Frame-Background Delta Treshold",framebg_delta_treshold);
        //cv::imshow("Draw Contours",contours_draw);
        cv::waitKey(2);
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start_time);
		std::cout << "\rFPS: " << fps << std::flush;
    }
    return EXIT_SUCCESS;
}