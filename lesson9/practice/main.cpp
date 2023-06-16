// g++ practice.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
// g++ practice.cpp `pkg-config --cflags --libs opencv`
#include <iostream>           //入出力関連ヘッダ
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダ

int main(int argc, const char *argv[])
{
    // 画像ファイルの読み込み
    cv::Mat sourceImage = cv::imread("input.jpg");
    cv::cvtColor(sourceImage, sourceImage, cv::COLOR_BGR2GRAY);

    // 画像ファイルが読み込めなかった場合
    if (sourceImage.empty())
    {
        printf("File not found\n");
        exit(0);
    }

    // 画像格納用インスタンスの生成
    cv::Mat captureImage, grayImage, resultImage;
    cv::Mat FilterBank[8];
    cv::Mat FilterImage[8];
    cv::Mat resultImage1, resultImage2, resultImage3, resultImage4, resultImage5, resultImage6;

    // ウィンドウの生成と移動
    cv::namedWindow("gabor0");
    cv::moveWindow("gabor0", 0, 0);
    cv::namedWindow("gabor90");
    cv::moveWindow("gabor90", 50, 0);
    cv::namedWindow("filterd0");
    cv::moveWindow("filterd0", sourceImage.cols, 0);
    cv::namedWindow("filterd90");
    cv::moveWindow("filterd90", sourceImage.cols * 2, 0);
    cv::namedWindow("input");
    cv::moveWindow("input", 0, 0);
    cv::namedWindow("output");
    cv::moveWindow("output", sourceImage.cols, 0);

    // フィルタの生成
    // cv::getGaborKernel(フィルタのサイズ,標準偏差,法線の向き,正弦波因子の波長,空間アスペクト比,位相オフセット,フィルタの種類)

    for (int i = 0; i < 8; i++)
    {
        FilterBank[i] = cv::getGaborKernel(cv::Size(50, 50), 5.0, i * 3.14 / 8, 10.0, 0.5, 0);
    }

    // フィルタの表示
    cv::imshow("gabor0", FilterBank[0]);
    cv::imshow("gabor90", FilterBank[4]);

    // フィルタをかける
    for (int i = 0; i < 8; i++)
    {
        cv::filter2D(sourceImage, FilterImage[i], sourceImage.depth(), FilterBank[i]);
    }

    // フィルタ後の画像の表示
    cv::imshow("input", sourceImage);
    cv::imshow("filterd0", FilterImage[0]);
    cv::imshow("filterd90", FilterImage[4]);

    // フィルタ後の画像の合成
    cv::addWeighted(FilterImage[0], 0.5, FilterImage[1], 0.5, 0, resultImage1);
    cv::addWeighted(FilterImage[2], 0.5, FilterImage[3], 0.5, 0, resultImage2);
    cv::addWeighted(FilterImage[4], 0.5, FilterImage[5], 0.5, 0, resultImage3);
    cv::addWeighted(FilterImage[6], 0.5, FilterImage[7], 0.5, 0, resultImage4);
    cv::addWeighted(resultImage1, 0.5, resultImage2, 0.5, 0, resultImage5);
    cv::addWeighted(resultImage3, 0.5, resultImage4, 0.5, 0, resultImage6);
    cv::addWeighted(resultImage5, 0.5, resultImage6, 0.5, 0, resultImage);

    // 表示
    cv::imshow("input", sourceImage);
    cv::imshow("output", resultImage);
    cv::waitKey(0);
    return 0;
}
