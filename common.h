#ifndef CO_MM_O1
#define CO_MM_O1

#include "Hypercube.h"
#include "HypercubeRouting.h"
#include "Event.h"
#include "Message.h"
#include <vector>
#include <fstream>

void outtotest(vector<Message*>* vecmess, Hypercube* hypercube);
void bufferleft(Hypercube* hypercube, int dimension);
void drain(vector<Message*>* vecmess, Hypercube* hypercube, Event* s);

#endif