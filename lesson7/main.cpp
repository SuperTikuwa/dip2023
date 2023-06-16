// g++ 07_01.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <opencv2/opencv.hpp>

int main()
{
    // 入力画像の読み込み
    cv::Mat inputImage = cv::imread("dog.jpg");

    // アフィン変換行列の作成
    int count = 0;
    while (1)
    {
        cv::Mat affineMatrix = cv::getRotationMatrix2D(cv::Point2f(inputImage.cols / 2, inputImage.rows / 2), count % 360, 1.0);

        // アフィン変換の適用
        cv::Mat outputImage;
        cv::warpAffine(inputImage, outputImage, affineMatrix, inputImage.size());

        // アフィン変換後の画像の表示
        cv::imshow("Input Image", inputImage);
        cv::imshow("Affine Transformation", outputImage);

        count++;
        char c = cv::waitKey(1);
        if (c == 'q')
        {
            break;
        }
    }

    // 射影変換行列の作成
    // cv::Point2f inputPoints[4];
    // cv::Point2f outputPoints[4];

    // // 入力画像の頂点を指定
    // inputPoints[0] = cv::Point2f(0, 0);
    // inputPoints[1] = cv::Point2f(inputImage.cols - 1, 0);
    // inputPoints[2] = cv::Point2f(inputImage.cols - 1, inputImage.rows - 1);
    // inputPoints[3] = cv::Point2f(0, inputImage.rows - 1);

    // // 出力画像の頂点を指定
    // outputPoints[0] = cv::Point2f(0, 0);
    // outputPoints[1] = cv::Point2f(inputImage.cols * 0.7, 0);
    // outputPoints[2] = cv::Point2f(inputImage.cols - 1, inputImage.rows * 0.9);
    // outputPoints[3] = cv::Point2f(inputImage.cols * 0.3, inputImage.rows - 1);

    // 射影変換行列の作成
    // cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(inputPoints, outputPoints);

    // 射影変換の適用
    // cv::warpPerspective(inputImage, outputImage, perspectiveMatrix, inputImage.size());

    // 射影変換後の画像の表示
    // cv::imshow("Perspective Transformation", outputImage);
    // cv::waitKey(0);

    return 0;
}
