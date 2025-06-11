#include "common.h"

int GENERATETYPE = 1;
int totalcircle = 100000;
int dimension = 4; // 4维超立方体，共16个节点

int getNetworkSize(const std::vector<Message*>* messages) {
    int count = 0;
    for (const auto& msg : *messages) {
        if (msg->active) count++;
    }
    return count;
}