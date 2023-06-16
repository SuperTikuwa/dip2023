//
//  Stats.h
//
//
//  Created by KITASAKATAKAYUKI on 2022/04/27.
//

#ifndef Stats_h
#define Stats_h

namespace histogram
{ // Stats名前空間

	cv::Mat make(const cv::Mat src, const cv::Size hsize)
	{
		cv::Mat histImage;

		if (src.channels() != 1)
		{
			std::cout << "入力画像はグレースケールのみです" << std::endl;
			return histImage;
		}

		std::vector<int> histArray(hsize.width); // ヒストグラム配列

		// ヒストグラムの計算
		unsigned char v;
		for (int j = 0; j < src.rows; j++)
		{
			for (int i = 0; i < src.cols; i++)
			{
				// 画素値の頻度計測
				v = src.at<unsigned char>(j, i);
				histArray[v]++;
			}
		}

		histImage = cv::Mat(hsize, CV_8UC3);

		// 最大頻度を求める
		int max = 0;
		for (int i = 0; i < hsize.width; i++)
		{
			if (max < histArray[i])
			{
				max = histArray[i];
			}
		}

		cv::Point s, e;
		for (int i = 0; i < hsize.width; i++)
		{
			s.x = i;
			s.y = hsize.height - 0;
			e.x = i;
			e.y = hsize.height - (int)(histArray[i] * hsize.height / max);
			cv::line(histImage, s, e, CV_RGB(255, 255, 255));
		}

		return histImage;
	}

	void show(const std::string winname, const cv::Mat hImage)
	{
		cv::imshow(winname, hImage);
	}
	
	void drawLineAt(cv::Mat &histImage, const cv::Size hsize, const int xPos)
	{
		if (xPos < 0 || xPos >= hsize.width)
		{
			std::cout << "xPos is out of histogram bounds" << std::endl;
			return;
		}

		cv::Point start, end;
		start.x = end.x = xPos;
		start.y = 0;
		end.y = hsize.height;
		cv::line(histImage, start, end, CV_RGB(255, 0, 0)); // Draw a red line
	}

}

namespace Stats
{ // 統計量計測関係の名前空間

	std::vector<int> calcHist(const cv::Mat src, const cv::Size hsize)
	{
		std::vector<int> histArray(hsize.width); // ヒストグラム配列
		// ヒストグラムの計算
		unsigned char v;
		for (int j = 0; j < src.rows; j++)
		{
			for (int i = 0; i < src.cols; i++)
			{
				// 画素値の頻度計測
				v = src.at<unsigned char>(j, i);
				histArray[v]++;
			}
		}
		return histArray;
	}

	// グレースケール
	unsigned char min(std::vector<int> hist)
	{
		int _min = 10000000;
		unsigned char v;
		for (int i = 0; i < hist.size(); i++)
		{
			if (hist[i] > 0)
			{
				_min = i;
				break;
			}
		}

		return _min;
	}

}

#endif /* Stats_h */
