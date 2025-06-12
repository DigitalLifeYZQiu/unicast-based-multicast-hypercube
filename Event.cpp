/**
 * @file Event.cpp
 * @brief 事件处理类的实现
 * 实现消息生成和转发的事件处理功能
 */

#include "Event.h"
#include <cmath>
#include <cstdlib>
#include "Message.h"

extern int GENERATETYPE;  // 流量模式类型

// 构造函数：初始化事件处理对象
Event::Event(HypercubeRouting* rout1) {
	consumed = 0;
	totalcir = 0;
	messarrive = 0;
	rout = rout1;
	hypercube = rout1->getHypercube();
	dimension = hypercube->getDimension();
}

// 析构函数
Event::~Event() {
}

// 生成新消息
Message* Event::genMes() {
	int nodeCount = 1 << dimension;
	
	// uniform流量模式：随机选择源节点和目标节点
	if(GENERATETYPE == 1) {
		int src = rand() % nodeCount;
		int dest;
		do {
			dest = rand() % nodeCount;
		} while(dest == src);
		return new Message(src, dest);
	}
	
	// 对角线流量模式：源节点和目标节点在二进制表示上完全相反
	if(GENERATETYPE == 2) {
		int src = rand() % nodeCount;
		int dest = src ^ ((1 << dimension) - 1);  // 取反所有位
		return new Message(src, dest);
	}
	
	// 随机流量模式：随机选择源节点和目标节点
	if(GENERATETYPE == 3) {
		int src = rand() % nodeCount;
		int dest;
		do {
			dest = rand() % nodeCount;
		} while(dest == src);
		return new Message(src, dest);
	}
	
	return NULL;
}

// 转发消息
void Event::forwardMes(Message& s) {
	s.count++;  // 每次转发消息时增加计数
	
	// 消息在源节点
	if(s.routpath[0].node == s.src) {
		if(s.begintrans > 0) {
			s.begintrans--;  // 等待开始传输
		} else {
			s.begintrans--;
			NodeInfo* next = rout->forward(s);  // 获取下一跳节点
			if(next->node == -1) {
				s.timeout++;  // 路由失败，增加超时计数
			} else {
				s.timeout = 0;  // 重置超时计数
				assert(s.routpath[0].node != next->node);
				s.routpath[0] = *next;  // 更新路由路径
			}
		}
	} else {
		// 消息在中间节点
		if(s.routpath[0].node != s.dst) {
			NodeInfo* next = rout->forward(s);
			if(next->node == -1) {
				// 路由失败，尝试移动消息
				int i;
				for(i = 1; i < MESSLENGTH && s.routpath[i].node == s.routpath[0].node; i++);
				if(i < MESSLENGTH) {
					NodeInfo temp1, temp2;
					temp2 = s.routpath[i - 1];
					while(i < MESSLENGTH) {
						temp1 = s.routpath[i];
						s.routpath[i] = temp2;
						temp2 = temp1;
						i++;
					}
					if(temp2.node != s.routpath[MESSLENGTH - 1].node) {
						assert(s.routpath[MESSLENGTH - 1].buff->linkused);
						s.releaselink = true;  // 需要释放链路
					}
					if(temp2.buff != NULL && temp2.node != s.routpath[MESSLENGTH - 1].node)
						temp2.buff->bufferPlus(temp2.channel, MESSLENGTH);  // 释放缓冲区
				}
				s.timeout++;  // 增加超时计数
			} else {
				s.timeout = 0;  // 重置超时计数
				// 移动消息
				NodeInfo temp1, temp2;
				temp2 = s.routpath[0];
				int i = 1;
				while(i < MESSLENGTH) {
					temp1 = s.routpath[i];
					s.routpath[i] = temp2;
					temp2 = temp1;
					i++;
				}
				if(temp2.node != s.routpath[MESSLENGTH - 1].node) {
					assert(s.routpath[MESSLENGTH - 1].buff->linkused);
					s.releaselink = true;  // 需要释放链路
				}
				if(temp2.buff != NULL && temp2.node != s.routpath[MESSLENGTH - 1].node)
					temp2.buff->bufferPlus(temp2.channel, MESSLENGTH);  // 释放缓冲区
				
				s.routpath[0] = *next;  // 更新路由路径
			}
		} else {
			// 消息到达目标节点
			NodeInfo temp1, temp2;
			temp2 = s.routpath[0];
			int i;
			for(i = 1; i < MESSLENGTH && s.routpath[i].node == s.routpath[0].node; i++);
			if(i == MESSLENGTH) {
				s.routpath[i-1].buff->bufferPlus(s.routpath[i-1].channel, MESSLENGTH);  // 释放缓冲区
				s.active = false;  // 消息处理完成
				totalcir += s.count;  // 更新总延迟计数
				messarrive++;  // 更新到达消息计数
				return;
			}
			while(i < MESSLENGTH) {
				temp1 = s.routpath[i];
				s.routpath[i] = temp2;
				temp2 = temp1;
				i++;
			}
			if(temp2.node != s.routpath[MESSLENGTH-1].node)
				s.releaselink = true;  // 需要释放链路
			
			if(temp2.buff != NULL && temp2.node != s.routpath[MESSLENGTH - 1].node)
				temp2.buff->bufferPlus(temp2.channel, MESSLENGTH);  // 释放缓冲区
		}
	}
}
