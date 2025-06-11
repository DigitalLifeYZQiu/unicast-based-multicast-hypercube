#ifndef ROUTING_HYPERCUBE_H
#define ROUTING_HYPERCUBE_H

#include "Allrouting.h"
#include "Hypercube.h"
#include "NodeInfo.h"

class RoutingHypercube : public Allrouting {
public:
    RoutingHypercube(Hypercube* cube);
    virtual NodeInfo* forward(Message& s) override;

private:
    NodeInfo* multicastRoute(Message& s);
    NodeInfo* unicastRoute(Message& s, int current, int destination);
    int nextDimension(int current, int destination);
    int hammingDistance(int a, int b);
    int findNeighborIndex(int node, int neighbor);
    
    NodeInfo nextInfo; // 用于返回结果
};

#endif