// g++ mc08_04.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

int main()
{
    // 画像の読み込み
    cv::Mat img = cv::imread("keyboard.png");
    if (img.empty())
    {
        std::cout << "画像ファイルを読み込めませんでした。" << std::endl;
        return -1;
    }

    cv::Mat gray_img, thresh_img;
    int thresh = 79;

    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
    cv::threshold(gray_img, thresh_img, thresh, 255, cv::THRESH_BINARY);

    // 黒と白を反転させる
    cv::bitwise_not(thresh_img, thresh_img);

    while (1)
    {
        cv::imshow("Thresholded Image", thresh_img);
        char key = cv::waitKey(0);
        if (key == 'q')
        {
            break;
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
        cv::threshold(gray_img, thresh_img, thresh, 255, cv::THRESH_BINARY);
        cv::bitwise_not(thresh_img, thresh_img);
    }

    cv::dilate(thresh_img, thresh_img, cv::Mat(), cv::Point(-1, -1), 3);
    cv::erode(thresh_img, thresh_img, cv::Mat(), cv::Point(-1, -1), 5);
    cv::dilate(thresh_img, thresh_img, cv::Mat(), cv::Point(-1, -1), 2);

    // 輪郭線を検出する
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    for (int i = 0; i < contours.size(); i++)
    {
        // 輪郭線を描画する
        // cv::drawContours(img, contours, i, cv::Scalar(0, 0, 255), 2);

        // 形状特徴パラメータ
        // 外接長方形
        cv::Rect rect = cv::boundingRect(contours[i]);

        cv::rectangle(img, rect, cv::Scalar(0, 0, 255), 2);

        // 重心
        cv::Moments mu = cv::moments(contours[i], false);
        cv::Point2f mc = cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);

        // 重心の表示
        // cv::circle(img, mc, 4, cv::Scalar(0, 255, 255), -1, 8, 0);

        // 物体のIDを重心に表示する
        cv::putText(img, std::to_string(i + 1), mc, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    // 図形が書かれた画像を表示
    cv::imshow("Shapes", img);
    cv::imshow("Thresholded Image", thresh_img);
    cv::waitKey(0);
    cv::waitKey(0);
    cv::waitKey(0);

    return 0;
}
