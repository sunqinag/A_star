#ifndef A_STAR_H
#define A_STAR_H
#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>
#include <Eigen/Core>

struct Node {
    int x;
    int y;

    int G;
    int H;
    int F;
    Node* prev = nullptr;//路径的前一个格子

    bool visiable;//可访问的
};


class A_Star {
public:
    A_Star(const std::vector<std::vector<int>>& map);
    bool solve();//解决方案
    void print_path();//追溯路径，打印在控制台上
    void print_map(std::vector<std::vector<int>> &map);//直接打印寻找的地图

    Node* m_map = nullptr;//地图用一维数组数组存储
    int m_map_height;//高和宽
    int m_map_weight;
    Node* m_start = nullptr;//开始和结束格子
    Node* m_end = nullptr;
};

#endif // A_STAR_H