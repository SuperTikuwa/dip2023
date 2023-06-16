#include <stdio.h>
#include <opencv2/opencv.hpp>

int main()
{
  cv::Mat source_img = cv::imread("keyboard.png", cv::IMREAD_GRAYSCALE);

  if (source_img.empty())
  {
    printf("Failed to open image file.\n");
    return -1;
  }

  cv::Mat thresh_img;
  cv::threshold(source_img, thresh_img, 10, 256, cv::THRESH_BINARY | cv::THRESH_OTSU);

  cv::erode(thresh_img, thresh_img, cv::Mat(), cv::Point(-1, -1), 2);

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(thresh_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

  for (int i = 0; contours.size(); i++)
  {
    cv::drawContours(thresh_img, contours, i, cv::Scalar(0, 0, 255), 2);
  }

  cv::imshow("Source Image", source_img);
  cv::imshow("Thresholded Image", thresh_img);
  cv::waitKey();
}