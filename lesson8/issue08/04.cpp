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
    int thresh = 100;

    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
    cv::threshold(gray_img, thresh_img, thresh, 255, cv::THRESH_BINARY);

    // 黒と白を反転させる
    cv::bitwise_not(thresh_img, thresh_img);

    // 輪郭線を検出する
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    for (int i = 0; i < contours.size(); i++)
    {
        // 輪郭線を描画する
        cv::drawContours(img, contours, i, cv::Scalar(0, 0, 255), 2);

        // 形状特徴パラメータ
        // 外接長方形
        cv::Rect rect = cv::boundingRect(contours[i]);

        // 外接長方形の表示
        cv::rectangle(img, rect, cv::Scalar(0, 255, 0), 2);

        // 重心
        cv::Moments mu = cv::moments(contours[i], false);
        cv::Point2f mc = cv::Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);

        // 重心の表示
        cv::circle(img, mc, 4, cv::Scalar(0, 255, 255), -1, 8, 0);

        // 物体のIDを重心に表示する
        cv::putText(img, std::to_string(i), mc, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);

        // 面積を計算する
        double area = cv::contourArea(contours[i]);

        // バウンディングボックスの下に面積を表示する
        cv::putText(img, std::to_string(area), cv::Point(rect.x, rect.y + rect.height + 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

        // 周囲長を計算する
        double perimeter = cv::arcLength(contours[i], true);

        // バウンディングボックスの下に周囲長を表示する
        cv::putText(img, std::to_string(perimeter), cv::Point(rect.x, rect.y + rect.height + 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

        // 円周度を計算する
        double circularity = 4 * M_PI * area / (perimeter * perimeter);

        // バウンディングボックスの下に円周度を表示する
        cv::putText(img, std::to_string(circularity), cv::Point(rect.x, rect.y + rect.height + 60), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
    }

    // 図形が書かれた画像を表示
    cv::imshow("Shapes", img);
    cv::imshow("Thresholded Image", thresh_img);
    cv::waitKey();
    return 0;
}
