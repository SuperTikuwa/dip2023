// コンパイルコマンド　g++ mc04.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
#include <iostream>           //入出力関連ヘッダ
#include <opencv2/opencv.hpp> //OpenCV関連ヘッダ

std::vector<unsigned char> generateGammaLut()
{
    std::vector<unsigned char> lut(256);
    for (int i = 0; i < 256; i++)
    {
        int v = 2 * i;
        if (v > 255)
        {
            v = 255;
        }
        lut[i] = v;
    }
    return lut;
}

std::vector<unsigned char> generateRandomLut()
{
    std::vector<unsigned char> lut(256);
    for (int i = 0; i < 256; i++)
    {
        lut[i] = rand() % 256;
    }
    return lut;
}

std::vector<unsigned char> generateSolarizeLut()
{
    std::vector<unsigned char> lut(256);
    for (int i = 0; i < 256; i++)
    {
        lut[i] = 255 * (14 * pow((double)i / 255 - 0.5, 3) - 2.5 * ((double)i / 255 - 0.5) + 0.5);
    }
    return lut;
}

std::vector<unsigned char> generatePosterrizationLut(int nGrad)
{
    std::vector<unsigned char> lut(256);
    for (int i = 0; i < 256; i++)
    {
        lut[i] = ((double)255 / (nGrad - 1)) * floor((double)nGrad * i / 256);
    }
    return lut;
}

std::map<std::string, std::vector<unsigned char>> generateThermographyLut()
{
    std::map<std::string, std::vector<unsigned char>> lut;
    lut["red"] = std::vector<unsigned char>(256);
    lut["green"] = std::vector<unsigned char>(256);
    lut["blue"] = std::vector<unsigned char>(256);

    for (int i = 0; i < 256; i++)
    {
        if (i < 64)
        {
            lut["red"][i] = 0;
            lut["green"][i] = i * 4;
            lut["blue"][i] = 255;
        }

        if (i >= 64 && i < 128)
        {
            lut["red"][i] = 0;
            lut["green"][i] = 255;
            lut["blue"][i] = 255 - (i - 64) * 4;
        }

        if (i >= 128 && i < 192)
        {
            lut["red"][i] = (i - 128) * 4;
            lut["green"][i] = 255;
            lut["blue"][i] = 0;
        }

        if (i >= 192)
        {
            lut["red"][i] = 255;
            lut["green"][i] = 255 - (i - 192) * 4;
            lut["blue"][i] = 0;
        }

        if (lut["red"][i] < 0)
        {
            lut["red"][i] = 0;
        }
        if (lut["red"][i] > 255)
        {
            lut["red"][i] = 255;
        }

        if (lut["green"][i] < 0)
        {
            lut["green"][i] = 0;
        }
        if (lut["green"][i] > 255)
        {
            lut["green"][i] = 255;
        }

        if (lut["blue"][i] < 0)
        {
            lut["blue"][i] = 0;
        }
        if (lut["blue"][i] > 255)
        {
            lut["blue"][i] = 255;
        }
    }

    return lut;
}

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
    cv::Mat captureImage;
    cv::Mat beforeFrameImage(cv::Size(width, height), CV_8UC3);
    cv::Mat frameImage(cv::Size(width, height), CV_8UC3);
    cv::Mat grayColorImage(cv::Size(width, height), CV_8UC3);
    cv::Mat grayImage(cv::Size(width, height), CV_8UC1);
    cv::Mat slideImage(cv::Size(width, height), CV_8UC1);
    cv::Mat outputImage(cv::Size(width, height), CV_8UC3);
    cv::Mat grayOutputImage(cv::Size(width, height), CV_8UC1);

    // ウィンドウの生成
    cv::namedWindow("Frame");
    cv::namedWindow("Output");
    cv::moveWindow("Frame", 0, 0);
    cv::moveWindow("Output", 0, 0);

    std::map<std::string, std::vector<unsigned char>> thermographyLut = generateThermographyLut();

    srand(time(NULL));

    bool flag = false;

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
        cv::cvtColor(frameImage, grayImage, cv::COLOR_RGB2GRAY);

        if (beforeFrameImage.empty())
        {
            beforeFrameImage = frameImage.clone();
        }

        cv::Mat reverseImage = cv::Mat(grayImage.size(), CV_8UC1);

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                reverseImage.at<unsigned char>(i, j) = 255 - grayImage.at<unsigned char>(i, j);
            }
        }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i < height - 3 && j < width - 3)
                {
                    slideImage.at<unsigned char>(i, j) = reverseImage.at<unsigned char>(i + 3, j);
                }
            }
        }

        if (flag)
        {
            cv::addWeighted(slideImage, 1.0, grayImage, 1.0, -128.0, grayOutputImage);
            cv::imshow("Output", grayOutputImage);
        }
        else
        {
            cv::addWeighted(beforeFrameImage, 0.5, frameImage, 0.5, 0.0, outputImage);
            cv::imshow("Output", outputImage);
        }

        // 画像表示
        // cv::imshow("Frame", frameImage);

        char key = cv::waitKey(30);
        if (key == 'q' || key == 'Q')
            break;

        if (key == 'c')
        {
            flag = !flag;
        }

        beforeFrameImage = frameImage.clone();
    }

    return 0;
}