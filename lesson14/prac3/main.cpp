// コンパイルコマンド：make all

#include <iostream>

#include "particlefilter.hpp"

using namespace std;

/**
 * より右下に近い座標か判定する
 * Arg: 比較する2点
 * Ret: 
 *   - p1がp2より右側ならtrue
 *   - 左右方向で同位置な場合に限りp1がp2よりも下側ならtrue
 *   - 上記以外はfalse
*/
bool largerPoint(cv::Point p1, cv::Point p2) {
	if (p1.x != p2.x) return p1.x > p2.x;
	return p1.y > p2.y;
}

int main(void){
	/*--- 0. フラグおよび変数の初期化 ---*/
    bool initialization_done = false;
    cv::Mat frame;

	/*--- 1. ビデオの準備 ---*/
    cv::VideoCapture video(0);
    if (!video.isOpened()){
        cerr << "Unable to open your video" << endl;
		exit(1);
    }

	/*--- 2. パーティクルフィルターを準備 ---*/
    ParticleFilter *pf = new ParticleFilter();

	/*--- 3. 動画に対してパーティクルフィルターを適用 ---*/
    while (true){
        video >> frame;

		// フレーム画像が空すなわち動画が終了
        if (frame.empty())
            break;

        if (!initialization_done) {
            std::vector<int> upper = { frame.size().width, frame.size().height, 10, 10 };
            std::vector<int> lower = { 0, 0, -10, -10 };
            std::vector<int> noise = { 30, 30, 10, 10 };

            pf = new ParticleFilter(300, frame.size().height, frame.size().width, upper, lower, noise);
            initialization_done = true;
        }

        pf->Resampling();
        pf->Predict();
        pf->CalcWeight(frame);

        // パーティクルの描画
        std::vector<Particle> particles = pf->GetParticles();
		cv::Point lower_left, upper_right;
        for (int i = 0;i < particles.size();i++) {
			cv::Point coordinate = cv::Point(particles[i].width, particles[i].height);
            cv::circle(frame, coordinate, 1, cv::Scalar(0, 255, 255), -1);
			if (i == 0) lower_left = upper_right = coordinate;
			else {
				if (!largerPoint(lower_left, coordinate)) lower_left = coordinate;
				if (largerPoint(upper_right, coordinate)) upper_right = coordinate;
			}
		}
	    cv::rectangle(frame, lower_left, upper_right, cv::Scalar(0, 255, 255), 3);

		// 結果の表示
        cv::imshow("Result", frame);

        char c = (char)cv::waitKey(1);
        if (c == 'q' || c == 'Q')
            break;
    }

	return 0;
}
