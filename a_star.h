#ifndef A_STAR_H
#define A_STAR_H
#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>
#include <Eigen/Core>
#include <fstream>
#include <memory>

struct Node
{
    int x;
    int y;

    int G;
    int H;
    int F;
    std::shared_ptr<Node> prev; // 路径的前一个格子

    bool visiable; // 可访问的
};

class A_Star
{
public:
    A_Star(const std::vector<std::vector<int>> &map);
    // ~A_Star()
    // {
    //     delete[] m_map;
    // }

    bool solve();                          // 解决方案
    void print_path();                     // 追溯路径，打印在控制台上
    void print_map(std::string path_file); // 直接打印寻找的地图

    std::vector<std::shared_ptr<Node>> m_map; // 地图用一维数组数组存储
    int m_map_height;      // 高和宽
    int m_map_weight;
    std::shared_ptr<Node> m_start; // 开始和结束格子
    std::shared_ptr<Node> m_end;
    bool allow_diagnoally = false; // 允许斜向寻迹
};

#endif // A_STAR_H