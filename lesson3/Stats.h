//
//  Stats.h
//
//
//  Created by KITASAKATAKAYUKI on 2022/04/27.
//

#ifndef Stats_h
#define Stats_h
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダ

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

}

namespace Stats
{ // 統計量計測関係の名前空間

	int min(cv::Mat src)
	{
		int min = INT_MAX;
		unsigned char value;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				value = src.at<unsigned char>(i, j);
				if ((int)value < min)
				{
					min = (int)value;
				}
			}
		}

		return min;
	}

	int max(cv::Mat src)
	{
		int max = INT_MIN;
		unsigned char value;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				value = src.at<unsigned char>(i, j);
				if ((int)value > max)
				{
					max = (int)value;
				}
			}
		}

		return max;
	}

	double average(cv::Mat src)
	{
		unsigned int value;

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				value += src.at<unsigned char>(i, j);
			}
		}

		double average = (double)value / (double)(src.cols * src.rows);
		return average;
	}

	int median(cv::Mat src)
	{
		std::vector<unsigned char> values(src.cols * src.rows);
		unsigned char value;

		int index = 0;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				value = src.at<unsigned char>(i, j);
				values[index] = value;
				index++;
			}
		}

		int median = 0;
		double medianIndex = (double)(src.cols * src.rows) / 2;
		if (medianIndex == (int)medianIndex)
		{
			median = values[(int)medianIndex];
		}
		else
		{
			median = (values[(int)medianIndex] + values[(int)medianIndex + 1]) / 2;
		}

		return median;
	}
}

#endif /* Stats_h */
