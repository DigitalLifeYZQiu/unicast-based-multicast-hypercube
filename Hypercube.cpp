/**
 * @file Hypercube.cpp
 * @brief 超立方体网络类的实现
 * 实现超立方体网络的创建、初始化和节点连接等功能
 */

#include "Hypercube.h"
#include <cmath>

// 构造函数：创建指定维度的超立方体网络
Hypercube::Hypercube(int _dimension, int buff1, int buff2) {
    dimension = _dimension;
    nodeCount = 1 << dimension;  // 2^dimension
    nodes = new HypercubeNode*[nodeCount];
    
    initializeNodes(buff1, buff2);
    connectNodes();
}

// 析构函数：释放所有节点和节点数组的内存
Hypercube::~Hypercube() {
    for(int i = 0; i < nodeCount; i++) {
        if(nodes[i] != NULL) {
            delete nodes[i];
        }
    }
    delete[] nodes;
}

// 初始化所有节点：设置节点坐标和缓冲区大小
void Hypercube::initializeNodes(int buff1, int buff2) {
    for(int i = 0; i < nodeCount; i++) {
        nodes[i] = new HypercubeNode();
        nodes[i]->setCoordinate(i, dimension, i);
        nodes[i]->setBuffer(buff1, buff2);
    }
}

// 建立节点间的连接关系：根据超立方体拓扑结构连接相邻节点
void Hypercube::connectNodes() {
    for(int i = 0; i < nodeCount; i++) {
        for(int d = 0; d < dimension; d++) {
            // 计算在维度d上的相邻节点
            int neighbor = i ^ (1 << d);
            nodes[i]->linkNodes[d] = neighbor;
        }
    }
} 