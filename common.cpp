#include "common.h"

int GENERATETYPE = 1;
int totalcircle = 10000;


int getNetworkSize(const std::vector<Message*>* messages) {
    int count = 0;
    for (const auto& msg : *messages) {
        if (msg->active) count++;
    }
    return count;
}