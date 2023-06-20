// g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>

int main(int argc, const char *argv[])
{
  // 画像読み込み
  cv::VideoCapture capture("images/game.mp4");
  cv::Mat templateImage = cv::imread("images/player.png");

  cv::Mat captureImage;
  int width = 720, height = 405;
  cv::Mat frameImage(cv::Size(width, height), CV_8UC3);

  if (!capture.isOpened())
  {
    printf("Video not found\n");
    return -1;
  }

  if (templateImage.empty())
  {
    printf("File not found\n");
    exit(0);
  }

  while (1)
  {
    capture >> captureImage;
    if (captureImage.empty())
    {
      capture.set(cv::CAP_PROP_POS_FRAMES, 0);
      capture >> captureImage;
    }

    // 出力画像
    cv::Mat resultImage(cv::Size(captureImage.cols, captureImage.rows), CV_8UC3);
    cv::Point matchPoint;
    cv::Rect resultRect(0, 0, templateImage.cols, templateImage.rows);
    double val;

    // 距離変換画像とテンプレート画像のエッジ画像でマッチング
    cv::matchTemplate(captureImage, templateImage, resultImage, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(resultImage, NULL, &val, NULL, &matchPoint);

    resultRect.x = matchPoint.x;
    resultRect.y = matchPoint.y;

    cv::rectangle(captureImage, resultRect, cv::Scalar(0, 0, 255), 3);

    cv::imshow("Source", captureImage);
    cv::imshow("Template", templateImage);
    cv::imshow("Result", resultImage);

    char key = cv::waitKey(1);

    if (key == 'q')
    {
      break;
    }
    // cv::imshow("Result", resultImage);
  }

  return 0;
}
