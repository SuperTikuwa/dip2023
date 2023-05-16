// コンパイルコマンド　g++ mc04.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <iostream>           //入出力関連ヘッダ
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダ

int main(int argc, const char *argv[])
{
    // 内蔵カメラからビデオキャプチャ"capture"生成
    cv::VideoCapture capture("movie.mp4"); //"water.mov"にすればムービーファイルから映像を取り込む
    // キャプチャできたかチェック
    if (capture.isOpened() == 0)
    {
        printf("Camera not found\n");
        return -1;
    }

    // 画像サイズの指定
    int width = 720, height = 405;

    // 画像格納用インスタンス準備
    cv::Mat captureImage;                                  // オリジナルの入力画像格納用（大抵大きすぎる）
    cv::Mat frameImage(cv::Size(width, height), CV_8UC3);  // コンストラクタによる宣言時のインスタンス確保
    cv::Mat outputImage(cv::Size(width, height), CV_8UC3); // コンストラクタによる宣言時のインスタンス確保
    // cv::Mat frameImage = cv::Mat(cv::Size(width, height), CV_8UC3);  //こう書いても良い．宣言した後にインスタンス設定

    // ウィンドウの生成
    cv::namedWindow("Frame");
    cv::namedWindow("Output");
    cv::moveWindow("Frame", 0, 0);
    cv::moveWindow("Output", 0, 0);

    std::vector<unsigned char> lut(256);
    std::vector<unsigned char> gammaLut(256);
    std::vector<unsigned char> randomLut(256);
    std::vector<unsigned char> solarizeLut(256);

    srand(time(NULL));

    for (int i = 0; i < 256; i++)
    {
        int v = 2 * i;
        if (v > 255)
        {
            v = 255;
        }
        lut[i] = v;
    }

    for (int i = 0; i < 256; i++)
    {
        // double gamma = 2.0;
        // unsigned char v = (unsigned char)(255.0 * pow((i / 255.0), gamma));
        // gammaLut[i] = v;
    }

    for (int i = 0; i < 256; i++)
    {
        randomLut[i] = rand() % 256;
    }

    for (int i = 0; i < 256; i++)
    {
        if (i < 64)
        {
            solarizeLut[i] = i * 4;
        }
        else if (i < 128)
        {
            solarizeLut[i] = 255 - i * 4;
        }
        else if (i < 192)
        {
            solarizeLut[i] = (i - 192) * 4;
        }

        while (1)
        {
            // カメラから1フレーム取り出し
            capture >> captureImage;

            if (captureImage.empty())
            {
                break;
            }

            // 画像の大きさ変更
            cv::resize(captureImage, frameImage, frameImage.size());

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    cv::Vec3b s = frameImage.at<cv::Vec3b>(i, j);
                    cv::Vec3b v;
                    v[0] = randomLut[s[0]];
                    v[1] = randomLut[s[1]];
                    v[2] = randomLut[s[2]];

                    outputImage.at<cv::Vec3b>(i, j) = v;
                }
            }

            // 画像表示
            cv::imshow("Frame", frameImage);
            cv::imshow("Output", outputImage);

            char key = cv::waitKey(30);
            if (key == 'q' || key == 'Q')
                break;
        }

        return 0;
    }