#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include "Message.h"

extern int GENERATETYPE;
extern int totalcircle;
extern int dimension; // 超立方体维度

// 获取网络中活动消息数量
int getNetworkSize(const std::vector<Message*>* messages);

#endif