// g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4` -std=c++17 && ./a.out
#include <opencv2/opencv.hpp>

int main()
{

    cv::VideoCapture cap("./video.mp4");

    // Haar-like特徴分類器の読み込み
    cv::CascadeClassifier face_cascade;
    face_cascade.load("./haarcascade/haarcascade_frontalface_default.xml");

    while (1)
    {
        if (!cap.isOpened())
        {
            std::cout << "カメラが開けません" << std::endl;
            return -1;
        }

        cv::Mat image;
        cap >> image;

        // グレースケール変換
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

        // 顔を検知
        std::vector<cv::Rect> faces;

        // 穴埋め

        face_cascade.detectMultiScale(gray_image, faces);

        for (int i = 0; i < faces.size(); i++)
        {
            cv::rectangle(image, faces[i], cv::Scalar(255, 0, 0), 2);
        }

        cv::imshow("camera", image);

        char key = cv::waitKey(10);
        if (key == 'q')
        {
            break;
        }
    }

    // 何かキーを押したら終了
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
