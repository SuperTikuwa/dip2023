// g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4` -std=c++17 && ./a.out

#include <opencv2/opencv.hpp>
#include <iostream>

void drawAxis(cv::Mat &img, cv::Point p, cv::Point q, cv::Scalar colour, const float scale = 0.2)
{
    double angle = atan2((double)p.y - q.y, (double)p.x - q.x); // angle in radians
    double hypotenuse = sqrt((double)(p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));
    // Here we lengthen the arrow by a factor of scale
    q.x = (int)(p.x - scale * hypotenuse * cos(angle));
    q.y = (int)(p.y - scale * hypotenuse * sin(angle));
    cv::line(img, p, q, colour, 1, cv::LINE_AA);
    // create the arrow hooks
    p.x = (int)(q.x + 9 * cos(angle + CV_PI / 4));
    p.y = (int)(q.y + 9 * sin(angle + CV_PI / 4));
    cv::line(img, p, q, colour, 1, cv::LINE_AA);
    p.x = (int)(q.x + 9 * cos(angle - CV_PI / 4));
    p.y = (int)(q.y + 9 * sin(angle - CV_PI / 4));
    cv::line(img, p, q, colour, 1, cv::LINE_AA);
}

double getOrientation(const std::vector<cv::Point> &pts, cv::Mat &img)
{
    // 主成分分析のために、対象となる輪郭のみを別のバッファにコピーします。
    cv::Mat data_pts = cv::Mat(pts.size(), 2, CV_64F); // [pts.size() x 2] 行列
    for (int i = 0; i < data_pts.rows; i++)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }
    // 主成分分析の実行
    // 穴埋め
    cv::PCA pcaResult(data_pts, cv::Mat(), cv::PCA::DATA_AS_ROW);

    // [分析結果] 中心の座標
    cv::Point cntr;

    // 穴埋め

    // for (int i = 0; i < pcaResult.mean.rows; i++)
    // {
    //     cntr = cv::Point(pcaResult.mean.at<double>(i, 0), pcaResult.mean.at<double>(i, 1));
    //     std::cout << "中心 x:" << cntr.x << " y:" << cntr.y << std::endl;
    // }
    cntr = cv::Point(pcaResult.mean.at<double>(0, 0), pcaResult.mean.at<double>(0, 1));
    std::cout << "中心 x:" << cntr.x << " y:" << cntr.y << std::endl;

    // [分析結果] 固有値と固有ベクトル
    std::vector<cv::Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);

    // 穴埋め
    for (int i = 0; i < 2; i++)
    {
        eigen_vecs[i] = cv::Point2d(pcaResult.eigenvectors.at<double>(i, 0), pcaResult.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pcaResult.eigenvalues.at<double>(i, 0);
        std::cout << "第" << i << "主成分ベクトル x:" << eigen_vecs[i].x << " y:" << eigen_vecs[i].y << " 固有値:" << eigen_val[i] << std::endl;
    }

    // 分析結果を可視化のために描画します。
    cv::circle(img, cntr, 3, cv::Scalar(255, 0, 255), 2);
    cv::Point p1 = cntr + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]);
    cv::Point p2 = cntr - 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]);
    drawAxis(img, cntr, p1, cv::Scalar(0, 255, 0), 1);
    drawAxis(img, cntr, p2, cv::Scalar(255, 255, 0), 5);

    // 物体の方向をラジアンで返します。
    double angle = atan2(eigen_vecs[0].y, eigen_vecs[0].x);
    return angle;
}

int main()
{
    cv::Mat src = cv::imread("./image/test-image.jpg", -1);
    if (src.empty())
    {
        return -1;
    }

    // グレースケール画像に変換
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // 50 を閾値として黒 0 か白 255 に二値化します。
    cv::Mat bw;
    cv::threshold(gray, bw, 50, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("gray", gray);
    cv::imshow("bw", bw);

    // 輪郭を検出します。
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(bw, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    for (size_t i = 0; i < contours.size(); i++)
    {

        // 面積が小さすぎる場合と多きすぎる場合を除外します。
        double area = cv::contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area)
        {
            continue;
        }
        // 輪郭を描画します。
        cv::drawContours(src, contours, i, cv::Scalar(0, 0, 255), 2);
        // 各輪郭について、それぞれ方向を検出します。
        getOrientation(contours[i], src);
    }

    cv::imshow("output", src);
    cv::waitKey(0);
    return 0;
}