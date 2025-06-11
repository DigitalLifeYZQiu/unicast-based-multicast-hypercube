#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "HypercubeNode.h"
#include <vector>

class Hypercube {
public:
    Hypercube(int dim, int bufferSize);
    ~Hypercube();

    HypercubeNode* getNode(int address);
    int getDimension() const { return dimension; }
    int getNodeCount() const { return nodeCount; }
    void clearAllBuffers();

private:
    int dimension;
    int nodeCount;
    std::vector<HypercubeNode> nodes;
};

#endif