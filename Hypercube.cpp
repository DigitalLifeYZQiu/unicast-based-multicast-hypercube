#include "Hypercube.h"
#include <cmath>

Hypercube::Hypercube(int _dimension, int buff1, int buff2) {
    dimension = _dimension;
    nodeCount = 1 << dimension;  // 2^dimension
    nodes = new HypercubeNode*[nodeCount];
    
    initializeNodes(buff1, buff2);
    connectNodes();
}

Hypercube::~Hypercube() {
    for(int i = 0; i < nodeCount; i++) {
        if(nodes[i] != NULL) {
            delete nodes[i];
        }
    }
    delete[] nodes;
}

void Hypercube::initializeNodes(int buff1, int buff2) {
    for(int i = 0; i < nodeCount; i++) {
        nodes[i] = new HypercubeNode();
        nodes[i]->setCoordinate(i, dimension, i);
        nodes[i]->setBuffer(buff1, buff2);
    }
}

void Hypercube::connectNodes() {
    for(int i = 0; i < nodeCount; i++) {
        for(int d = 0; d < dimension; d++) {
            // 计算在维度d上的相邻节点
            int neighbor = i ^ (1 << d);
            nodes[i]->linkNodes[d] = neighbor;
        }
    }
} 