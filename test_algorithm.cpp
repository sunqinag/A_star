#include <opencv2/opencv.hpp>
#include <iostream>

#include "a_star.h"
/**
 * 
#   0: obstacle
#   205: background
#   255: OK area

start point (232, 120)
end point (1079, 839)
 */


std::vector<std::vector<int>> Mat2Vector(const cv::Mat& mat) {  
    std::vector<std::vector<int>> result;  
    if (!mat.empty()) {  
        result.resize(mat.rows);  
        for (int i = 0; i < mat.rows; ++i) {  
            result[i].resize(mat.cols);  
            for (int j = 0; j < mat.cols; ++j) {  
                // 假设mat是CV_8U类型，直接转换为int（可能会有截断）  
                // 如果mat是其他类型，你可能需要相应地转换  
                result[i][j] = static_cast<int>(mat.at<uchar>(i, j));  
            }  
        }  
    }  
    return result;  
}


cv::Mat Vector2Mat(const std::vector<std::vector<int>>& vec) {  
    if (vec.empty()) {  
        // 处理空向量的情况  
        return cv::Mat();  
    }  
  
    // 假设所有内部向量的大小都相同  
    int rows = vec.size();  
    if (rows == 0) {  
        // 没有行，返回空矩阵  
        return cv::Mat();  
    }  
    int cols = vec[0].size();  
    if (cols == 0) {  
        // 没有列，返回空矩阵  
        return cv::Mat();  
    }  
  
    // 创建一个大小合适的矩阵，类型为CV_32S（32位整数）  
    cv::Mat mat(rows, cols, CV_32S);  
  
    // 填充矩阵  
    for (int i = 0; i < rows; ++i) {  
        if (vec[i].size() != cols) {  
            // 如果发现行的大小不一致，抛出异常或处理错误  
            throw std::runtime_error("Inner vectors do not have the same size!");  
        }  
        for (int j = 0; j < cols; ++j) {  
            mat.at<int>(i, j) = vec[i][j];  
        }  
    }  
  
    return mat;  
}  

int main()
{
    Eigen::Vector2d start_point = {232, 120};
    Eigen::Vector2d  end_point = {1079, 839};

    std::string file = "/home/sunqiang/data/zz520/jueying.pgm";
    cv::Mat map = cv::imread(file, cv::IMREAD_GRAYSCALE); // 读取为灰度图像
    std::cout<<"cols: "<<map.cols<<" rows: "<<map.rows<<std::endl;

    std::vector<std::vector<int>>  map2d = Mat2Vector(map);

    std::cout<<"start: "<<map2d[start_point.y()][start_point.x()]<<std::endl;
    std::cout<<"end: "<<map2d[end_point.y()][end_point.x()]<<std::endl;


    // //set start 
    map2d[start_point.y()][start_point.x()] = 1;
    

    // //set end
    map2d[end_point.y()][end_point.x()] = 2;


    A_Star problem(map2d);

    if (problem.solve())
    {
        problem.print_map(map2d);
    }else{
        std::cout<<"solve false\n";
    }
    
    return 1;
}