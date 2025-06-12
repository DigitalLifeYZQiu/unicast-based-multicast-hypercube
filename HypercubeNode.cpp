/**
 * @file HypercubeNode.cpp
 * @brief 超立方体节点类的实现
 * 实现节点缓冲区管理和坐标设置等功能
 */

#include "HypercubeNode.h"

// 减少指定通道的缓冲区大小
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

// 增加指定通道的缓冲区大小
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

// 设置节点的坐标信息
void HypercubeNode::setCoordinate(int _nodeid, int _dimension, int _coordinate) {
    nodeid = _nodeid;
    dimension = _dimension;
    coordinate = _coordinate;
}

// 初始化节点的缓冲区
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

// 清空节点的所有缓冲区
void HypercubeNode::clearBuffer() {
    for(int i = 0; i < dimension; i++) {
        if(links[i] != NULL) {
            delete links[i];
            links[i] = NULL;
        }
    }
} 