/**
 * @file Event.h
 * @brief 事件处理类定义
 * 用于处理消息生成和转发的事件类
 */

#ifndef EVENT_H
#define EVENT_H

#include "Hypercube.h"
#include "HypercubeRouting.h"
#include "Message.h"
#include <vector>

/**
 * @class Event
 * @brief 事件处理类，负责消息的生成和转发
 */
class Event {
public:
	/**
	 * @brief 构造函数
	 * @param rout1 路由对象指针
	 */
	Event(HypercubeRouting* rout1);
	
	/**
	 * @brief 析构函数
	 */
	~Event();
	
	/**
	 * @brief 生成新消息
	 * @return 返回生成的消息指针
	 */
	Message* genMes();
	
	/**
	 * @brief 转发消息
	 * @param s 要转发的消息引用
	 */
	void forwardMes(Message& s);
	
	int consumed;    ///< 已消费的消息数量
	int totalcir;    ///< 总循环次数
	int messarrive;  ///< 到达的消息数量
	
private:
	HypercubeRouting* rout;  ///< 路由对象指针
	Hypercube* hypercube;    ///< 超立方体网络指针
	int dimension;           ///< 网络维度
};

#endif