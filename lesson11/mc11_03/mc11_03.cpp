//g++ mc11_03.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>

int main()
{
    // １.入力画像をグレースケール化する
    cv::Mat image = cv::imread("boxImg3.jpg");
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // ２.ケニーのエッジ検出を適用
    cv::Mat edges;
    cv::Canny(grayImage, edges, 50, 150);

    // ３.線の検出結果をグレースケール画像に描画して表示
    cv::Mat lineDetectionImage;
    cv::cvtColor(grayImage, lineDetectionImage, cv::COLOR_GRAY2BGR);  // グレースケール画像をBGRに変換

    // 4.ハフ変換を適用して直線検出


     // 5.検出された直線を赤色の線で描画
    for (size_t i = 0; i < lines.size(); i++)
    {
        
    }
    // 元画像と直線検出結果を同じウィンドウに表示
    cv::imshow("Line Detection", lineDetectionImage);
    cv::imshow("Gray Image", grayImage);
    cv::imshow("Original Image", image);
    cv::waitKey(0);

    return 0;
}

