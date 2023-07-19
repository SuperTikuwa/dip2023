//  g++ mc11_01.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
//  g++ mc11_01.cpp `pkg-config --cflags --libs opencv`

#include <iostream>  //入出力関連ヘッダ
#include <opencv2/opencv.hpp>  //OpenCV関連ヘッダ

int main (int argc, const char* argv[]) {
    
    // 画像ファイルの読み込み
    cv::Mat src_img = cv::imread("sample.jpg", cv::IMREAD_COLOR);
    
    // 画像ファイルが読み込めなかった場合
    if (src_img.data == 0 || src_img.empty()) {
        printf("File not found\n");
        exit(0);
    }
    
    // src_imgをdst_imgにコピー
    cv::Mat dst_img = src_img.clone();

    // グレースケール化
    cv::Mat gray_img;
    cv::cvtColor(src_img, gray_img, cv::COLOR_BGR2GRAY);

    // コーナー座標を格納するための変数cornersを定義
    std::vector<cv::Point2f> corners;
    
    // ここから記述
    // ハリスのコーナー検出に必要なパラメータの宣言
    
    
    
    
    
    
    // コーナー検出関数を用いて，入力画像からコーナーを検出し座標をcornersに格納
    
    
    
    // 検出したコーナー位置に円を描画
    for (size_t i = 0; i < corners.size(); i++) {
        // pointに円の中心座標を格納
        cv::Point2f point = corners[i];
        // dst_img上に座標pointを中心とする半径6太さ2の赤い円を描画
        cv::circle(dst_img, point, 6, cv::Scalar(0, 0, 255), 3);
    }
    
    // 表示
    cv::imshow("input", src_img);
    cv::imshow("result", dst_img);
    cv::waitKey(0);

    return 0;
}
