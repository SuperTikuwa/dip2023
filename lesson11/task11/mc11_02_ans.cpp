//  g++ sift.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
//  g++ sift.cpp `pkg-config --cflags --libs opencv`

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main (int argc, const char* argv[]) {

    // 画像ファイルの読み込み
    cv::Mat src_img1 = cv::imread("Lenna1.png", cv::IMREAD_COLOR);
    cv::Mat src_img2 = cv::imread("Lenna2.png", cv::IMREAD_COLOR);
    
    // 画像ファイルが読み込めなかった場合
    if (src_img1.empty() || src_img2.empty()) {
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
    
    //ここに記述
    // 2つの画像に対して特徴点と特徴量を計算
    
    sift->detectAndCompute(src_img1, noArray(), keypoints1, descriptors1);
    sift->detectAndCompute(src_img2, noArray(), keypoints2, descriptors2);

    // Brute-Forceクラスのオブジェクトを生成
    
    cv::BFMatcher matcher(NORM_L2);
    
    // matcher.match関数を使って2つの特徴量セットをマッチングし，matchesに格納
    
    matcher.match(descriptors1, descriptors2, matches);
    

    // 2つの画像をマッチング情報をもとに横に並べて表示し，対応する点を線で結ぶ
    cv::Mat match_img;
    cv::drawMatches(src_img1, keypoints1, src_img2, keypoints2, matches, match_img);
    
    // マッチング結果を表示
    cv::imshow("Lenna1", src_img1);
    cv::imshow("Lenna2", src_img2);
    cv::imshow("Matches", match_img);
    cv::waitKey(0);

    return 0;
}
