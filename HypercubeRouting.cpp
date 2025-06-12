#include "HypercubeRouting.h"
#include <cmath>

NodeInfo* HypercubeRouting::forward(Message& s) {
    next->node = -1;
    next->buff = NULL;
    
    int curNode = s.routpath[0].node;
    int dstNode = s.dst;
    
    if(curNode == dstNode) {
        return next;
    }
    
    // 获取下一个维度
    int nextDim = getNextDimension(curNode, dstNode);
    if(nextDim == -1) {
        return next;
    }
    
    // 获取当前节点的所有链路
    Buffer** links = (*hypercube)[curNode]->links;
    int* linkNodes = (*hypercube)[curNode]->linkNodes;
    
    // 选择最佳链路
    int chn = R1;  // 默认使用R1通道
    Buffer* record = NULL;
    
    // 首先尝试在目标维度上路由
    if(checkBuffer(links[nextDim], chn, record)) {
        next->node = linkNodes[nextDim];
        next->channel = chn;
        next->buff = record;
        return next;
    }
    
    // 如果目标维度不可用，尝试其他维度
    for(int d = 0; d < dimension; d++) {
        if(d != nextDim && checkBuffer(links[d], chn, record)) {
            next->node = linkNodes[d];
            next->channel = chn;
            next->buff = record;
            return next;
        }
    }
    
    // 如果R1通道都不可用，尝试R2通道
    chn = R2;
    if(checkBuffer(links[nextDim], chn, record)) {
        next->node = linkNodes[nextDim];
        next->channel = chn;
        next->buff = record;
        return next;
    }
    
    // 最后尝试其他维度的R2通道
    for(int d = 0; d < dimension; d++) {
        if(d != nextDim && checkBuffer(links[d], chn, record)) {
            next->node = linkNodes[d];
            next->channel = chn;
            next->buff = record;
            return next;
        }
    }
    
    return next;
}

bool HypercubeRouting::checkBuffer(Buffer* buff, int& chn, Buffer*& record) {
    if(buff->linkused) {
        return false;
    }
    
    if(chn == R1) {
        if(buff->r1 >= MESSLENGTH) {
            buff->linkused = true;
            record = buff;
            buff->bufferMin(chn, MESSLENGTH);
            return true;
        }
    } else if(chn == R2) {
        if(buff->r2 >= MESSLENGTH) {
            buff->linkused = true;
            record = buff;
            buff->bufferMin(chn, MESSLENGTH);
            return true;
        }
    }
    
    return false;
}

int HypercubeRouting::getNextDimension(int src, int dst) {
    int diff = src ^ dst;
    for(int d = 0; d < dimension; d++) {
        if(diff & (1 << d)) {
            return d;
        }
    }
    return -1;
}

int HypercubeRouting::prefer(Buffer** links, int* linkNodes, int dimension, int& chn, Buffer*& record) {
    // 首先尝试在目标维度上路由
    if(checkBuffer(links[dimension], chn, record)) {
        return dimension;
    }
    
    // 如果目标维度不可用，尝试其他维度
    for(int d = 0; d < this->dimension; d++) {
        if(d != dimension && checkBuffer(links[d], chn, record)) {
            return d;
        }
    }
    
    // 如果R1通道都不可用，尝试R2通道
    chn = R2;
    if(checkBuffer(links[dimension], chn, record)) {
        return dimension;
    }
    
    // 最后尝试其他维度的R2通道
    for(int d = 0; d < this->dimension; d++) {
        if(d != dimension && checkBuffer(links[d], chn, record)) {
            return d;
        }
    }
    
    return -1;
} 