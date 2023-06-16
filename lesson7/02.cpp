// g++ 07_02.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
// g++ 07_02.cpp `pkg-config --cflags --libs opencv`#include "opencv2/core/core.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

int main(int argc, const char *argv[])
{
    // 画像の読み込み
    cv::Mat Source1 = cv::imread("A.png", cv::IMREAD_COLOR);
    if (Source1.empty())
    {
        std::cout << "Failed to open Source image1" << std::endl;
        return -1;
    }

    cv::Mat Source2 = cv::imread("B.png", cv::IMREAD_COLOR);
    if (Source2.empty())
    {
        std::cout << "Failed to open Source image2" << std::endl;
        return -1;
    }

    cv::Mat Source3 = cv::imread("C.png", cv::IMREAD_COLOR);
    if (Source3.empty())
    {
        std::cout << "Failed to open Source image3" << std::endl;
        return -1;
    }

    // 画像を配列に格納
    std::vector<cv::Mat> Sources;
    Sources.push_back(Source1);
    Sources.push_back(Source2);
    Sources.push_back(Source3);

    // A-KAZE検出器の生成
    cv::Ptr<cv::Feature2D> akaze = cv::AKAZE::create();

    // 画像を読み込み、特徴点を計算
    std::vector<cv::KeyPoint> kp1, kp2;
    cv::Mat des1, des2;
    akaze->detectAndCompute(Source1, cv::noArray(), kp1, des1);
    akaze->detectAndCompute(Source2, cv::noArray(), kp2, des2);

    // 特徴点を描画した画像を作成
    cv::Mat img1_with_keypoints, img2_with_keypoints;
    cv::drawKeypoints(Source1, kp1, img1_with_keypoints, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
    cv::drawKeypoints(Source2, kp2, img2_with_keypoints, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

    // 画像を表示
    // cv::namedWindow("Image 1 with Keypoints", cv::WINDOW_NORMAL);
    // cv::namedWindow("Image 2 with Keypoints", cv::WINDOW_NORMAL);
    cv::imshow("Image 1 with Keypoints", img1_with_keypoints);
    cv::imshow("Image 2 with Keypoints", img2_with_keypoints);

    // 特徴点のマッチングを行い、マッチング結果を描画
    cv::Ptr<cv::DescriptorMatcher> bf = cv::BFMatcher::create();
    std::vector<std::vector<cv::DMatch>> matches;
    bf->knnMatch(des1, des2, matches, 2);

    float ratio = 0.5f;
    std::vector<cv::DMatch> good_matches;
    for (size_t i = 0; i < matches.size(); i++)
    {
        if (matches[i][0].distance < ratio * matches[i][1].distance)
        {
            good_matches.push_back(matches[i][0]);
        }
    }

    cv::Mat img_matches;
    cv::drawMatches(Source1, kp1, Source2, kp2, good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // マッチング結果を表示
    // cv::namedWindow("Matches", cv::WINDOW_NORMAL);
    cv::imshow("Matches", img_matches);

    // パノラマ画像の生成用インスタンスを生成
    cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);

    // 出力画像の宣言
    cv::Mat destination;
    // パノラマ画像の生成
    cv::Stitcher::Status status = stitcher->stitch(Sources, destination);
    if (status != cv::Stitcher::OK)
    {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
        return -1;
    }

    // 画像を表示
    // cv::namedWindow("Panorama", cv::WINDOW_AUTOSIZE);
    cv::imshow("Panorama", destination);
    cv::waitKey(0);

    return 0;
}
