#ifndef NODE_INFO_H
#define NODE_INFO_H

#include "Buffer.h"

struct NodeInfo {
    int node;        // 节点地址
    int channel;     // 通道标识（保留）
    Buffer* buffer;  // 指向使用的缓冲区
    
    NodeInfo() : node(-1), channel(0), buffer(nullptr) {}
};

#endif