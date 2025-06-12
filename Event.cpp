#include "Event.h"
#include <cmath>
#include <cstdlib>
#include "Message.h"

extern int GENERATETYPE;

Event::Event(HypercubeRouting* rout1) {
	consumed = 0;
	totalcir = 0;
	messarrive = 0;
	rout = rout1;
	hypercube = rout1->getHypercube();
	dimension = hypercube->getDimension();
}

Event::~Event() {
}

Message* Event::genMes() {
	int nodeCount = 1 << dimension;
	
	// uniform流量模式
	if(GENERATETYPE == 1) {
		int src = rand() % nodeCount;
		int dest;
		do {
			dest = rand() % nodeCount;
		} while(dest == src);
		return new Message(src, dest);
	}
	
	// 对角线流量模式
	if(GENERATETYPE == 2) {
		int src = rand() % nodeCount;
		int dest = src ^ ((1 << dimension) - 1);  // 取反所有位
		return new Message(src, dest);
	}
	
	// 随机流量模式
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

void Event::forwardMes(Message& s) {
	s.count++;  // 每次转发消息时增加计数
	
	if(s.routpath[0].node == s.src) {
		if(s.begintrans > 0) {
			s.begintrans--;
		} else {
			s.begintrans--;
			NodeInfo* next = rout->forward(s);
			if(next->node == -1) {
				s.timeout++;
			} else {
				s.timeout = 0;
				assert(s.routpath[0].node != next->node);
				s.routpath[0] = *next;
			}
		}
	} else {
		if(s.routpath[0].node != s.dst) {
			NodeInfo* next = rout->forward(s);
			if(next->node == -1) {
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
						s.releaselink = true;
					}
					if(temp2.buff != NULL && temp2.node != s.routpath[MESSLENGTH - 1].node)
						temp2.buff->bufferPlus(temp2.channel, MESSLENGTH);
				}
				s.timeout++;
			} else {
				s.timeout = 0;
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
					s.releaselink = true;
				}
				if(temp2.buff != NULL && temp2.node != s.routpath[MESSLENGTH - 1].node)
					temp2.buff->bufferPlus(temp2.channel, MESSLENGTH);
				
				s.routpath[0] = *next;
			}
		} else {
			NodeInfo temp1, temp2;
			temp2 = s.routpath[0];
			int i;
			for(i = 1; i < MESSLENGTH && s.routpath[i].node == s.routpath[0].node; i++);
			if(i == MESSLENGTH) {
				s.routpath[i-1].buff->bufferPlus(s.routpath[i-1].channel, MESSLENGTH);
				s.active = false;
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
				s.releaselink = true;
			
			if(temp2.buff != NULL && temp2.node != s.routpath[MESSLENGTH - 1].node)
				temp2.buff->bufferPlus(temp2.channel, MESSLENGTH);
		}
	}
}
