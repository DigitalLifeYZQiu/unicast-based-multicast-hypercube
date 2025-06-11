#ifndef EVENT_H
#define EVENT_H

#include "Allrouting.h"
#include "Message.h"
#include "Hypercube.h"
#include <vector>

class Event {
public:
    Event(Allrouting* routing);
    ~Event();
    
    Message* generateMessage();
    void forwardMessage(Message& msg);
    int consumed;
    double totalcir; // total circle all the messages take;
    int messarrive; // how many message arrived
    
private:
    Allrouting* routing;
    Hypercube* cube;
    int dimension;
};

#endif