/**
 * @file Message.h
 * @brief 消息类定义
 * 实现超立方体网络中的消息结构和处理
 */

#ifndef  MESS_AGE
#define  MESS_AGE

#define PROCESSTIME 8    ///< 消息处理时间
#define TIMEOUT   100000 ///< 消息超时时间
#define MESSLENGTH 16    ///< 消息长度

#include<iostream>
#include<string>
#include<assert.h>
#include"HypercubeNode.h"
using namespace std;

class Buffer;
class NodeInfo;

/**
 * @class Message
 * @brief 消息类，表示在网络中传输的数据包
 */
class Message {
public:
	int length;           ///< 消息长度（以flits为单位）
	int src;             ///< 消息源节点
	int dst;             ///< 消息目标节点
	int timeout;         ///< 消息等待通道的超时时间（以周期为单位）
	int begintrans;      ///< 消息生成后到开始传输的时间
	int step;            ///< 消息已经走过的步数
	bool active;         ///< 消息是否被消费或到达目标节点
	NodeInfo* routpath;  ///< 路由路径，routpath[i]表示第i个flit当前所在节点和使用的缓冲区
	int count;           ///< 消息消耗的总时间
	bool releaselink;    ///< 消息尾部移动时是否释放物理链路
	bool turn;           ///< 用于气泡流控制，为true时请求通道需要2个缓冲区

	/**
	 * @brief 默认构造函数
	 */
	Message() {
		src = -1;
		dst = -1;
	}

	/**
	 * @brief 带参数的构造函数
	 * @param src1 源节点
	 * @param dst1 目标节点
	 */
	Message(int src1, int dst1) {
		begintrans = PROCESSTIME;
		src = src1;
		dst = dst1;
		routpath = new NodeInfo[MESSLENGTH];
		assert(routpath);
		for(int i = 0; i < MESSLENGTH; i++) {
			routpath[i].node = src;
			routpath[i].channel = 0;
			routpath[i].buff = NULL;
		}
		step = 0;
		active = true;
		length = MESSLENGTH;
		count = 0;
		releaselink = false;
		turn = true;
		timeout = 0;
	}

	/**
	 * @brief 析构函数
	 */
	~Message() {
		delete []routpath;
	}

	/**
	 * @brief 设置消息长度
	 * @param n 新的消息长度
	 */
	void setLength(int n) {
		length = n;
	}
};

#endif