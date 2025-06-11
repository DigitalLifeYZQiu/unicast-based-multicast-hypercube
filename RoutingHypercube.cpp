#include "RoutingHypercube.h"
#include "HypercubeNode.h"
#include "Message.h"
#include <cmath>

RoutingHypercube::RoutingHypercube(Hypercube* cube) {
    this->cube = cube;
    this->dimension = cube->getDimension();
}

NodeInfo* RoutingHypercube::forward(Message& s) {
    // 多播路由
    if(s.destinations.size() > 1) {
        return multicastRoute(s);
    } 
    // 单播路由
    else {
        return unicastRoute(s, s.routpath[0].node, s.dst);
    }
}

NodeInfo* RoutingHypercube::multicastRoute(Message& s) {
    int currentNode = s.routpath[0].node;
    HypercubeNode* current = cube->getNode(currentNode);
    
    // 查找所有目标中的最小维度
    int minDim = dimension;
    for (int dest : s.destinations) {
        int dim = nextDimension(currentNode, dest);
        if (dim < minDim) {
            minDim = dim;
        }
    }
    
    // 没有找到有效维度（不应该发生）
    if (minDim >= dimension) {
        return nullptr;
    }
    
    // 计算下一个节点（翻转最小维度位）
    int nextNode = currentNode ^ (1 << minDim);
    int neighborIndex = findNeighborIndex(currentNode, nextNode);
    
    if (neighborIndex == -1) {
        return nullptr;
    }
    
    Buffer* buffer = current->getBuffer(neighborIndex);
    if (!buffer || buffer->isLinkUsed() || !buffer->canInsert(MESSLENGTH)) {
        return nullptr;
    }
    
    // 设置返回信息
    nextInfo.node = nextNode;
    nextInfo.buffer = buffer;
    
    return &nextInfo;
}

NodeInfo* RoutingHypercube::unicastRoute(Message& s, int current, int destination) {
    if (current == destination) {
        // 到达目的地
        s.active = false;
        return nullptr;
    }
    
    HypercubeNode* currentPtr = cube->getNode(current);
    if (!currentPtr) return nullptr;
    
    int dim = nextDimension(current, destination);
    if (dim >= dimension) return nullptr;
    
    int nextNode = current ^ (1 << dim);
    int neighborIndex = findNeighborIndex(current, nextNode);
    
    if (neighborIndex == -1) {
        return nullptr;
    }
    
    Buffer* buffer = currentPtr->getBuffer(neighborIndex);
    if (!buffer || buffer->isLinkUsed() || !buffer->canInsert(MESSLENGTH)) {
        return nullptr;
    }
    
    // 设置返回信息
    nextInfo.node = nextNode;
    nextInfo.buffer = buffer;
    
    return &nextInfo;
}

int RoutingHypercube::nextDimension(int current, int destination) {
    int diff = current ^ destination;
    for (int dim = 0; dim < dimension; dim++) {
        if (diff & (1 << dim)) {
            return dim;
        }
    }
    return dimension; // 没有找到不同维度
}

int RoutingHypercube::hammingDistance(int a, int b) {
    int dist = 0;
    int diff = a ^ b;
    while (diff) {
        dist++;
        diff &= diff - 1;
    }
    return dist;
}

int RoutingHypercube::findNeighborIndex(int node, int neighbor) {
    HypercubeNode* nodePtr = cube->getNode(node);
    if (!nodePtr) return -1;
    
    const std::vector<int>& neighbors = nodePtr->getNeighbors();
    for (int i = 0; i < static_cast<int>(neighbors.size()); i++) {
        if (neighbors[i] == neighbor) {
            return i;
        }
    }
    return -1;
}