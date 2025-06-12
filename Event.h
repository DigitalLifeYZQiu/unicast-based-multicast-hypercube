#ifndef EVENT_H
#define EVENT_H

#include "Hypercube.h"
#include "HypercubeRouting.h"
#include "Message.h"
#include <vector>

class Event {
public:
	Event(HypercubeRouting* rout1);
	~Event();
	
	Message* genMes();
	void forwardMes(Message& s);
	
	int consumed;
	int totalcir;
	int messarrive;
	
private:
	HypercubeRouting* rout;
	Hypercube* hypercube;
	int dimension;
};

#endif