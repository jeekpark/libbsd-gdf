/**
 * @file Event.hpp
 * @author Taeil-Nam (nam0314@gmail.com)
 * @brief IO event와 관련된 역할을 수행하는 클래스
 * @version 0.1
 * @date 2024-02-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <new>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <sys/event.h>

#include <BSD-GDF/Config.hpp>
#include <BSD-GDF/Logger.hpp>

namespace gdf
{

class KernelEvent;

class KernelQueue
{
public:
    KernelQueue();
    virtual ~KernelQueue();

    bool Init();
    bool AddReadEvent(const int32 fd);
    bool AddWriteEvent(const int32 fd);
    
    bool Poll(KernelEvent& event);
    void SetTimeout(const int64 ms);
private:
    KernelQueue(const KernelQueue& event); // = delete
    const KernelQueue& operator=(const KernelQueue& event); // = delete
    
    bool createKqueue();
    const struct kevent* getEventList();
private:
    enum { MAX_KEVENT_SIZE = 128 };
    int32 mKqueue;
    struct kevent* mEventList;
    int32 mEventCount;
    int32 mEventIndex;
    struct timespec mTimeout;
};
 
}
