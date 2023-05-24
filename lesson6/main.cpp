//g++ filter.cpp -std=c++11 `pkg-config --cflags --libs opencv4`
//g++ filter.cpp `pkg-config --cflags --libs opencv`
#include <iostream>  //入出力関連ヘッダ
#include <opencv2/opencv.hpp>  //OpenCV関連ヘッダ

int main (int argc, const char* argv[])
{
    //①画像ファイルの読み込み
	cv::Mat sourceImage = cv::imread("test.jpg", cv::IMREAD_COLOR);

    if (sourceImage.data==0) {  //画像ファイルが読み込めなかった場合
        printf("File not found\n");
        exit(0);
    }
    printf("Width=%d, Height=%d\n", sourceImage.cols, sourceImage.rows);
    
    //②画像格納用インスタンスの生成
	cv::Mat grayImage,aveImage;
    cv::Mat binImage;
	
    
    //③ウィンドウの生成と移動
    cv::namedWindow("gray");
    cv::moveWindow("gray", 0,0);
    cv::namedWindow("average");
    cv::moveWindow("average", sourceImage.cols,0);
   
    //④フィルター配列作成
    
    //平均化フィルター
    float Fave[]={
        1.0/9.0, 1.0/9.0, 1.0/9.0,
        1.0/9.0, 1.0/9.0, 1.0/9.0,
        1.0/9.0, 1.0/9.0, 1.0/9.0,
    };
    cv::Mat kernel(cv::Size(3,3),CV_32F,Fave);

    //ガウシアンフィルター
    
    //微分フィルター
    
    //ラプラシアンフィルター
    
    
    
    //⑤画像処理
    cv::cvtColor(sourceImage,grayImage,cv::COLOR_BGR2GRAY);//モノクロ化
    cv::filter2D(grayImage,aveImage,grayImage.depth(),kernel);//平均かフィルタ適用
   
    //⑥ウィンドウへの画像の表示
    cv::imshow("gray", grayImage);
    cv::imshow("average", aveImage);
   
    
    
   
    //⑦キー入力待ち
    cv::waitKey(0);
    
    
    return 0;
}
