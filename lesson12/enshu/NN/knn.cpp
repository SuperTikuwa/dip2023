// g++ knn.cpp -std=c++11 `pkg-config --cflags --libs opencv4` && ./a.out

#include <opencv2/opencv.hpp>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace cv;
using namespace std;

struct FruitPoint
{
    double x;
    double y;
    string classification;
};

vector<FruitPoint> readFruitCSV(const string &filename)
{
    ifstream file(filename);
    vector<FruitPoint> data;

    string line;
    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);
        string name;
        string corners_str;
        string hue_str;

        getline(ss, name, ',');
        getline(ss, corners_str, ',');
        getline(ss, hue_str, ',');

        double corners = stod(corners_str);
        double hue = stod(hue_str);

        data.push_back(FruitPoint{corners, hue, name});
    }

    return data;
}

void plotFruitData(const vector<FruitPoint> &points, const FruitPoint &testdata, const vector<FruitPoint> &nearestPoints)
{
    vector<FruitPoint> allPoints = points;
    allPoints.push_back(testdata);

    double x_min = min_element(allPoints.begin(), allPoints.end(), [](const FruitPoint &a, const FruitPoint &b)
                               { return a.x < b.x; })
                       ->x;
    double x_max = max_element(allPoints.begin(), allPoints.end(), [](const FruitPoint &a, const FruitPoint &b)
                               { return a.x < b.x; })
                       ->x;
    double y_min = min_element(allPoints.begin(), allPoints.end(), [](const FruitPoint &a, const FruitPoint &b)
                               { return a.y < b.y; })
                       ->y;
    double y_max = max_element(allPoints.begin(), allPoints.end(), [](const FruitPoint &a, const FruitPoint &b)
                               { return a.y < b.y; })
                       ->y;

    double x_range = x_max - x_min;
    double y_range = y_max - y_min;

    x_min -= x_range * 0.1;
    x_max += x_range * 0.1;
    y_min -= y_range * 0.1;
    y_max += y_range * 0.1;

    Mat img = Mat::zeros(800, 800, CV_8UC3);
    img.setTo(Scalar(255, 255, 255));

    map<string, Scalar> colors;
    colors["Apple"] = Scalar(0, 0, 255);
    colors["Banana"] = Scalar(0, 255, 255);
    colors["Grape"] = Scalar(128, 0, 128);

    int x_testdata = static_cast<int>((testdata.x - x_min) / (x_max - x_min) * 800);
    int y_testdata = 800 - static_cast<int>((testdata.y - y_min) / (y_max - y_min) * 800);

    auto comp = [](const FruitPoint &a, const FruitPoint &b)
    { return a.x == b.x && a.y == b.y && a.classification == b.classification; };
    for (const auto &point : points)
    {
        int x = static_cast<int>((point.x - x_min) / (x_max - x_min) * 800);
        int y = 800 - static_cast<int>((point.y - y_min) / (y_max - y_min) * 800);

        circle(img, cv::Point(x, y), 5, colors[point.classification], -1);

        if (find_if(nearestPoints.begin(), nearestPoints.end(), [&point, comp](const FruitPoint &p)
                    { return comp(p, point); }) != nearestPoints.end())
        {
            line(img, cv::Point(x_testdata, y_testdata), cv::Point(x, y), Scalar(0, 255, 0));
        }
    }

    circle(img, cv::Point(x_testdata, y_testdata), 10, Scalar(0, 165, 255), -1);

    imshow("Plot", img);
    waitKey(0);
}

// 最も頻度が高いclassificationを見つける関数
string getMostCommonClassification(const vector<FruitPoint> &points)
{
    // 各classificationの頻度を保存するマップ
    map<string, int> freqs;

    // 全ての点に対して
    for (const auto &p : points)
    {
        // classificationの頻度をインクリメントする
        freqs[p.classification]++;
    }

    // 最も頻度が高いclassificationとその頻度を保存する
    string mostCommonClassification;
    int maxFreq = -1;

    // 全てのマップ要素に対して
    for (const auto &pair : freqs)
    {
        // 現在のclassificationの頻度が最大頻度より高い場合
        if (pair.second > maxFreq)
        {
            // そのclassificationと頻度を保存する
            mostCommonClassification = pair.first;
            maxFreq = pair.second;
        }
    }

    // 最も頻度が高いclassificationを返す
    return mostCommonClassification;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 「kNN法」に関わるメソッド
// ############################################################################################################
// # double distance(const FruitPoint& a, const FruitPoint& b)                                                #
// # vector<FruitPoint> getNearestPoints(const vector<FruitPoint>& points, const FruitPoint& testdata, int k) #
// ############################################################################################################

// 計算する2つのFruitPointオブジェクト間のユークリッド距離を返す
double fruitPoint_distance(const FruitPoint &a, const FruitPoint &b)
{
    // a: 1つ目のFruitPointオブジェクト
    // b: 2つ目のFruitPointオブジェクト
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

// テストデータから最も近いk個のFruitPointオブジェクトを返す
vector<FruitPoint> getNearestPoints(const vector<FruitPoint> &points, const FruitPoint &testdata, int k)
{
    vector<pair<double, FruitPoint>> dists;
    for (const auto &point : points)
    {
        double dist = fruitPoint_distance(point, testdata);
        dists.push_back(make_pair(dist, point));
    }

    // compは比較関数であり、sort関数でdistsのfirst要素(距離)が小さい順にソートされるために用いられる
    auto comp = [](const pair<double, FruitPoint> &a, const pair<double, FruitPoint> &b)
    { return a.first < b.first; };
    sort(dists.begin(), dists.end(), comp);

    vector<FruitPoint> nearestPoints;
    // nearestPointsに近傍k個の登録データを追加
    // -- Plase Write the code --
    for (int i = 0; i < k && i < dists.size(); i++)
    {
        nearestPoints.push_back(dists[i].second);
    }

    // --------------------------

    return nearestPoints;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main()
{
    // 登録データ
    vector<FruitPoint> points = readFruitCSV("fruits.csv");
    // テストデータ
    // FruitPoint testdata = {corners, hue, "test"}
    FruitPoint testdata = {62, 21.6238, "test"};
    // 近傍k個の登録データを取得
    int k = 3;

    if (testdata.classification.empty())
    {
        exit(0);
    }

    vector<FruitPoint> nearestPoints = getNearestPoints(points, testdata, k);

    // 近傍k個の登録データを出力
    for (size_t i = 0; i < nearestPoints.size(); ++i)
    {
        const auto &p = nearestPoints[i];
        cout << i + 1 << ": (" << p.x << ", " << p.y << ", " << p.classification << ") " << endl;
    }

    cout << "最も多く識別されるクラス: " << getMostCommonClassification(nearestPoints) << endl;

    plotFruitData(points, testdata, nearestPoints);

    return 0;
}
