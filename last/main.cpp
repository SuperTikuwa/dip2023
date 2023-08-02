//g++ -std=c++11 mc_last.cpp `pkg-config --cflags --libs opencv4`

#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/bgsegm.hpp>

//エフェクト
void drawOpticalFlow(cv::Mat& result, cv::Mat flow)
{
	for(int y=0; y<result.rows; y+=5){
		for(int x=0; x<result.cols; x+=5){
			const cv::Point2f flowatxy = flow.at<cv::Point2f>(y,x) * 1;
			cv::line(result, cv::Point(x,y), cv::Point(std::round(x+flowatxy.x), std::round(y+flowatxy.y)), cv::Scalar(255,0,0));
			cv::circle(result, cv::Point(x,y), 1, cv::Scalar(0,128,0), -1);
		}
	}
}

int main(int, char**)
{
	cv::Mat orgframe;
    cv::Mat firstFrame, frame, frameGray, prevFrame, prevFrameGray, fgmask, fgmaskColor, result;
    cv::VideoCapture cap;
    
    cv::Mat blended;
    std::vector<cv::Mat> results;

    // 入力
    cap.open("domokun.mp4");
    if (!cap.isOpened()) return -1;

	cv::Size imageSize;
	cap >> orgframe;
	cv::resize(orgframe, frame, cv::Size(), 0.5, 0.5);
	imageSize = frame.size();

	int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    cv::VideoWriter writer = cv::VideoWriter("./out.mp4", fourcc, 24.0, imageSize);
        
    char key = 0;
    while (key != 'q') {
        
        // キャプチャしたのをフレームにいれる
        cap >> orgframe;
        if (orgframe.empty()) break;
		cv::resize(orgframe, frame, cv::Size(), 0.5, 0.5);
		imageSize = frame.size();
        
        // 背景差分
		if (firstFrame.empty()) { //背景画像取得
			firstFrame = frame.clone();
		}
		cv::absdiff(frame, firstFrame, fgmask); //画像同士の差分取得
		cv::cvtColor(fgmask, fgmask, cv::COLOR_BGR2GRAY);
		cv::threshold(fgmask, fgmask, 30, 255, cv::THRESH_BINARY);
        
        // クロージング，オープニング処理
        
        // マスク処理
        cv::cvtColor(fgmask, fgmaskColor, cv::COLOR_GRAY2BGR);
		cv::bitwise_and(frame, fgmaskColor, result); //論理積計算AND
        
        // オプティカルフロー
		cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
		if ( !prevFrameGray.empty() ) {
			cv::Mat flow;
			cv::calcOpticalFlowFarneback(prevFrameGray, frameGray, flow, .4, 1, 12, 2, 8, 1.2, 0);
//			drawOpticalFlow(result, flow);
		}
        
        // 1秒に1回、画像を記録
        if ((int)(cap.get(cv::CAP_PROP_POS_FRAMES)) % (int)std::round(cap.get(cv::CAP_PROP_FPS) * 0.5) == 0) {
            
            //コピー
			results.push_back( result.clone() );
			if ( results.size() >= 5 ) {
				results.erase( results.begin() );   //先頭を削除
			}
            
        }
		//ブレンド
		int last = ( int ) results.size() - 1;
		blended = cv::Mat::zeros( frame.rows, frame.cols, CV_8UC3 );
//		for (int i = 0; i <= last; i++) {
			for (int i = last; i > 0; i--) {
			if (results[i].empty()) continue;
			if (i == last) {
				results[i].copyTo(blended);
			} else {
				cv::addWeighted(blended, 0.8, results[i], 0.2, 0.0, blended);
			}
		}
		//現在のフレームのマスク部分を上書き
		frame.copyTo(blended, fgmask);

        cv::imshow("frame", frame);
        cv::imshow("mask", fgmask);
        cv::imshow("result", result);

        //ブレンド結果画像表示
		if(!blended.empty()){
			cv::imshow("blended", blended);
		}
        
        //オプティカルフロー用過去画像の更新
		prevFrameGray = frameGray.clone();
		prevFrame = frame.clone();
        
		if(!blended.empty()){
			writer << blended;
		}
//        writer << result;

        key = cv::waitKey(1);
        
    }

	writer.release();
    cap.release();
	
    return 0;
}
