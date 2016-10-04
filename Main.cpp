#include "Nider.hpp"

int main(int argc, char const *argv[])
{
    std::string video_path = "../data/video_test.mp4";
    nider::sistema sistema;
    nider::calibracion calibrador(video_path);
    auto DatosCalibracion = calibrador.Calibrar();
    cv::VideoCapture video_input;
    video_input.open("../data/video_test.mp4");
    cv::Mat frame;
    sistema.Iniciar(false);
    while(true)
    {
        sistema.ActualizarTiempoInicio(cv::getTickCount());
        video_input >> frame;
        if(frame.empty())
        {
            std::cout << "\nFin del Video" << std::endl;
            break;
        }
        cv::imshow("Input",frame);
        cv::warpPerspective(frame,frame,DatosCalibracion.transformation_matrix,DatosCalibracion.output_size);
        cv::imshow("Output",frame);
        cv::waitKey(1);
        sistema.ImprimirFPS();
    }
    return EXIT_SUCCESS;
}
