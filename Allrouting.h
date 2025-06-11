#ifndef ALLROUTING_H
#define ALLROUTING_H

class Hypercube;
class Message;
class NodeInfo;

class Allrouting {
public:
    virtual ~Allrouting() {}
    virtual NodeInfo* forward(Message& s) = 0;
    Hypercube* getCube() const { return cube; }
    int getDimension() const { return dimension; }
    
protected:
    Hypercube* cube;
    int dimension;
};

#endif