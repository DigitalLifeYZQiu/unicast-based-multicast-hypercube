/**
 * @file HypercubeNode.h
 * @brief 超立方体节点类定义
 * 实现超立方体网络中的节点结构和缓冲区管理
 */

#ifndef HYPERCUBE_NODE_H
#define HYPERCUBE_NODE_H

#include <iostream>
#include <string>
#include <assert.h>

// 虚拟通道标识
#define R1 1  // 虚拟通道1
#define R2 2  // 虚拟通道2
#define MESSLENGTH 16  // 消息长度

using namespace std;

class Hypercube;

// 缓冲区类：管理节点的虚拟通道和流控制
class Buffer {
public:
    bool linkused;   // 检查物理链路是否被使用
    int r1;          // 虚拟通道1的缓冲区大小
    int r2;          // 虚拟通道2的缓冲区大小
    
    // 减少指定通道的缓冲区大小
    void bufferMin(int chn, int n);
    
    // 增加指定通道的缓冲区大小
    void bufferPlus(int chn, int n);
    
    int c;           // 流控制缓冲区
    int s;           // 流控制特殊缓冲区
};

// 节点信息类：存储路由信息
class NodeInfo {
public:
    int node;        // 下一跳节点
    int channel;     // 使用的虚拟通道(R1或R2)
    Buffer* buff;    // 指向占用的缓冲区
    NodeInfo(): node(-1) {}  // 构造函数，初始化node为-1
};

// 超立方体节点类：表示网络中的一个节点
class HypercubeNode {
public:
    int nodeid;      // 节点ID
    int dimension;   // 节点在超立方体中的维度
    int coordinate;  // 节点的二进制坐标
    
    Buffer* links[32];  // 最多支持32维超立方体的链路缓冲区
    int linkNodes[32];  // 相邻节点的ID数组
    
    // 设置节点坐标信息
    void setCoordinate(int _nodeid, int _dimension, int _coordinate);
    
    // 设置节点缓冲区大小
    void setBuffer(int buff1, int buff2);
    
    // 清空节点缓冲区
    void clearBuffer();
};

#endif 