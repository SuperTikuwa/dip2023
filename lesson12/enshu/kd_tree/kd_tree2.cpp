// g++ -std=c++17 kd_tree2.cpp && ./a.out

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include <tuple>

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
    // 「暫定的な最近傍を探索」に関わるメソッド
    // ####################################################
    // # bool has_child_node(int next_point)              #
    // # int searchTentativeNearestNode(int i, bool axis) #
    // ####################################################

    // 子ノードが存在するか判定
    bool has_child_node(int next_point)
    {
        // next_pointがkd_treeのサイズより小さく、kd_tree[next_point]のx, y要素がINFでなければ子ノードが存在
        return next_point < kd_tree.size() && kd_tree[next_point].x != INF && kd_tree[next_point].y != INF;
    }

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

    // 2座標(p1, p2)のユークリッド距離を計算
    double calc_dist(Point p1, Point p2)
    {
        double x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    // 最近傍を探索
    pair<vector<Point>, double> searchNearestNeighbor(Point testdata)
    {
        test_data = testdata;
        // 暫定的な最近傍のインデックス番号を取得
        int tentativeNearestIndex = searchTentativeNearestNode(1, firstSplitAxis(point));
        std::vector<Point> tentativeNearestNode;
        tentativeNearestNode.push_back(kd_tree[tentativeNearestIndex]);
        // 暫定的な最近傍のノードと距離をtentativeNearestNeighborに格納
        pair<vector<Point>, double> tentativeNearestNeighbor = make_pair(tentativeNearestNode, calc_dist(kd_tree[tentativeNearestIndex], test_data));
        return tentativeNearestNeighbor;
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
        cout << "(" << kd_tree[i].x << ", " << kd_tree[i].y << ")" << endl;

        indent += branch.empty() ? "" : "   ";
        printTree(2 * i, indent, "├─");
        printTree(2 * i + 1, indent, "└─");
    }
};

int main()
{
    // 登録データ
    vector<Point> points = {{1, 1}, {2, 4}, {3, 3}, {4, 1}, {5, 4}, {6, 5}, {7, 1}};
    // テストデータ
    Point testdata = {6, 3};

    // kd-treeの構築
    KD_TREE kd(points);

    // kd-treeの出力
    cout << "=========================================" << endl;
    kd.printTree(1);
    cout << "=========================================" << endl;

    // テストデータとの最近傍を探索
    pair<vector<Point>, double> result = kd.searchNearestNeighbor(testdata);

    cout << "テストデータ　: "
            "("
         << testdata.x << ", " << testdata.y << ") " << endl;
    cout << "暫定的な最近傍: ";
    for (const Point &p : result.first)
    {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;
    cout << "距離　　　　　: " << result.second << endl;

    return 0;
}
