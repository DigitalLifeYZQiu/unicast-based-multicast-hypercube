#ifndef HYPERCUBE_NODE_H
#define HYPERCUBE_NODE_H

#include "Buffer.h"
#include <vector>

class HypercubeNode {
public:
    HypercubeNode(int addr, int dim);
    
    void setNeighbors(const std::vector<int>& neighbors);
    void setBuffer(int bufferSize);
    void clearBuffer();
    
    int getAddress() const { return address; }
    const std::vector<int>& getNeighbors() const { return neighbors; }
    Buffer* getBuffer(int neighborIndex);
    
private:
    int address;
    int dimension;
    std::vector<int> neighbors;
    std::vector<Buffer> buffers; // 每个邻居方向一个缓冲区
};

#endif