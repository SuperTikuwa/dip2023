#include <opencv2/opencv.hpp>
#include "Stats.h"


int main()
{
    // 画像の読み込み
    cv::Mat img = cv::imread("keyboard.png");
    if (img.empty())
    {
        std::cout << "画像ファイルを読み込めませんでした。" << std::endl;
        return -1;
    }
    cv::Mat thresh_img, gray_img;

    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);

    int thresh = 130;
    cv::Size histSize(256, 256);  // ヒストグラム画像のサイズ（256x256ピクセル）
    // ヒストグラムの生成
    cv::Mat histogram = histogram::make(gray_img, histSize);
    histogram::drawLineAt(histogram, histSize, thresh);  // Draw a line at x=50
    histogram::show("Histogram with Line", histogram);

    cv::threshold(gray_img, thresh_img, thresh, 256, cv::THRESH_BINARY_INV);
    // 画像の表示
    cv::imshow("Thresh Image", thresh_img);
    cv::imshow("Labelled Image", img);
    cv::waitKey();

    return 0;
}
