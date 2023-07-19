//  g++ task11.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
//  g++ task11.cpp `pkg-config --cflags --libs opencv`

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char *argv[])
{
    cv::VideoCapture video("car_movie.mp4");
    cv::Mat templateImage = cv::imread("car_img.png");
    cv::Mat frame;

    if (!video.isOpened() || templateImage.empty())
    {
        printf("File not found\n");
        exit(0);
    }

    // SIFT特徴点検出と特徴量計算
    // SIFT特徴点検出機器と特徴量計算機を作成
    cv::Ptr<SIFT> sift = SIFT::create(50);

    // 検出された特徴点を格納
    std::vector<KeyPoint> keypoints1, keypoints2;

    // 計算された特徴量を格納
    cv::Mat descriptors1, descriptors2;

    // 2つの特徴量のインデックス・マッチングの距離・およびその他の情報を保持するデータ
    std::vector<DMatch> matches;

    // 画像の特徴点と特徴量を計算

    // Brute-Forceクラスのオブジェクトを生成
    cv::BFMatcher matcher(NORM_L2);

    while (1)
    {
        video >> frame;
        if (frame.empty())
        {
            video.set(cv::CAP_PROP_POS_FRAMES, 0);
            video >> frame;
        }

        // 動画の特徴点と特徴量を計算
        sift->detectAndCompute(frame, noArray(), keypoints1, descriptors1);
        sift->detectAndCompute(templateImage, noArray(), keypoints2, descriptors2);

        // matcher.match関数を使って2つの特徴量セットをマッチングし，matchesに格納
        matcher.match(descriptors1, descriptors2, matches);

        // 画像と動画をマッチング情報をもとに横に並べて表示し，対応する点で結ぶ
        cv::Mat match_img;
        cv::drawMatches(frame, keypoints1, templateImage, keypoints2, matches, match_img);

        // マッチング結果を表示
        cv::imshow("Matches", match_img);
        char key = cv::waitKey(1);

        if (key == 'q' || key == 'Q')
            break;
    }

    video.release();

    return 0;
}
