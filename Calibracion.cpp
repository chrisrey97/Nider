#include "Calibracion.hpp"

CalibracionInfo Calibrar(cv::Mat frame)
{
    CalibracionInfo data;
    data.calibrando = true;
    data.frame = frame;
    data.selected_points = std::vector<cv::Point2f>();
    cv::namedWindow("Calibrar",cv::WINDOW_NORMAL);
    cv::imshow("Calibrar",data.frame);
    cv::setMouseCallback("Calibrar", MouseCallbackFunc,static_cast<void*>(&data));
    while(data.calibrando)
    {
        cv::waitKey(30);
        cv::imshow("Calibrar",data.frame);
    }
    cv::destroyWindow("Calibrar");
    Ordenar_Puntos_SentidoReloj(data.selected_points);
    Calcular_Transformation_Matrix(data);
    return data;
}

void Calcular_Transformation_Matrix(CalibracionInfo &data)
{
    std::vector<double> altos;
    std::vector<double> anchos;
    altos.push_back(Calcular_Distancia_Puntos2d(data.selected_points[0],data.selected_points[3]));
    altos.push_back(Calcular_Distancia_Puntos2d(data.selected_points[1],data.selected_points[2]));
    anchos.push_back(Calcular_Distancia_Puntos2d(data.selected_points[0],data.selected_points[1]));
    anchos.push_back(Calcular_Distancia_Puntos2d(data.selected_points[3],data.selected_points[2]));
    std::vector<double>::iterator alto = std::max_element(altos.begin(),altos.end());
    std::vector<double>::iterator ancho = std::max_element(anchos.begin(),anchos.end());
    std::vector<cv::Point2f> output_coord;
    output_coord.push_back(cv::Point(0,0));
    output_coord.push_back(cv::Point(*ancho,0));
    output_coord.push_back(cv::Point(*ancho,*alto));
    output_coord.push_back(cv::Point(0,*alto));
    data.transformation_matrix = cv::getPerspectiveTransform(data.selected_points.data(),output_coord.data());
    data.output_size = cv::Size(*ancho,*alto);
}

void Ordenar_Puntos_SentidoReloj(std::vector<cv::Point2f> &points)
{
    std::vector<std::pair<double,cv::Point2f>> distancias;
    distancias.push_back(std::make_pair(Calcular_Distancia_Punto_Origen(points[0]),points[0]));
    distancias.push_back(std::make_pair(Calcular_Distancia_Punto_Origen(points[1]),points[1]));
    distancias.push_back(std::make_pair(Calcular_Distancia_Punto_Origen(points[2]),points[2]));
    distancias.push_back(std::make_pair(Calcular_Distancia_Punto_Origen(points[3]),points[3]));
    std::sort(distancias.begin(),distancias.end(),
    [](const std::pair<double,cv::Point2f> &A, const std::pair<double,cv::Point2f> &B)
    {
        return A.first < B.first;
    });
    points[0] = distancias[0].second;
    points[2] = distancias[3].second;
    distancias.erase(std::remove(distancias.begin(),distancias.end(),distancias[0]),distancias.end());
    distancias.erase(std::remove(distancias.begin(),distancias.end(),distancias[3]),distancias.end());
    std::sort(distancias.begin(),distancias.end(),
    [](const std::pair<double,cv::Point2f> &A, const std::pair<double,cv::Point2f> &B)
    {
        return A.second.x > B.second.x;
    });
    points[1] = distancias[0].second;
    points[3] = distancias[1].second;
}

void MouseCallbackFunc(int type, int x, int y, int flags, void* data)
{
    CalibracionInfo* calib_info = static_cast<CalibracionInfo*>(data);
    if(type == cv::EVENT_LBUTTONDOWN)
    {
        calib_info->selected_points.push_back(cv::Point(x,y));
        cv::line(calib_info->frame,cv::Point(0,y),cv::Point(x,y),cv::Scalar(0,255,0));
        cv::line(calib_info->frame,cv::Point(x,y),cv::Point(calib_info->frame.size().width,y),cv::Scalar(0,255,0));
        cv::line(calib_info->frame,cv::Point(x,y),cv::Point(x,calib_info->frame.size().height),cv::Scalar(0,255,0));
        cv::line(calib_info->frame,cv::Point(x,0),cv::Point(x,y),cv::Scalar(0,255,0));
    }
    if(calib_info->selected_points.size() == 4)
    {
        calib_info->calibrando = false;
    }
}