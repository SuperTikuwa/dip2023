// コンパイルコマンド：g++ meanshift.cpp -std=c++11 `pkg-config --cflags --libs opencv4`

//=============================================================
// プログラム概要
// ミーンシフトトラッキングを行う。
// ミーンシフトトラッキングは色ヒストグラムを基に物体追跡を行うため、
// 追跡対象の色が変わったり、類似する色を持つ他の物体に追跡が寄せられたり
// することを、動作を以て実感する。
//=============================================================

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

using namespace std;

int main()
{
	/*--- 1. 動画ファイルを開く ---*/
	string file_name = "./game.mp4";
	cv::VideoCapture capture(file_name);
	if (!capture.isOpened()) {
		// Error handling
		cerr << "Unable to open file!" << endl;
		exit(1);
	}

	/*--- 2. 動画出力用writerを開く ---*/
	cv::VideoWriter writer;
	int width, height, fourcc;
	fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
	double fps;
	width  = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	fps    = capture.get(cv::CAP_PROP_FPS);
	writer.open("output.mp4", fourcc, fps, cv::Size(width, height));

	/*--- 3. 初めの画像でウィンドウを指定し色ヒストグラムを構築 ---*/
	cv::Mat frame, roi, hsv_roi, mask;
	capture >> frame;
	cv::Rect track_window(273, 185, 80, 80);

	/*--- 4. ROIの設定 ---*/
	roi = frame(track_window);
	cv::cvtColor(roi, hsv_roi, cv::COLOR_BGR2HSV);
	cv::inRange(hsv_roi, cv::Scalar(0, 60, 32), cv::Scalar(180, 255, 255), mask);
	float _range[] = {0, 180};
	const float* range[] = {_range};
	cv::Mat roi_hist;
	int histSize[] = {180};
	int channels[] = {0};
	cv::calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
	cv::normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);

	/*--- 5. 終了基準の設定 ---*/
	cv::TermCriteria term_crit(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1);

	/*--- 6. 動画に対する物体追跡を実施 ---*/
	while (true) {
		cv::Mat hsv, dst;
		capture >> frame;

		// フレーム画像が空すなわち動画が終了
		if (frame.empty()) {
			cv::waitKey(0);
			break;
		}
		
		// 色空間を変換
		cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
		cv::calcBackProject(&hsv, 1, channels, roi_hist, dst, range);

		// ミーンシフトトラッキングを実行し追跡対象の位置を特定
		// int meanShift(const Mat& probImage, Rect& window, TermCriteria criteria)
		// TODO: Please write here

		// 追跡結果を表示
		cv::rectangle(frame, track_window, cv::Scalar(255, 0, 0), 5);
		cv::imshow("Result", frame);
		writer << frame;
		int key = cv::waitKey(30);
		if (key == 'q' || key == 'Q')
			break;
	}

	return 0;
}