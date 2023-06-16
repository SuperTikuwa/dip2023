// g++ mc08_03.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>
#include "Stats.h"

int main()
{
    // 画像の読み込み
    cv::Mat img = cv::imread("input_image_with_shapes.png", cv::IMREAD_GRAYSCALE);
    if (img.empty())
    {
        std::cout << "画像ファイルを読み込めませんでした。" << std::endl;
        return -1;
    }

    // ヒストグラムの生成
    cv::Mat thresh_img;
    cv::threshold(img, thresh_img, 10, 256, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    // 膨張収縮処理
    int dilate_size = 0;
    int erode_size = 0;

    std::vector<std::string> log;

    while (1)
    {

        // 膨張収縮処理
        // 画像の表示
        cv::imshow("Source Image", img);
        cv::imshow("Thresholded Image", thresh_img);

        char key = cv::waitKey(0);
        if (key == 'e')
        {
            cv::erode(thresh_img, thresh_img, cv::Mat(), cv::Point(-1, -1), 1);
            erode_size++;
            log.push_back("erode");
        }
        else if (key == 'd')
        {
            cv::dilate(thresh_img, thresh_img, cv::Mat(), cv::Point(-1, -1), 1);
            dilate_size++;
            log.push_back("dilate");
        }
        else if (key == ' ')
        {
            printf("dilate_size = %d, erode_size = %d\n", dilate_size, erode_size);
            for (auto &&s : log)
            {
                printf("%s ", s.c_str());
            }
            printf("\n");
        }
        else if (key == 'q')
        {
            break;
        }
    }

    // ヒストグラムの表示

    return 0;
}
