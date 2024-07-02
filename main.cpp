
#include <iostream>
#include <opencv2/opencv.hpp>
#include "a_star.h"

//以这种方式
int main() {
//地图可视化参照上方图片
//O代表可行路径
//X代表障碍物
//S代表起点
//E代表终点
    std::vector<std::vector<char>> map = {
        {'O', 'O','O','O','O','O','O','O'},
        {'O', 'O','O','O','X','O','O','O'},
        {'O', 'O','S','O','X','O','E','O'},
        {'O', 'O','O','O','X','O','O','O'},
        {'O', 'O','O','O','O','O','O','O'},
        {'O', 'O','O','O','O','O','O','O'}
    };

    // A_Star problem(map);

    // if (problem.solve()) {
    //     problem.print_map();
    // }
}