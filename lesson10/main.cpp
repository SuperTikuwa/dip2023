// g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>

//! 位置空間フィルタの半径
double posRadius = 50.0;

//! 色空間フィルタの半径
double colorRadius = 30.0;

int main(int argc, const char *argv[])
{
  // 画像読み込み
  cv::Mat sourceImage = cv::imread("images/source.jpg");
  cv::Mat templateImage = cv::imread("images/big_image.png");

  cv::Mat graySourceImage, grayTemplateImage;
  cv::Mat binarySourceImage, binaryTemplateImage;
  cv::

      if (sourceImage.empty() || templateImage.empty())
  {
    printf("File not found\n");
    exit(0);
  }

  // 出力画像
  cv::Mat resultImage(cv::Size(sourceImage.cols, sourceImage.rows), CV_8UC3);
  cv::Point matchPoint;

  cv::Rect resultRect(0, 0, templateImage.cols, templateImage.rows);
  double val;

  cv::cvtColor(sourceImage, graySourceImage, cv::COLOR_BGR2GRAY);
  cv::threshold(graySourceImage, binarySourceImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
  cv::Sobel(binarySourceImage, )

      cv::matchTemplate(sourceImage, templateImage, resultImage, cv::TM_CCOEFF_NORMED);
  cv::minMaxLoc(resultImage, NULL, &val, NULL, &matchPoint);

  // std::cout << maxVal << std::endl;
  std::cout << matchPoint << std::endl;
  resultRect.x = matchPoint.x;
  resultRect.y = matchPoint.y;

  cv::rectangle(sourceImage, resultRect, cv::Scalar(0, 0, 255), 3);

  cv::imshow("Source", sourceImage);
  cv::imshow("Template", templateImage);
  cv::imshow("Result", resultImage);

  cv::waitKey(0);

  return 0;
}
