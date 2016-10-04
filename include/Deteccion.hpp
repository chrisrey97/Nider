#ifndef DETECCION_HEADER
#define DETECCION_HEADER

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

namespace nider
{
    class detector
    {
    public:
        detector();
        void Iniciar();
        void LoopDeteccionPrincipal();
    private:
        std::string video_path;
        cv::VideoCapture video_input;
        cv::Mat currentFrame;
        cv::Mat nextFrame;
    };
}

#endif
