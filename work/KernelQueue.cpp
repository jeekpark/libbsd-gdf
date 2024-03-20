#include "./KernelQueue.hpp"
#include <cstddef>
#include <cstring>
#include <sys/event.h>

namespace gdf
{

KernelQueue::KernelQueue()
: mKqueue(ERROR)
, mEventList(NULL)
, mEventListIndex(0)
, mEventCount(0)
{
    SetTimeout(5);
}

KernelQueue::~KernelQueue()
{
    close(mKqueue);
    delete [] mEventList;
}

bool KernelQueue::Init()
{
    mEventList = new (std::nothrow) struct kevent[MAX_KEVENT_SIZE];
    if (mEventList == NULL)
    {
        return FAILURE;
    }
    std::memset(mEventList, 0, sizeof(struct kevent) * MAX_KEVENT_SIZE);
    if (createKqueue() == FAILURE)
    {
        return FAILURE;
    }
    return SUCCESS;
}

bool KernelQueue::AddReadEvent(const int32 IN fd)
{
    struct kevent newEvent;
    EV_SET(&newEvent, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(mKqueue, &newEvent, 1, NULL, 0, NULL) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to add READ event(errno:" << errno << " - "
            << strerror(errno) << ") on kevent()";
        return FAILURE;
    }
    return SUCCESS;
}

bool KernelQueue::AddWriteEvent(const int32 IN fd)
{
    struct kevent newEvent;
    EV_SET(&newEvent, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
    if (kevent(mKqueue, &newEvent, 1, NULL, 0, NULL) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to add WRITE event(errno:" << errno << " - "
            << strerror(errno) << ") on kevent()";
        return FAILURE;
    }
    return SUCCESS;
}

bool KernelQueue::poll(KernelEvent& OUT event)
{
    if (mEventCount == mEventListIndex)
    {
        mEventCount = kevent(mKqueue, NULL, 0, mEventList, MAX_KEVENT_SIZE, &mTimeout);
        if (mEventCount == ERROR)
        {
            LOG(LogLevel::Error) << "Failed to generate Event list (errno:" << errno << " - "
                << strerror(errno) << ") on kevent()";
            event.fd = -1;
            event.type = -1;
            return false;
        }
        mEventListIndex = 0;
    }
    event.fd = -1;
    event.type = -1;
    if (mEventCount == 0)
        return false;
    const struct kevent* c_event = &mEventList[mEventListIndex++];
    event.type = c_event->filter;
    event.fd = c_event->ident;
    return true;
    

}

bool KernelQueue::createKqueue()
{
    mKqueue = kqueue();
    if (mKqueue == ERROR)
    {
        LOG(LogLevel::Error) << "Faild to excute Kqueue (errno:" << errno << " - "
            << strerror(errno) << ") on kqueue()";
        return FAILURE;
    }
    return SUCCESS;
}

void KernelQueue::SetTimeout(const int64 ms)
{
    mTimeout.tv_sec = ms / 1000;
    mTimeout.tv_nsec = (ms % 1000) * 1000 * 1000;
}

}