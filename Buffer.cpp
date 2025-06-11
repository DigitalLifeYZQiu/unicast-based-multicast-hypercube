#include "Buffer.h"

Buffer::Buffer() : size(0), occupied(0), linkUsed(false) {}

void Buffer::reset(int bufferSize) {
    size = bufferSize;
    occupied = 0;
    linkUsed = false;
}

void Buffer::insert(int flits) {
    if (occupied + flits <= size) {
        occupied += flits;
    }
}

void Buffer::release(int flits) {
    if (occupied >= flits) {
        occupied -= flits;
    } else {
        occupied = 0;
    }
}