/**
 * @file common.h
 * @brief 通用工具函数和全局声明
 * 包含项目中使用的通用函数声明和必要的头文件包含
 */

#ifndef CO_MM_O1
#define CO_MM_O1

#include "Hypercube.h"
#include "HypercubeRouting.h"
#include "Event.h"
#include "Message.h"
#include <vector>
#include <fstream>

/**
 * @brief 将消息输出到测试文件
 * @param vecmess 消息向量指针
 * @param hypercube 超立方体网络指针
 */
void outtotest(vector<Message*>* vecmess, Hypercube* hypercube);

/**
 * @brief 处理缓冲区中剩余的消息
 * @param hypercube 超立方体网络指针
 * @param dimension 维度参数
 */
void bufferleft(Hypercube* hypercube, int dimension);

/**
 * @brief 清空消息队列
 * @param vecmess 消息向量指针
 * @param hypercube 超立方体网络指针
 * @param s 事件指针
 */
void drain(vector<Message*>* vecmess, Hypercube* hypercube, Event* s);

#endif