// g++ mc08_01.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
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

    // ヒストグラムの生成
    int thresh = 0; // 　しきい値
    cv::Mat thresh_img;
    cv::Size histSize(256, 256); // ヒストグラム画像のサイズ（256x256ピクセル）

    cv::Mat histogram = histogram::make(img, histSize);

    // しきい値の箇所に赤線を引く
    histogram::drawLineAt(histogram, histSize, thresh);
    histogram::show("Histogram with Line", histogram);

    // 2値化
    cv::threshold(img, thresh_img, thresh, 256, cv::THRESH_BINARY);

    // 画像の表示
    cv::imshow("Source image", img);
    cv::imshow("Thresholded Image", thresh_img);

    char key = cv::waitKey(0);
    if (key == 'q')
    {
        return 0;
    }
    else if (key == 'u')
    {
        thresh++;
    }
    else if (key == 'd')
    {
        thresh--;
    }
    else if (key == ' ')
    {
        printf("thresh = %d\n", thresh);
    }

    return 0;
}
