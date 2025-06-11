#include "HypercubeNode.h"

HypercubeNode::HypercubeNode(int addr, int dim) 
    : address(addr), dimension(dim) {}

void HypercubeNode::setNeighbors(const std::vector<int>& neighbors) {
    this->neighbors = neighbors;
    buffers.resize(neighbors.size());
}

void HypercubeNode::setBuffer(int bufferSize) {
    for (auto& buffer : buffers) {
        buffer.reset(bufferSize);
    }
}

void HypercubeNode::clearBuffer() {
    for (auto& buffer : buffers) {
        buffer.reset();
    }
}

Buffer* HypercubeNode::getBuffer(int neighborIndex) {
    if (neighborIndex >= 0 && neighborIndex < static_cast<int>(buffers.size())) {
        return &buffers[neighborIndex];
    }
    return nullptr;
}