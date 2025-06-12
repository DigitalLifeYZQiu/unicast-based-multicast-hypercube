#ifndef HYPERCUBE_NODE_H
#define HYPERCUBE_NODE_H

#include <iostream>
#include <string>
#include <assert.h>
#define R1 1
#define R2 2
#define MESSLENGTH 16

using namespace std;

class Hypercube;

class Buffer {
public:
    bool linkused;   // 检查物理链路是否被使用
    int r1;          // 虚拟通道1的缓冲区大小
    int r2;          // 虚拟通道2的缓冲区大小
    void bufferMin(int chn, int n);
    void bufferPlus(int chn, int n);
    int c;           // 流控制缓冲区
    int s;           // 流控制特殊缓冲区
};

class NodeInfo {
public:
    int node;        // 下一跳节点
    int channel;     // 使用的虚拟通道(R1或R2)
    Buffer* buff;    // 指向占用的缓冲区
    NodeInfo(): node(-1) {}
};

class HypercubeNode {
public:
    int nodeid;      // 节点ID
    int dimension;   // 节点在超立方体中的维度
    int coordinate;  // 节点的二进制坐标
    
    Buffer* links[32];  // 最多支持32维超立方体
    int linkNodes[32];  // 相邻节点的ID
    
    void setCoordinate(int _nodeid, int _dimension, int _coordinate);
    void setBuffer(int buff1, int buff2);
    void clearBuffer();
};

#endif 