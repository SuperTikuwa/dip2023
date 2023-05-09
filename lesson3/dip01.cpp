// g++ dip01.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
// g++ dip01.cpp `pkg-config --cflags --libs opencv`
#include <iostream>           //入出力関連ヘッダ
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダ
#include "./Stats.h"

using namespace std;
using namespace cv;

int main(int argc, const char *argv[])
{
    // ①画像ファイルの読み込み
    cv::Mat sourceImage = cv::imread("color.jpg", cv::IMREAD_COLOR);

    if (sourceImage.data == 0)
    { // 画像ファイルが読み込めなかった場合
        printf("File not found\n");
        exit(0);
    }
    printf("Width=%d, Height=%d\n", sourceImage.cols, sourceImage.rows);

    // ②画像格納用インスタンスの生成
    Mat grayImage;
    vector<Mat> bgrImage(3);

    cv::cvtColor(sourceImage, grayImage, cv::COLOR_RGB2GRAY);

    // cv::imshow("gray", grayImage);
    // cv::waitKey();

    cv::Mat hist = histogram::make(grayImage, grayImage.size());
    histogram::show("histogram", hist);

    int min = Stats::min(grayImage);
    int max = Stats::max(grayImage);
    double avg = Stats::average(grayImage);
    int med = Stats::median(grayImage);
    std::cout << "min:" << min << endl;
    std::cout << "max:" << max << endl;
    std::cout << "avg:" << avg << endl;
    std::cout << "med:" << med << endl;
    cv::waitKey();
    return 0;
}
