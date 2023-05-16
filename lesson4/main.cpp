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
    cv::Mat captureImage;                                          // オリジナルの入力画像格納用（大抵大きすぎる）
    cv::Mat frameImage(cv::Size(width, height), CV_8UC3);          // コンストラクタによる宣言時のインスタンス確保
    cv::Mat randomImage(cv::Size(width, height), CV_8UC3);         // コンストラクタによる宣言時のインスタンス確保
    cv::Mat solarizeImage(cv::Size(width, height), CV_8UC3);       // コンストラクタによる宣言時のインスタンス確保
    cv::Mat posterrizationImage(cv::Size(width, height), CV_8UC3); // コンストラクタによる宣言時のインスタンス確保
    // cv::Mat frameImage = cv::Mat(cv::Size(width, height), CV_8UC3);  //こう書いても良い．宣言した後にインスタンス設定

    // ウィンドウの生成
    cv::namedWindow("Frame");
    cv::namedWindow("Random");
    cv::namedWindow("Solarize");
    cv::namedWindow("Posterrization");
    cv::moveWindow("Frame", 0, 0);
    cv::moveWindow("Random", 0, 0);
    cv::moveWindow("Solarize", 0, 0);
    cv::moveWindow("Posterrization", 0, 0);

    std::vector<unsigned char> lut(256);
    std::vector<unsigned char> gammaLut(256);
    std::vector<unsigned char> randomLut(256);
    std::vector<unsigned char> solarizeLut(256);
    std::vector<unsigned char> posterrizationLut(256);

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

    int nGrad = 8;
    for (int i = 0; i < 256; i++)
    {
        posterrizationLut[i] = ((double)255 / (nGrad - 1)) * floor((double)nGrad * i / 256);
    }

    for (int i = 0; i < 256; i++)
    {
        randomLut[i] = rand() % 256;
    }

    for (int i = 0; i < 256; i++)
    {

        solarizeLut[i] = 255 * (14 * pow((double)i / 255 - 0.5, 3) - 2.5 * ((double)i / 255 - 0.5) + 0.5);
    }

    while (1)
    {
        // カメラから1フレーム取り出し
        capture >> captureImage;

        if (captureImage.empty())
        {
            capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            capture >> captureImage;
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
                randomImage.at<cv::Vec3b>(i, j) = v;

                v[0] = solarizeLut[s[0]];
                v[1] = solarizeLut[s[1]];
                v[2] = solarizeLut[s[2]];
                solarizeImage.at<cv::Vec3b>(i, j) = v;

                v[0] = posterrizationLut[s[0]];
                v[1] = posterrizationLut[s[1]];
                v[2] = posterrizationLut[s[2]];
                posterrizationImage.at<cv::Vec3b>(i, j) = v;
            }
        }

        // 画像表示
        cv::imshow("Frame", frameImage);
        cv::imshow("Random", randomImage);
        cv::imshow("Posterrization", posterrizationImage);
        cv::imshow("Solarize", solarizeImage);

        char key = cv::waitKey(30);
        if (key == 'q' || key == 'Q')
            break;
    }

    return 0;
}