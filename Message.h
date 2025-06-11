#ifndef MESSAGE_H
#define MESSAGE_H

#include "NodeInfo.h"
#include <vector>

#define MESSLENGTH 16

class Message {
public:
    int src;          // 源节点
    int dst;          // 当前目的地（多播时变化）
    std::vector<int> destinations; // 多播目的节点列表
    int timeout;      // 超时计数器
    int begintrans;   // 开始传输倒计时
    bool active;      // 消息是否活跃
    NodeInfo* routpath; // 路由路径信息（每个flit）
    int count;        // 总耗时
    bool releaselink; // 是否需要释放链路
    
    Message();
    Message(int src, const std::vector<int>& dests);
    ~Message();
    
    // 创建针对特定目的地的消息副本
    Message* copyForDestination(int dest);
    
private:
    void initialize();
};

#endif