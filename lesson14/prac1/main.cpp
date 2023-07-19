// g++ -std=c++17 main.cpp `pkg-config --cflags --libs opencv4` && ./a.out

#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    // カメラキャプチャの開始
    cv::VideoCapture cap("./video.mp4");
    if (!cap.isOpened())
    {
        std::cout << "カメラを開けませんでした。" << std::endl;
        return -1;
    }

    cv::Mat prev_frame, current_frame, frame_diff, threshold_frame;

    // 初期フレームを取得して、前のフレームとして保存
    cap >> prev_frame;
    cv::cvtColor(prev_frame, prev_frame, cv::COLOR_BGR2GRAY);

    while (true)
    {
        // 現在のフレームを取得
        cap >> current_frame;
        cv::cvtColor(current_frame, current_frame, cv::COLOR_BGR2GRAY);

        // フレーム間の差分を計算
        // TODO: Please write here
        cv::absdiff(prev_frame, current_frame, frame_diff);

        // 閾値処理を行い、差分が大きい部分を白に変換
        // TODO: Please write here
        cv::threshold(frame_diff, threshold_frame, 50, 255, cv::THRESH_BINARY);

        // 結果を表示
        cv::imshow("Frame Difference", threshold_frame);

        // 'q'キーが押されたら終了
        if (cv::waitKey(10) == 'q')
        {
            break;
        }

        // 現在のフレームを前のフレームとして保存
        prev_frame = current_frame.clone();
    }

    // カメラキャプチャを解放
    cap.release();

    // ウィンドウを閉じる
    cv::destroyAllWindows();

    return 0;
}