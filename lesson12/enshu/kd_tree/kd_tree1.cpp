// g++ -std=c++17 kd_tree1.cpp && ./a.out

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

    return 0;
}
