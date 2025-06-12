#include "common.h"
#include <iostream>
#include <fstream>

using namespace std;

int getsize(vector<Message*>* mess){
	int size = 0;
	for(int i = 0; i < 10; i++){
		if(!mess[i].empty())
			size += mess[i].size();
	}
		return size;
}

void outtotest(vector<Message*>* vecmess, Hypercube* hypercube) {
    ofstream out("test.txt");
    for(int i = 0; i < 10; i++) {
        vector<Message*>& mess = vecmess[i];
        for(vector<Message*>::iterator it = mess.begin(); it != mess.end(); it++) {
            if((*it)->active) {
                out << "count: " << (*it)->count 
                    << "  src: " << (*it)->src 
                    << "  dst: " << (*it)->dst
                    << "  head: " << (*it)->routpath[0].node
                    << ", R" << (*it)->routpath[0].channel
                    << "  tail: " << (*it)->routpath[MESSLENGTH-1].node
                    << ", R" << (*it)->routpath[MESSLENGTH-1].channel
                    << endl;
            }
        }
    }
}

void bufferleft(Hypercube* hypercube, int dimension) {
    int nodeCount = 1 << dimension;
    int star1 = 0, star2 = 0, star3 = 0, star4 = 0;
    
    for(int sta = 0; sta < nodeCount; sta++) {
        for(int d = 0; d < dimension; d++) {
            star1 += ((*hypercube)[sta]->links[d]->c);
        }
    }
    
    cout << "total buffer left: " << star1 << endl;
}

void drain(vector<Message*>* allvecmess, Hypercube* hypercube, Event* s) {
    for(int i = 0; i < 10; i++) {
        vector<Message*>& vecmess = allvecmess[i];
        for(vector<Message*>::iterator it = vecmess.begin(); it != vecmess.end();) {
            if((*it)->active) {
                s->forwardMes(*(*it));
                if((*it)->active == false) {
                    delete (*it);
                    it = vecmess.erase(it);
                } else {
                    it++;
                }
            } else {
                it++;
            }
        }
    }
}