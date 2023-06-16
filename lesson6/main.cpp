// g++ filter.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
// g++ filter.cpp `pkg-config --cflags --libs opencv`
#include <iostream>           //入出力関連ヘッダ
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダ

int main(int argc, const char *argv[])
{
    // ①画像ファイルの読み込み
    cv::Mat sourceImage = cv::imread("test.jpg", cv::IMREAD_COLOR);

    int width = 720, height = 405;
    cv::Mat frameImage(cv::Size(width, height), CV_8UC3);

    cv::VideoCapture capture("movie.mp4");
    if (capture.isOpened() == 0)
    {
        printf("Camera not found\n");
        return -1;
    }

    if (sourceImage.data == 0)
    { // 画像ファイルが読み込めなかった場合
        printf("File not found\n");
        exit(0);
    }
    printf("Width=%d, Height=%d\n", sourceImage.cols, sourceImage.rows);

    // ②画像格納用インスタンスの生成
    cv::Mat grayImage, aveImage, gaussianImage, diffImage, laplacianImage, medianImage, cannyImage;
    cv::Mat binImage;
    cv::Mat captureImage;

    // ③ウィンドウの生成と移動
    cv::namedWindow("gray");
    cv::moveWindow("gray", 0, 0);
    cv::namedWindow("average");
    cv::moveWindow("average", sourceImage.cols, 0);
    cv::namedWindow("gaussian");
    cv::moveWindow("gaussian", sourceImage.cols * 2, 0);
    cv::namedWindow("diff");
    cv::moveWindow("diff", 0, sourceImage.rows);
    cv::namedWindow("laplacian");
    cv::moveWindow("laplacian", sourceImage.cols, sourceImage.rows);

    // ④フィルター配列作成

    // 平均化フィルター
    float Fave[] = {
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
        1.0 / 9.0,
    };
    cv::Mat kernel(cv::Size(3, 3), CV_32F, Fave);

    // ガウシアンフィルター
    float gaussianFilter[] = {
        1.0 / 16.0,
        2.0 / 16.0,
        1.0 / 16.0,
        2.0 / 16.0,
        4.0 / 16.0,
        2.0 / 16.0,
        1.0 / 16.0,
        2.0 / 16.0,
        1.0 / 16.0,
    };
    cv::Mat gaussianKernel(cv::Size(3, 3), CV_32F, gaussianFilter);

    // 微分フィルター
    float diffFilter[] = {
        0,
        -1,
        0,
        -1,
        4,
        -1,
        0,
        -1,
        0,
    };
    cv::Mat diffKernel(cv::Size(3, 3), CV_32F, diffFilter);

    // ラプラシアンフィルター
    float laplacianFilter[] = {
        0,
        -1,
        0,
        -1,
        4,
        -1,
        0,
        -1,
        0,
    };
    cv::Mat laplacianKernel(cv::Size(3, 3), CV_32F, laplacianFilter);

    while (1)
    {
        capture >> captureImage;
        if (captureImage.empty())
        {
            capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            capture >> captureImage;
        }

        cv::resize(captureImage, frameImage, frameImage.size());

        // ⑤画像処理
        cv::cvtColor(frameImage, grayImage, cv::COLOR_BGR2GRAY);      // モノクロ化
        cv::filter2D(grayImage, aveImage, grayImage.depth(), kernel); // 平均かフィルタ適用
        cv::filter2D(grayImage, gaussianImage, grayImage.depth(), gaussianKernel);
        cv::filter2D(grayImage, diffImage, grayImage.depth(), diffKernel);
        cv::filter2D(grayImage, laplacianImage, grayImage.depth(), laplacianKernel);
        cv::medianBlur(frameImage, medianImage, 11);
        cv::Canny(grayImage, cannyImage, 50, 200);

        cv::imshow("source", frameImage);
        // ⑥ウィンドウへの画像の表示
        cv::imshow("gray", grayImage);
        cv::imshow("average", aveImage);
        cv::imshow("gaussian", gaussianImage);
        cv::imshow("diff", diffImage);
        cv::imshow("laplacian", laplacianImage);
        cv::imshow("median", medianImage);
        cv::imshow("canny", cannyImage);

        char key = cv::waitKey(1);

        if (key == 'q')
        {
            break;
        }
    }

    return 0;
}
