// g++ getImageInfo.cpp -std=c++11 `pkg-config --cflags --libs opencv4` -std=c++17 && ./a.out

#include <iostream>           //入出力関連ヘッダ
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダs
#include <filesystem>         // C++17 filesystem librarys

int main(int argc, const char *argv[])
{

    std::string path = "./testdata"; // 画像フォルダへのパス
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        // 画像ファイルの読み込み
        cv::Mat src_img = cv::imread(entry.path(), cv::IMREAD_COLOR);

        // 画像ファイルが読み込めなかった場合
        if (src_img.data == 0 || src_img.empty())
        {
            printf("File not found\n");
            continue;
        }

        std::cout << entry.path() << std::endl;

        // src_imgをdst_imgにコピー
        cv::Mat dst_img = src_img.clone();

        // グレースケール化
        cv::Mat gray_img;
        cv::cvtColor(src_img, gray_img, cv::COLOR_BGR2GRAY);

        // コーナー座標を格納するための変数cornersを定義
        std::vector<cv::Point2f> corners;

        // ここから記述
        // ハリスのコーナー検出に必要なパラメータの宣言

        int MAX_CORNERS = 100;      // 検出する最大コーナー数
        double QUALITY_LEVEL = 0.2; // コーナーとして選択されるための品質しきい値
        double MIN_DISTANCE = 5.0;  // 検出されたコーナー間の最小距離
        int BLOCK_SIZE = 3;         // コーナーを検出するための近傍領域のサイズ

        // コーナー検出関数を用いて，入力画像からコーナーを検出し座標をcornersに格納

        cv::goodFeaturesToTrack(gray_img, corners, MAX_CORNERS, QUALITY_LEVEL, MIN_DISTANCE, cv::Mat(), BLOCK_SIZE);

        // 検出したコーナー位置に円を描画
        for (size_t i = 0; i < corners.size(); i++)
        {
            // pointに円の中心座標を格納
            cv::Point2f point = corners[i];
            // dst_img上に座標pointを中心とする半径6太さ2の赤い円を描画
            cv::circle(dst_img, point, 6, cv::Scalar(0, 0, 255), 3);
        }

        std::cout << "Number of corners: " << corners.size() << std::endl;

        cv::Mat hsvImg;
        cv::cvtColor(src_img, hsvImg, cv::COLOR_BGR2HSV); // BGRからHSVへの変換

        cv::Scalar avgColor = cv::mean(hsvImg);                   // 画像全体の平均色を計算
        std::cout << "Average hue: " << avgColor[0] << std::endl; // 色相の平均値を出力
    }

    return 0;
}
