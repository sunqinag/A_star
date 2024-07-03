#include <opencv2/opencv.hpp>
#include <iostream>

#include "a_star.h"
#include <Eigen/Dense>
#include <filesystem>

namespace fs = std::filesystem;

/**
 *
#   0: obstacle
#   205: background
#   255: OK area

start point (232, 120)
end point (1079, 839)
 */

std::vector<std::vector<int>> Mat2Vector(const cv::Mat &mat)
{
    std::vector<std::vector<int>> result;
    if (!mat.empty())
    {
        result.resize(mat.rows);
        for (int i = 0; i < mat.rows; ++i)
        {
            result[i].resize(mat.cols);
            for (int j = 0; j < mat.cols; ++j)
            {
                // 假设mat是CV_8U类型，直接转换为int（可能会有截断）
                // 如果mat是其他类型，你可能需要相应地转换
                result[i][j] = static_cast<int>(mat.at<uchar>(i, j));
            }
        }
    }
    return result;
}

cv::Mat Vector2Mat(const std::vector<std::vector<int>> &vec)
{
    if (vec.empty())
    {
        // 处理空向量的情况
        return cv::Mat();
    }

    // 假设所有内部向量的大小都相同
    int rows = vec.size();
    if (rows == 0)
    {
        // 没有行，返回空矩阵
        return cv::Mat();
    }
    int cols = vec[0].size();
    if (cols == 0)
    {
        // 没有列，返回空矩阵
        return cv::Mat();
    }

    // 创建一个大小合适的矩阵，类型为CV_32S（32位整数）
    cv::Mat mat(rows, cols, CV_32S);

    // 填充矩阵
    for (int i = 0; i < rows; ++i)
    {
        if (vec[i].size() != cols)
        {
            // 如果发现行的大小不一致，抛出异常或处理错误
            throw std::runtime_error("Inner vectors do not have the same size!");
        }
        for (int j = 0; j < cols; ++j)
        {
            mat.at<int>(i, j) = vec[i][j];
        }
    }

    return mat;
}

std::vector<std::vector<int>> set_new_node(std::vector<std::vector<int>> &map2d, Eigen::Vector2d &start_node, Eigen::Vector2d &end_node)
{
    map2d[start_node.y()][start_node.x()] = 1;
    map2d[end_node.y()][end_node.x()] = 2;
    return map2d;
}

std::vector<std::vector<int>> reset_node(std::vector<std::vector<int>> &map2d, Eigen::Vector2d &old_start_node, Eigen::Vector2d &old_end_node)
{
    // reset
    map2d[old_start_node.y()][old_start_node.x()] = 0;
    map2d[old_end_node.y()][old_end_node.x()] = 0;
    return map2d;
}

cv::Mat expanse_map(cv::Mat &map)
{
    cv::Mat expanse_map = map.clone();
    for (size_t i = 0; i < map.rows; i++)
    {
        for (size_t j = 0; j < map.cols; j++)
        {
            if (map.at<uchar>(i, j) == 0)
            {
                int expanse_pixel = 5; //膨胀10个像素，效果极为显著
                cv::circle(expanse_map,cv::Point(j,i),expanse_pixel,cv::Scalar(0)); 
            }
        }
    }

    // // 显示结果
    cv::imwrite("../debug/Original Image.png", map);
    cv::imwrite("../debug/Sharpened Image.png", expanse_map);
    // exit(0);
    return expanse_map;
}

int main()
{
    Eigen::MatrixXd nodes(25, 2);
    nodes << 
        // 456, 226,
        1124, 1353,
        1182, 1636,
        1152, 2193,
        1118, 2543,
        1100, 2743,
        1144, 2340,
        1212, 2473,
        1320, 2776,
        1178, 2736,
        1256, 2773,
        1336, 2496,
        1298, 2210,
        1390, 1866,
        1270, 2590,
        1370, 2186,
        1226, 2153,
        1228, 1883,
        1166, 1903,
        1048, 2370,
        1314, 1880,
        1322, 1676,
        1270, 1440,
        1114, 1640,
        1416, 1650,
        1124, 1353;
        

    std::string path_file = "../debug/path.txt";
    fs::path pf(path_file);
    if (fs::exists(pf))
    {
        fs::remove(pf);
        std::cout << "删除： " << pf.c_str() << std::endl;
    }

    std::cout << nodes << std::endl;
    std::string file = "/home/sunqiang/data/zz520/jueying.pgm";
    cv::Mat map = cv::imread(file, cv::IMREAD_GRAYSCALE); // 读取为灰度图像
    cv::threshold(map, map, 240, 255, cv::THRESH_BINARY);
    std::cout << "cols: " << map.cols << " rows: " << map.rows << std::endl;

    map = expanse_map(map);

    std::vector<std::vector<int>> map2d = Mat2Vector(map);

    Eigen::Vector2d start_node = nodes.row(0);
    for (size_t i = 1; i < nodes.rows(); i++)
    {
        Eigen::Vector2d end_node = nodes.row(i);
        map2d = set_new_node(map2d, start_node, end_node);
        std::unique_ptr<A_Star> problem(new A_Star(map2d));

        if (problem->solve())
        {

            problem->print_map(path_file);
            std::cout<<"start point: "<<start_node.transpose()<<" end point: "<<end_node.transpose()<<std::endl;
        }
        else
        {
            std::cout << "solve false, start node: " << start_node.transpose() << ",  end node: " << end_node.transpose() << std::endl;
            exit(0);
        }

        map2d = reset_node(map2d, start_node, end_node);
        start_node = end_node;
    }

    return 1;
}