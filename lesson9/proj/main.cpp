// g++ kadai.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>

//! 位置空間フィルタの半径
double posRadius = 50.0;

//! 色空間フィルタの半径
double colorRadius = 30.0;

int main(int argc, const char *argv[])
{
  // 画像読み込み
  cv::Mat sourceImage = cv::imread("pxfuel.jpg", cv::IMREAD_COLOR);
  if (sourceImage.data == 0)
  {
    printf("File not found\n");
    exit(0);
  }

  // 出力画像
  cv::Mat resultImage(cv::Size(sourceImage.cols, sourceImage.rows), CV_8UC3);

  // ミーンシフトの実施
  // cv::pyrMeanShiftFiltering(入力画像, 出力画像, 位置空間フィルターの半径, 色空間フィルターの半径);
  cv::pyrMeanShiftFiltering(sourceImage, resultImage, posRadius, colorRadius);

  // 画像サイズの変換
  while (sourceImage.cols > 700 || sourceImage.rows > 700)
  {
    cv::resize(sourceImage, sourceImage, cv::Size(), 0.9, 0.9);
    cv::resize(resultImage, resultImage, cv::Size(), 0.9, 0.9);
  }

  // 表示ウィンドウ作成
  cv::namedWindow("Source");
  cv::namedWindow("Result");
  cv::moveWindow("Source", 0, 0);
  cv::moveWindow("Result", sourceImage.cols, 0);

  // 表示
  cv::imshow("Source", sourceImage);
  cv::imshow("Result", resultImage);

  cv::waitKey(0);
  cv::waitKey(0);
  cv::waitKey(0);

  // 出力画像の保存
  // cv::imwrite("result_sample1.jpg", resultImage);

  return 0;
}
