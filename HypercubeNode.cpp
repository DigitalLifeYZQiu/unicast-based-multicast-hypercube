#include "HypercubeNode.h"

void Buffer::bufferMin(int chn, int n) {
    if(R1 == chn) {
        r1 -= n;
    } else {
        r2 -= n;
    }
    c -= 1;
    if(R2 == chn)
        s++;
}

void Buffer::bufferPlus(int chn, int n) {
    assert(chn == R1 || chn == R2);
    if(R1 == chn) {
        r1 += n;
    } else {
        r2 += n;
    }
    c += 1;
    if(R2 == chn)
        s--;
}

void HypercubeNode::setCoordinate(int _nodeid, int _dimension, int _coordinate) {
    nodeid = _nodeid;
    dimension = _dimension;
    coordinate = _coordinate;
}

void HypercubeNode::setBuffer(int buff1, int buff2) {
    // 为每个维度创建缓冲区
    for(int i = 0; i < dimension; i++) {
        links[i] = new Buffer();
        links[i]->r1 = buff1;
        links[i]->r2 = buff2;
        links[i]->c = (buff1 + buff2) / MESSLENGTH;
        links[i]->s = 0;
        links[i]->linkused = false;
    }
}

void HypercubeNode::clearBuffer() {
    for(int i = 0; i < dimension; i++) {
        if(links[i] != NULL) {
            delete links[i];
            links[i] = NULL;
        }
    }
} 