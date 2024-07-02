#include "a_star.h"
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <algorithm>

A_Star::A_Star(const std::vector<std::vector<int>> &map)
{
    m_map_height = map.size();
    m_map_weight = map[0].size();

    if (m_map_height == 0 || m_map_weight == 0)
    {
        throw std::logic_error("map format error");
    }

    m_map = new Node[m_map_height * m_map_weight];
    for (int i = 0; i < m_map_height; ++i)
    {
        for (int j = 0; j < m_map_weight; ++j)
        {
            Node &curr_node = m_map[i * m_map_weight + j];

            curr_node.x = j;
            curr_node.y = i;

            curr_node.G = std::numeric_limits<int>::max(); // 先全部设置为无限大
            curr_node.F = std::numeric_limits<int>::max();
            switch (map[i][j])
            {
            case 254:
                curr_node.visiable = true;
                break;
            case 205:
                curr_node.visiable = false;
                break;
            case 0:
                curr_node.visiable = false;
                break;
            case 1:
                curr_node.visiable = true;
                m_start = &curr_node;
                break;
            case 2:
                curr_node.visiable = true;
                m_end = &curr_node;
                break;
            default:
                throw std::logic_error("illegal identifier exist");
            }
        }
    }

    if (m_start == nullptr)
    {
        throw std::logic_error("start point is not exist");
    }
    if (m_end == nullptr)
    {
        throw std::logic_error("end point is not exist");
    }

    for (int i = 0; i < m_map_height; ++i)
    {
        for (int j = 0; j < m_map_weight; ++j)
        {
            Node &curr_node = m_map[i * m_map_weight + j];
            curr_node.H = (std::abs(curr_node.x - m_end->x) + std::abs(curr_node.y - m_end->y)) * 10; // 计算曼哈顿距离
        }
    }
}

bool A_Star::solve()
{

    std::vector<Node *> openlist;
    std::vector<Node *> closelist;

    openlist.push_back(m_start); // 开始节点加入openlist
    m_start->G = 0;              // 开始节点的G置零并计算F
    m_start->F = m_start->H;

    while (openlist.size() != 0)
    { // 如果openlist不为空

        Node *curr_node = nullptr; // 找到F值最小的节点作为当前处理节点
        for (std::vector<Node *>::iterator node = openlist.begin(); node != openlist.end(); node++)
        {
            if (curr_node == nullptr)
            {
                curr_node = *node;
                continue;
            }
            else if ((*node)->F <= curr_node->F)
            {
                curr_node = *node;
            }
        }

        if (curr_node == m_end)
        { // 如果这个节点刚好是终点，直接返回true退出
            return true;
        }

        bool has_prev_x = curr_node->x - 1 >= 0;
        bool has_next_x = curr_node->x + 1 < m_map_weight;
        bool has_prev_y = curr_node->y - 1 >= 0;
        bool has_next_y = curr_node->y + 1 < m_map_height;

        if (has_prev_x)
        { // 存在左侧
            Node *left = &m_map[curr_node->y * m_map_weight + curr_node->x - 1];
            if (left->visiable && std::find(closelist.begin(), closelist.end(), left) == closelist.end())
            { // 如果此路径可用并且不在closelist中
                if (curr_node->G + 10 < left->G)
                { // 自身G走10到达left节点比left原来的G小，则给left_up换条路径，并重新计算G和F
                    left->prev = curr_node;
                    left->G = curr_node->G + 10;
                    left->F = left->G + left->H;
                    if (std::find(openlist.begin(), openlist.end(), left) == openlist.end())
                    { // 如果不在openlist中，加入openlist
                        openlist.push_back(left);
                    }
                }
            }
            if (has_prev_y)
            { // 存在左上
                Node *left_up = &m_map[(curr_node->y - 1) * m_map_weight + curr_node->x - 1];
                if (left_up->visiable && std::find(closelist.begin(), closelist.end(), left_up) == closelist.end())
                {
                    if (curr_node->G + 14 < left_up->G)
                    { // 自身G走14到达left_up节点比left_up原来的G小，则换条路径
                        left_up->prev = curr_node;
                        left_up->G = curr_node->G + 14;
                        left_up->F = left_up->G + left_up->H;
                        if (std::find(openlist.begin(), openlist.end(), left_up) == openlist.end())
                        { // 如果左侧节点不再openlist中，则添加进去
                            openlist.push_back(left_up);
                        }
                    }
                }
            }
            if (has_next_y)
            { // 存在左下
                Node *left_down = &m_map[(curr_node->y + 1) * m_map_weight + curr_node->x - 1];
                if (left_down->visiable && std::find(closelist.begin(), closelist.end(), left_down) == closelist.end())
                {
                    if (curr_node->G + 14 < left_down->G)
                    { // 自身G走14到达left_down节点比left_down原来的G小，则换条路径
                        left_down->prev = curr_node;
                        left_down->G = curr_node->G + 14;
                        left_down->F = left_down->G + left_down->H;
                        if (std::find(openlist.begin(), openlist.end(), left_down) == openlist.end())
                        {
                            openlist.push_back(left_down);
                        }
                    }
                }
            }
        }
        if (has_next_x)
        { // 存在右侧
            Node *right = &m_map[curr_node->y * m_map_weight + curr_node->x + 1];
            if (right->visiable && std::find(closelist.begin(), closelist.end(), right) == closelist.end())
            {
                if (curr_node->G + 10 < right->G)
                {
                    right->prev = curr_node;
                    right->G = curr_node->G + 10;
                    right->F = right->G + right->H;
                    if (std::find(openlist.begin(), openlist.end(), right) == openlist.end())
                    {
                        openlist.push_back(right);
                    }
                }
            }
            if (has_prev_y)
            { // 存在右上
                Node *right_up = &m_map[(curr_node->y - 1) * m_map_weight + curr_node->x + 1];
                if (right_up->visiable && std::find(closelist.begin(), closelist.end(), right_up) == closelist.end())
                {
                    if (curr_node->G + 14 < right_up->G)
                    {
                        right_up->prev = curr_node;
                        right_up->G = curr_node->G + 14;
                        right_up->F = right_up->G + right_up->H;
                        if (std::find(openlist.begin(), openlist.end(), right_up) == openlist.end())
                        {
                            openlist.push_back(right_up);
                        }
                    }
                }
            }
            if (has_next_y)
            { // 存在右下
                Node *right_down = &m_map[(curr_node->y + 1) * m_map_weight + curr_node->x + 1];
                if (right_down->visiable && std::find(closelist.begin(), closelist.end(), right_down) == closelist.end())
                {
                    if (curr_node->G + 14 < right_down->G)
                    {
                        right_down->prev = curr_node;
                        right_down->G = curr_node->G + 14;
                        right_down->F = right_down->G + right_down->H;
                        if (std::find(openlist.begin(), openlist.end(), right_down) == openlist.end())
                        {
                            openlist.push_back(right_down);
                        }
                    }
                }
            }
        }
        if (has_prev_y)
        { // 存在上侧
            Node *up = &m_map[(curr_node->y - 1) * m_map_weight + curr_node->x];
            if (up->visiable && std::find(closelist.begin(), closelist.end(), up) == closelist.end())
            {
                if (curr_node->G + 10 < up->G)
                {
                    up->prev = curr_node;
                    up->G = curr_node->G + 10;
                    up->F = up->G + up->H;
                    if (std::find(openlist.begin(), openlist.end(), up) == openlist.end())
                    {
                        openlist.push_back(up);
                    }
                }
            }
        }
        if (has_next_y)
        { // 存在下侧
            Node *down = &m_map[(curr_node->y + 1) * m_map_weight + curr_node->x];
            if (down->visiable && std::find(closelist.begin(), closelist.end(), down) == closelist.end())
            {
                if (curr_node->G + 10 < down->G)
                {
                    down->prev = curr_node;
                    down->G = curr_node->G + 10;
                    down->F = down->G + down->H;
                    if (std::find(openlist.begin(), openlist.end(), down) == openlist.end())
                    {
                        openlist.push_back(down);
                    }
                }
            }
        }
        openlist.erase(std::find(openlist.begin(), openlist.end(), curr_node)); // 把当前节点从openlist中拿出来

        closelist.push_back(curr_node); // 放到closelist中
    }

    return false; // 没找到路径，直接返回
}

void A_Star::print_path()
{ // 从终点往回追溯路径并打印
    Node *curr_node = m_end;
    while (curr_node != nullptr)
    {
        std::cout << "(" << curr_node->x << ", " << curr_node->y << ")" << std::endl;
        curr_node = curr_node->prev;
    }
}

void A_Star::print_map(std::string path_file)
{
    std::ofstream path(path_file, std::ios::app);
    Node *curr_node = m_end->prev; // 将除了起点和终点的路径标为'*'
    std::vector<Eigen::Vector2d> points;
    while (curr_node->prev != nullptr)
    {
        Eigen::Vector2d point = {curr_node->x, curr_node->y};
        points.push_back(point);
        curr_node = curr_node->prev;
    }

    std::reverse(points.begin(), points.end());
    for (auto point : points)
    {
        path << point.x() << "," << point.y() << std::endl;
    }

    path.close();
}