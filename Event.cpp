#include "Event.h"
#include "RoutingHypercube.h"
#include <cstdlib>
#include <ctime>

Event::Event(Allrouting* routing) : routing(routing) {
    RoutingHypercube* hypercubeRouting = dynamic_cast<RoutingHypercube*>(routing);
    if (hypercubeRouting) {
//        cube = hypercubeRouting->cube;
        cube = hypercubeRouting->getCube();
//        dimension = hypercubeRouting->dimension;
        dimension = hypercubeRouting->getDimension();
    }
    srand(time(0));
}

Event::~Event() {}

Message* Event::generateMessage() {
    int nodeCount = 1 << dimension;
    
    // 随机源节点
    int src = rand() % nodeCount;
    
    // 随机选择1-10个目的节点
    int destCount = 1 + rand() % 10;
    std::vector<int> dests;
    for (int i = 0; i < destCount; i++) {
        int dest;
        do {
            dest = rand() % nodeCount;
        } while (dest == src || 
                std::find(dests.begin(), dests.end(), dest) != dests.end());
        dests.push_back(dest);
    }
    
    return new Message(src, dests);
}

void Event::forwardMessage(Message& msg) {
    if (!msg.active) return;
    
    NodeInfo* next = routing->forward(msg);
    if (!next) {
        msg.timeout++;
        if (msg.timeout > 1000) {
            msg.active = false; // 超时丢弃
        }
        return;
    }
    
    // 更新路由路径
    for (int i = MESSLENGTH - 1; i > 0; i--) {
        msg.routpath[i] = msg.routpath[i-1];
    }
    msg.routpath[0] = *next;
    
    // 标记链路使用
    if (next->buffer) {
        next->buffer->setLinkUsed(true);
        next->buffer->insert(MESSLENGTH);
    }
    
    // 多播处理：到达中间节点时检查是否需要复制
    if (!msg.destinations.empty() && msg.destinations.size() > 1) {
        int currentNode = msg.routpath[0].node;
        if (std::find(msg.destinations.begin(), msg.destinations.end(), currentNode) != msg.destinations.end()) {
            // 当前节点是目的地之一
            msg.destinations.erase(
                std::remove(msg.destinations.begin(), msg.destinations.end(), currentNode),
                msg.destinations.end()
            );
            
            if (msg.destinations.empty()) {
                msg.active = false;
            }
        }
    }
    
    // 单播处理：检查是否到达目的地
    if (msg.destinations.size() == 1 && msg.routpath[0].node == msg.dst) {
        msg.active = false;
    }
    
    msg.count++;
}