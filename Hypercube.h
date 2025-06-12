#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "HypercubeNode.h"
#include <vector>

class Hypercube {
public:
    Hypercube(int _dimension, int buff1, int buff2);
    ~Hypercube();
    
    int getDimension() const { return dimension; }
    int getNodeCount() const { return nodeCount; }
    HypercubeNode* operator[](int index) { return nodes[index]; }
    
private:
    int dimension;           // 超立方体的维度
    int nodeCount;          // 节点总数
    HypercubeNode** nodes;  // 节点数组
    
    void initializeNodes(int buff1, int buff2);
    void connectNodes();
};

#endif 