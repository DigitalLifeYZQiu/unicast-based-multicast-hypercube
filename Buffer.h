#ifndef BUFFER_H
#define BUFFER_H

class Buffer {
public:
    Buffer();
    
    void reset(int size = 0);
    bool isLinkUsed() const { return linkUsed; }
    void setLinkUsed(bool used) { linkUsed = used; }
    int getFreeSpace() const { return size - occupied; }
    bool canInsert(int flits) const { return (occupied + flits) <= size; }
    void insert(int flits);
    void release(int flits);

private:
    int size;        // 总容量（以flit为单位）
    int occupied;    // 已占用容量
    bool linkUsed;   // 物理链路是否被占用
};

#endif