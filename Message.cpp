#include "Message.h"

Message::Message() : src(-1), dst(-1), timeout(0), begintrans(0), 
                    active(false), routpath(nullptr), count(0), releaselink(false) {}

Message::Message(int src, const std::vector<int>& dests) : 
    src(src), destinations(dests), dst(dests.empty() ? -1 : dests[0]),
    timeout(0), begintrans(0), active(true), count(0), releaselink(false) {
    
    initialize();
}

Message::~Message() {
    delete[] routpath;
}

void Message::initialize() {
    routpath = new NodeInfo[MESSLENGTH];
    for (int i = 0; i < MESSLENGTH; i++) {
        routpath[i].node = src;
        routpath[i].buffer = nullptr;
    }
}

Message* Message::copyForDestination(int dest) {
    Message* copy = new Message();
    copy->src = this->src;
    copy->dst = dest;
    copy->destinations = {dest}; // 新消息只针对一个目的地
    copy->active = true;
    copy->count = this->count;
    
    // 复制路由路径
    copy->routpath = new NodeInfo[MESSLENGTH];
    for (int i = 0; i < MESSLENGTH; i++) {
        copy->routpath[i] = this->routpath[i];
    }
    
    return copy;
}