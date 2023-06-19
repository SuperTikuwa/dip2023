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
  cv::Mat templateImage = cv::imread("images/template.png");

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

  // cv::matchTemplate(sourceImage, templateImage, resultImage, cv::TM_SQDIFF);
  // cv::minMaxLoc(resultImage, &val, NULL, &matchPoint, NULL);

  // cv::matchTemplate(sourceImage, templateImage, resultImage, cv::TM_CCOEFF_NORMED);
  // cv::minMaxLoc(resultImage, NULL, &val, NULL, &matchPoint);

  for (int i = 0; i < sourceImage.rows - templateImage.rows; i++)
  {
    for (int j = 0; j < sourceImage.cols - templateImage.cols; j++)
    {
      sourceImage.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, i % 255);
    }
  }

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
