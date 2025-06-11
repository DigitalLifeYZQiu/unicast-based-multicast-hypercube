#include "Hypercube.h"
#include <cmath>

Hypercube::Hypercube(int dim, int bufferSize) 
    : dimension(dim), nodeCount(1 << dim) {
    
    nodes.reserve(nodeCount);
    
    // 初始化所有节点
    for (int i = 0; i < nodeCount; i++) {
        nodes.emplace_back(i, dim);
        
        // 设置邻居（每个维度的连接）
        std::vector<int> neighbors;
        for (int d = 0; d < dim; d++) {
            int neighbor = i ^ (1 << d);
            neighbors.push_back(neighbor);
        }
        nodes[i].setNeighbors(neighbors);
        
        // 设置缓冲区
        nodes[i].setBuffer(bufferSize);
    }
}

Hypercube::~Hypercube() {}

HypercubeNode* Hypercube::getNode(int address) {
    if (address >= 0 && address < nodeCount) {
        return &nodes[address];
    }
    return nullptr;
}

void Hypercube::clearAllBuffers() {
    for (auto& node : nodes) {
        node.clearBuffer();
    }
}