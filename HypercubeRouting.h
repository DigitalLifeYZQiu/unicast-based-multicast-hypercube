/**
 * @file HypercubeRouting.h
 * @brief 超立方体路由类定义
 * 实现超立方体网络中的路由算法和缓冲区管理，包括单播和多播路由功能
 */

#ifndef HYPERCUBE_ROUTING_H
#define HYPERCUBE_ROUTING_H

#include "Hypercube.h"
#include "Message.h"
#include <map>
#include <vector>

// 超立方体路由类：负责消息的路由和转发
class HypercubeRouting {
private:
    Hypercube* hypercube;  // 超立方体网络指针
    std::map<int, std::vector<int>> multicast_routes;  // 多播路由表
    int dimension;  // 网络维度
    NodeInfo* next;  // 下一跳节点信息

    // 查找从源节点到目标节点的路径
    std::vector<int> find_path(int source, int dest);
    
    // 获取节点的邻居节点列表
    std::vector<int> get_neighbors(int node);

    // 检查缓冲区是否可用
    bool checkBuffer(Buffer* buff, int& chn, Buffer*& record);
    
    // 获取下一跳维度
    int getNextDimension(int src, int dst);
    
    // 选择最优通道
    int prefer(Buffer** links, int* linkNodes, int dimension, int& chn, Buffer*& record);

public:
    // 构造函数：初始化路由对象
    HypercubeRouting(Hypercube* h) : hypercube(h) {
        dimension = hypercube->getDimension();
        next = new NodeInfo();
    }
    
    // 析构函数：释放资源
    ~HypercubeRouting() {
        delete next;
    }
    
    // 构建多播树：为给定的源节点和目标节点集合构建多播路由树
    void build_multicast_tree(int source, const std::vector<int>& destinations);
    
    // 转发消息：将消息从当前节点转发到下一跳节点
    bool forward(int current, int next, const Message& msg);
    
    // 原有的单播路由函数
    NodeInfo* forward(Message& s);
    
    // 选择最优路由：根据当前节点和目标节点选择最优路由
    int prefer(int current, int dest);
    
    // 获取超立方体网络指针
    Hypercube* getHypercube() { return hypercube; }
};

#endif 