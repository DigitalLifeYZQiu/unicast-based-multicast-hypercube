#ifndef HYPERCUBE_ROUTING_H
#define HYPERCUBE_ROUTING_H

#include "Hypercube.h"
#include "Message.h"

class HypercubeRouting {
public:
    HypercubeRouting(Hypercube* _hypercube) {
        hypercube = _hypercube;
        dimension = hypercube->getDimension();
        next = new NodeInfo();
    }
    
    ~HypercubeRouting() {
        delete next;
    }
    
    NodeInfo* forward(Message& s);
    bool checkBuffer(Buffer* buff, int& chn, Buffer*& record);
    Hypercube* getHypercube() { return hypercube; }
    
private:
    Hypercube* hypercube;
    int dimension;
    NodeInfo* next;
    
    int getNextDimension(int src, int dst);
    int prefer(Buffer** links, int* linkNodes, int dimension, int& chn, Buffer*& record);
};

#endif 