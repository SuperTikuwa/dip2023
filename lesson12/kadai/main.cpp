// g++ -std=c++17 kd_tree.cpp && ./a.out

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <tuple>
#include <fstream>
#include <sstream>

using namespace std;

struct Point
{
    double x, y;
    string classification;
};

class KD_TREE
{
private:
    vector<Point> kd_tree;
    vector<Point> point;
    Point test_data;
    double INF = numeric_limits<double>::max();

public:
    KD_TREE(vector<Point> p) : point(p)
    {
        create(p);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 「kd-treeの構築」に関わるメソッド
    // #####################################################
    // # void decisionTreeSize(vector<Point> p)            #
    // # bool firstSplitAxis(vector<Point> p)              #
    // # void buildTree(vector<Point> p, int i, bool axis) #
    // # void create(vector<Point> p)                      #
    // #####################################################

    // 座標の個数に応じて、kd-treeのサイズを決定
    void decisionTreeSize(vector<Point> p)
    {
        int n = 0;
        int size = 0;

        while (p.size() >= pow(2, n))
        {
            size += pow(2, n);
            n++;
        }

        // kd_treeを{INF, INF}のサイズが(size + 1)の配列に初期化
        kd_tree.resize(size + 1, {INF, INF});
    }

    // kd-treeの構築では分散が大きい座標から分割する
    // この関数では最初に分割する軸を決定
    bool firstSplitAxis(vector<Point> p)
    {
        double minX = INF, maxX = -INF;
        double minY = INF, maxY = -INF;

        for (auto [x, y, s] : p)
        {
            minX = min(minX, x);
            maxX = max(maxX, x);
            minY = min(minY, y);
            maxY = max(maxY, y);
        }

        // x要素の方が分散が大きいならtrue
        // y要素の方が分散が大きいならfalse
        return abs(minX - maxX) >= abs(minY - maxY);
    }

    // kd-treeの構築
    void buildTree(vector<Point> p, int i, bool axis)
    {
        // pの要素が空ならreturn
        if (p.empty())
            return;

        // axis==true  → x要素でソート
        // axis==false → y要素でソート
        sort(p.begin(), p.end(), [axis](Point a, Point b)
             { return axis ? a.x < b.x : a.y < b.y; });

        // kd_tree[i]にpの中央値を代入
        // -- Plase Write the code --
        kd_tree[i] = p[(p.size() - 1) / 2];

        // --------------------------

        // pの中央値より左側の要素をleft
        // pの中央値より右側の要素をright
        vector<Point> left(p.begin(), p.begin() + (p.size() - 1) / 2);
        vector<Point> right(p.begin() + (p.size() - 1) / 2 + 1, p.end());

        // left,rightの要素でそれぞれkd-treeを構築
        // -- Plase Write the code --

        buildTree(left, 2 * i, !axis);
        buildTree(right, 2 * i + 1, !axis);

        // --------------------------
    }

    // kd-treeの構築に必要な関数呼び出し
    void create(vector<Point> p)
    {
        // 初めにkd-treeのサイズを決定
        decisionTreeSize(p);
        // kd-treeの構築
        buildTree(p, 1, firstSplitAxis(p));
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 「暫定的な最近傍を探索」と「真の最近傍を探索(バックトラック)」に関わるメソッド
    // ########################################
    // # bool has_child_node(int next_point)  #
    // ########################################

    // 子ノードが存在するか判定
    bool has_child_node(int next_point)
    {
        // next_pointがkd_treeのサイズより小さく、kd_tree[next_point]のx, y要素がINFでなければ子ノードが存在
        return next_point < kd_tree.size() && kd_tree[next_point].x != INF && kd_tree[next_point].y != INF;
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 「暫定的な最近傍を探索」に関わるメソッド
    // #####################################################
    // # int searchTentativeNearestNode(int i, bool axis)  #
    // #####################################################

    // 暫定的な最近傍を探索
    int searchTentativeNearestNode(int i, bool axis)
    {
        // (kx, ky) = (kd-tree[i]のx, kd-tree[i]のy)
        // (tx, ty) = (テストデータのx, テストデータのy)
        double kx = kd_tree[i].x, ky = kd_tree[i].y;
        double tx = test_data.x, ty = test_data.y;
        int next_point = i * 2;

        if (axis)
        {
            // x要素で分割された場合、テストデータのxとkd-treeのxを比較
            //      →   (tx <= kx)なら左型の子ノード, (tx > kx)なら右型の子ノードを探索
            // -- Plase Write the code --

            if (tx > kx)
            {
                next_point++;
            }

            // --------------------------
        }
        else
        {
            // y要素で分割された場合、テストデータのyとkd-treeのyを比較
            //      →   (ty <= ky)なら左型の子ノード, (ty > ky)なら右型の子ノードを探索
            // -- Plase Write the code --
            if (ty > ky)
            {
                next_point++;
            }
            // --------------------------
        }

        // 次の探査するノードが存在するか判定
        if (has_child_node(next_point))
        {
            // 存在するならkd_tree[i]の子ノードであるkd_tree[next_point]を探索
            // -- Plase Write the code --

            return searchTentativeNearestNode(next_point, !axis);

            // --------------------------
        }
        else
        {
            // 存在しないなら現在探索しているノードのインデックス番号を返す
            // -- Plase Write the code --

            return i;

            // --------------------------
        }
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++v
    // 「真の最近傍を探索(バックトラック)」に関わるメソッド
    // #########################################################################################################################################
    // # double calc_squared_dist(Point p1, Point p2)                                                                                          #
    // # double searchMinDist(int i)                                                                                                           #
    // # vector<Point> searchTargetDistNode(int i, double targetDist, vector<Point> nearestPoint)                                              #
    // # pair<double, vector<Point>> updateNearestNeighbor(double dist, vector<Point> points, double nearestDist, vector<Point> nearestPoints) #
    // # pair<vector<Point>, double> backTrack(int i, int previous_node, double nearestDist, vector<Point> nearestPoints)                      #
    // #########################################################################################################################################

    // 2座標(p1, p2)のユークリッド距離を計算
    // ※小数点誤差を考慮するため、√(ルート)は付けない
    double calc_squared_dist(Point p1, Point p2)
    {
        double x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        return pow(x1 - x2, 2) + pow(y1 - y2, 2);
    }

    // kd_tree[i]を根とした時の木からテストデータとの距離が最小のものを探索
    double searchMinDist(int i)
    {
        double dist = calc_squared_dist(kd_tree[i], test_data);
        // 現在のテストデータとの最小の距離
        double minDist = dist;

        // kd_tree[i]の子ノード(x==0の時は左側の子ノード, x==1の時は右の子ノード)を探索
        for (int x = 0; x < 2; x++)
        {
            int next_point = i * 2 + x;
            if (has_child_node(next_point))
            {
                double childMinDist = searchMinDist(next_point);
                // 最小の距離を更新
                if (minDist > childMinDist)
                    minDist = childMinDist;
            }
        }

        return minDist;
    }

    // kd_tree[i]を根とした時の木からテストデータとの距離がtargetDistのノードを探索
    vector<Point> searchTargetDistNode(int i, double targetDist, vector<Point> nearestPoint)
    {
        // kd_tree[i]とテストデータの距離がtargetDistならnearestPointに追加
        if (calc_squared_dist(kd_tree[i], test_data) == targetDist)
        {
            nearestPoint.push_back(kd_tree[i]);
        }

        // kd_tree[i]の子要素(x==0の時は左側の子要素, x==1の時は右の子要素)を探索
        for (int x = 0; x < 2; x++)
        {
            int next_point = i * 2 + x;
            if (has_child_node(next_point))
            {
                // kd_tree[i]の子ノードからテストデータとの距離がtargetDistのノードを取得
                vector<Point> temp = searchTargetDistNode(next_point, targetDist, nearestPoint);
                // 上記で取得したノードをnearestPointに追加
                nearestPoint.insert(nearestPoint.end(), temp.begin(), temp.end());
            }
        }

        return nearestPoint;
    }

    // distとpointsを用いて、nearestDistとnearestpointsを更新
    pair<double, vector<Point>> updateNearestNeighbor(double dist, vector<Point> points, double nearestDist, vector<Point> nearestPoints)
    {
        // テストデータとの最小距離が更新された時、nearestPointsをクリア
        if (nearestDist > dist)
        {
            nearestPoints.clear();
        }

        // テストデータとの最小距離(nearestDist)と最小距離のノード(nearestPoints)を更新
        if (nearestDist >= dist)
        {
            nearestDist = dist;
            nearestPoints.insert(nearestPoints.end(), points.begin(), points.end());
        }

        return make_pair(nearestDist, nearestPoints);
    }

    // kd_tree[childIndex]を根とする木の中で、テストデータと最小の距離とそのノードを取得
    pair<double, vector<Point>> searchChildInfo(int childIndex)
    {
        // 初期値としてdistには上限値を格納
        double dist = INF;
        vector<Point> point;

        if (has_child_node(childIndex))
        {
            dist = searchMinDist(childIndex);
            point = searchTargetDistNode(childIndex, dist, {});
        }

        return make_pair(dist, point);
    }

    // バックトラック処理
    // backTrack(i: 注目するkd-treeのindex, previous_node: 前に探索していたkd-treeのindex, nearestDist: 現時点でのテストデータとの最短距離, nearestPoints: 現時点でのテストデータとの最短距離となるkd-treeのノード)
    pair<vector<Point>, double> backTrack(int i, int previous_node, double nearestDist, vector<Point> nearestPoints)
    {
        // テストデータとの最小の距離(childInfo.first)とそのノード(childInfo.second)
        pair<double, vector<Point>> childInfo;
        if (previous_node == -1)
        {
            childInfo.first = INF;
            childInfo.second = {};
        }
        else
        {
            // i * 2 + (1 - previous_node % 2)はkd_tree[i]の未探索の子ノードのインデックスを示す
            int childIndex = i * 2 + (1 - previous_node % 2);
            // kd_tree[childIndex]を根とする木の情報を取得
            childInfo = searchChildInfo(childIndex);
        }
        // tie: 複数変数への代入を一行で行う
        // 最近傍とそのノードを更新
        tie(nearestDist, nearestPoints) = updateNearestNeighbor(childInfo.first, childInfo.second, nearestDist, nearestPoints);

        // kd_treeの根まで到達した場合、探索は終了
        if (i == 1)
        {
            return make_pair(nearestPoints, sqrt(nearestDist));
        }

        // kd_tree[i]の親ノードとテストデータの距離をdistに格納
        double dist = calc_squared_dist(kd_tree[i / 2], test_data);
        if (nearestDist >= dist)
        {
            // 現在の最小距離よりdistが小さければ、最近傍とそのノードを更新
            tie(nearestDist, nearestPoints) = updateNearestNeighbor(dist, {kd_tree[i / 2]}, nearestDist, nearestPoints);
            // kd_tree[i]の親ノードを探索
            // -- Plase Write the code --

            return backTrack(i / 2, i, nearestDist, nearestPoints);

            // --------------------------
        }
        else
        {
            // 現在の最小距離よりdistが大きければ、親ノードに最近傍は存在しないため探索終了
            // -- Plase Write the code --

            return make_pair(nearestPoints, sqrt(nearestDist));
            // --------------------------
        }
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // 最近傍を探索
    pair<vector<Point>, double> searchNearestNeighbor(Point td)
    {
        test_data = td;
        // 暫定的な最近傍のインデックス番号を取得
        int tentativeNearestIndex = searchTentativeNearestNode(1, firstSplitAxis(point));
        // 暫定的な最近傍からバックトラックすることで、真の最近傍を探索
        pair<vector<Point>, double> nearestNeighbor = backTrack(tentativeNearestIndex, -1, calc_squared_dist(kd_tree[tentativeNearestIndex], test_data), {kd_tree[tentativeNearestIndex]});
        return nearestNeighbor;
    }

    // kd-treeの出力
    void printTree(int i, string indent = "", string branch = "")
    {
        if (i >= kd_tree.size() || (kd_tree[i].x == 0 && kd_tree[i].y == 0))
        {
            return;
        }

        cout << indent;
        if (!branch.empty())
        {
            cout << " └─" << branch;
        }
        cout << "(" << kd_tree[i].x << ", " << kd_tree[i].y << ", " << kd_tree[i].classification << ")" << endl;

        indent += branch.empty() ? "" : "   ";
        printTree(2 * i, indent, "├─");
        printTree(2 * i + 1, indent, "└─");
    }
};

// CSVファイルの読み込み
std::vector<Point> readCSV(const std::string &filename)
{
    std::vector<Point> data;
    std::ifstream file(filename);

    // Skip the header
    std::string line;
    std::getline(file, line);

    // Read data line by line
    while (std::getline(file, line))
    {
        std::istringstream ss(line);

        // Extract each field
        std::string field;
        std::getline(ss, field, ','); // Ignore the index
        std::getline(ss, field, ',');
        double x = std::stod(field);
        std::getline(ss, field, ',');
        double y = std::stod(field);
        std::getline(ss, field, ',');
        std::string classification = field;

        // Store the data in a Point structure
        Point p = {x, y, classification};
        data.push_back(p);
    }

    return data;
}

int main()
{
    // 登録データ
    std::vector<Point> points = readCSV("height_weight.csv");
    // テストデータ
    // Point testdata = {身長(cm), 体重(kg)};
    Point testdata = {174.0, 54.0};

    // kd-treeの構築
    KD_TREE kd(points);

    // kd-treeの出力
    // cout << "=========================================" << endl;
    // kd.printTree(1);
    // cout << "=========================================" << endl;

    // テストデータとの最近傍を探索
    pair<vector<Point>, double> result = kd.searchNearestNeighbor(testdata);

    cout << "テストデータ: "
            "("
         << testdata.x << ", " << testdata.y << ") " << endl;
    cout << "最近傍　　　: ";
    for (const Point &p : result.first)
    {
        cout << "(" << p.x << ", " << p.y << ", " << p.classification << ") ";
    }
    cout << endl;
    cout << "距離　　　　: " << result.second << endl;

    return 0;
}
