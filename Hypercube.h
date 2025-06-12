/**
 * @file Hypercube.h
 * @brief 超立方体网络类定义
 * 实现超立方体网络的基本结构和操作，包括节点管理和网络连接
 */

#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "HypercubeNode.h"
#include <vector>

// 超立方体网络类，管理网络节点和连接关系
class Hypercube {
public:
    // 构造函数：创建指定维度的超立方体网络
    Hypercube(int _dimension, int buff1, int buff2);
    
    // 析构函数：释放网络资源
    ~Hypercube();
    
    // 获取网络维度
    int getDimension() const { return dimension; }
    
    // 获取节点总数
    int getNodeCount() const { return nodeCount; }
    
    // 节点访问运算符重载，用于访问指定索引的节点
    HypercubeNode* operator[](int index) { return nodes[index]; }
    
private:
    int dimension;           // 超立方体的维度
    int nodeCount;          // 节点总数
    HypercubeNode** nodes;  // 节点数组
    
    // 初始化网络节点：设置节点坐标和缓冲区
    void initializeNodes(int buff1, int buff2);
    
    // 建立节点间的连接关系：根据超立方体拓扑结构连接相邻节点
    void connectNodes();
};

#endif 