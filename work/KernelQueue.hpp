#pragma once

#include <new>
#include <unistd.h>
#include <sys/event.h>

#include "../include/BSD-GDF/Config.hpp"

namespace gdf
{

class KernelEvent
{
public:
    int32 fd;
    int32 type;
};

class KernelQueue
{
public:
    KernelQueue();
    virtual ~KernelQueue();

    bool Init();
    bool AddReadEvent(const int32 IN fd);
    bool AddWriteEvent(const int32 IN fd);
    bool poll(KernelEvent& OUT event);
    void SetTimeout(const int64 IN ms);

private:
    KernelQueue(const KernelQueue&);
    KernelQueue& operator=(const KernelQueue&);

    bool createKqueue();
private:
    enum { MAX_KEVENT_SIZE = 128 };
    int32 mKqueue;
    struct kevent* mEventList;
    uint64 mEventListIndex;
    int32 mEventCount;
    struct timespec mTimeout;
};

}

