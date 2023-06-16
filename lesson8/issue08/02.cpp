// g++ mc08_02.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>
#include "Stats.h"

int main()
{
    // 画像の読み込み
    cv::Mat img = cv::imread("input_image.png", cv::IMREAD_GRAYSCALE);
    if (img.empty())
    {
        std::cout << "画像ファイルを読み込めませんでした。" << std::endl;
        return -1;
    }

    // 大津の二値化
    cv::Mat thresh_img;
    cv::threshold(img, thresh_img, 10, 256, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("Thresholded Image", thresh_img);
    cv::waitKey();

    return 0;
}
