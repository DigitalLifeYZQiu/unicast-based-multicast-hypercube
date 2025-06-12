/**
 * @file HypercubeRouting.cpp
 * @brief 超立方体路由类的实现
 * 实现消息路由、缓冲区管理和多播树构建等功能
 */

#include "HypercubeRouting.h"
#include <cmath>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

// 转发消息到下一跳节点
NodeInfo* HypercubeRouting::forward(Message& s) {
    next->node = -1;
    next->buff = NULL;

    int curNode = s.routpath[0].node;
    int dstNode = s.dst;

    if(curNode == dstNode) {
        return next;
    }

    // 获取下一个维度
    int nextDim = getNextDimension(curNode, dstNode);
    if(nextDim == -1) {
        return next;
    }

    // 获取当前节点的所有链路
    Buffer** links = (*hypercube)[curNode]->links;
    int* linkNodes = (*hypercube)[curNode]->linkNodes;

    // 选择最佳链路
    int chn = R1;  // 默认使用R1通道
    Buffer* record = NULL;

    // 首先尝试在目标维度上路由
    if(checkBuffer(links[nextDim], chn, record)) {
        next->node = linkNodes[nextDim];
        next->channel = chn;
        next->buff = record;
        return next;
    }

    // 如果目标维度不可用，尝试其他维度
    for(int d = 0; d < dimension; d++) {
        if(d != nextDim && checkBuffer(links[d], chn, record)) {
            next->node = linkNodes[d];
            next->channel = chn;
            next->buff = record;
            return next;
        }
    }

    // 如果R1通道都不可用，尝试R2通道
    chn = R2;
    if(checkBuffer(links[nextDim], chn, record)) {
        next->node = linkNodes[nextDim];
        next->channel = chn;
        next->buff = record;
        return next;
    }

    // 最后尝试其他维度的R2通道
    for(int d = 0; d < dimension; d++) {
        if(d != nextDim && checkBuffer(links[d], chn, record)) {
            next->node = linkNodes[d];
            next->channel = chn;
            next->buff = record;
            return next;
        }
    }

    return next;
}

// 检查缓冲区是否可用
bool HypercubeRouting::checkBuffer(Buffer* buff, int& chn, Buffer*& record) {
    if(buff->linkused) {
        return false;
    }
    
    if(chn == R1) {
        if(buff->r1 >= MESSLENGTH) {
            buff->linkused = true;
            record = buff;
            buff->bufferMin(chn, MESSLENGTH);
            return true;
        }
    } else if(chn == R2) {
        if(buff->r2 >= MESSLENGTH) {
            buff->linkused = true;
            record = buff;
            buff->bufferMin(chn, MESSLENGTH);
            return true;
        }
    }
    
    return false;
}

// 获取下一跳维度
int HypercubeRouting::getNextDimension(int src, int dst) {
    int diff = src ^ dst;
    for(int d = 0; d < dimension; d++) {
        if(diff & (1 << d)) {
            return d;
        }
    }
    return -1;
}

// 选择最优通道
int HypercubeRouting::prefer(Buffer** links, int* linkNodes, int dimension, int& chn, Buffer*& record) {
    // 首先尝试在目标维度上路由
    if(checkBuffer(links[dimension], chn, record)) {
        return dimension;
    }
    
    // 如果目标维度不可用，尝试其他维度
    for(int d = 0; d < this->dimension; d++) {
        if(d != dimension && checkBuffer(links[d], chn, record)) {
            return d;
        }
    }
    
    // 如果R1通道都不可用，尝试R2通道
    chn = R2;
    if(checkBuffer(links[dimension], chn, record)) {
        return dimension;
    }
    
    // 最后尝试其他维度
    for(int d = 0; d < this->dimension; d++) {
        if(d != dimension && checkBuffer(links[d], chn, record)) {
            return d;
        }
    }
    
    return -1;
}

// 添加多播路由表
std::map<int, std::vector<int>> multicast_routes;

// 构建多播树
void HypercubeRouting::build_multicast_tree(int source, const std::vector<int>& destinations) {
    multicast_routes.clear();
    std::set<int> visited;
    visited.insert(source);
    
    // 使用广度优先搜索构建多播树
    std::queue<int> q;
    q.push(source);
    
    while (!q.empty() && visited.size() < destinations.size() + 1) {
        int current = q.front();
        q.pop();
        
        // 检查所有可能的目标节点
        for (int dest : destinations) {
            if (visited.find(dest) != visited.end()) continue;
            
            // 计算当前节点到目标节点的路径
            std::vector<int> path = find_path(current, dest);
            if (!path.empty()) {
                // 将路径添加到多播路由表
                for (size_t i = 0; i < path.size() - 1; i++) {
                    multicast_routes[path[i]].push_back(path[i + 1]);
                }
                visited.insert(dest);
                q.push(dest);
            }
        }
    }
}

// 查找从源节点到目标节点的路径
std::vector<int> HypercubeRouting::find_path(int source, int dest) {
    std::vector<int> path;
    std::queue<int> q;
    std::map<int, int> parent;
    std::set<int> visited;
    
    q.push(source);
    visited.insert(source);
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        
        if (current == dest) {
            // 重建路径
            int node = dest;
            while (node != source) {
                path.push_back(node);
                node = parent[node];
            }
            path.push_back(source);
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        // 获取当前节点的邻居
        std::vector<int> neighbors = get_neighbors(current);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
    
    return path;
}

// 获取节点的邻居
std::vector<int> HypercubeRouting::get_neighbors(int node) {
    std::vector<int> neighbors;
    int n = hypercube->getDimension();
    
    for (int i = 0; i < n; i++) {
        int neighbor = node ^ (1 << i);
        if (neighbor < hypercube->getNodeCount()) {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

// 修改forward函数以支持多播
bool HypercubeRouting::forward(int current, int next, const Message& msg) {
    if (multicast_routes.find(current) == multicast_routes.end()) {
        return false;
    }
    
    // 检查next是否是current的下一个节点
    auto& next_nodes = multicast_routes[current];
    if (std::find(next_nodes.begin(), next_nodes.end(), next) == next_nodes.end()) {
        return false;
    }
    
    // 获取当前节点和下一个节点
    HypercubeNode* current_node = (*hypercube)[current];
    HypercubeNode* next_node = (*hypercube)[next];
    
    if (!current_node || !next_node) {
        return false;
    }
    
    // 找到连接两个节点的维度
    int dim = -1;
    for (int d = 0; d < dimension; d++) {
        if (current_node->linkNodes[d] == next) {
            dim = d;
            break;
        }
    }
    
    if (dim == -1) {
        return false;
    }
    
    // 检查缓冲区
    int chn = R1;
    Buffer* record = NULL;
    if (!checkBuffer(current_node->links[dim], chn, record)) {
        chn = R2;
        if (!checkBuffer(current_node->links[dim], chn, record)) {
            return false;
        }
    }
    
    // 更新消息的路由路径
    Message* msg_copy = const_cast<Message*>(&msg);
    msg_copy->routpath[msg_copy->step].node = next;
    msg_copy->routpath[msg_copy->step].channel = chn;
    msg_copy->routpath[msg_copy->step].buff = record;
    msg_copy->step++;
    
    return true;
} 