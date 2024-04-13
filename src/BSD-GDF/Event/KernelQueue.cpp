#include "BSD-GDF/Event/KernelQueue.hpp"

#include "BSD-GDF/Event/KernelEvent.hpp"

namespace gdf
{

KernelQueue::KernelQueue()
: mKqueue(ERROR)
, mEventList(NULL)
, mEventCount(0)
, mEventIndex(0)
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

bool KernelQueue::AddReadEvent(const int32 fd)
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

bool KernelQueue::AddWriteEvent(const int32 fd)
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




bool KernelQueue::Poll(KernelEvent& event)
{
    if (mEventCount == mEventIndex)
    {
        getEventList();
        if (mEventCount == ERROR)
        {
            mEventCount = 0;
        }
        mEventIndex = 0;
        return false;
    }
    else
    {
        const struct kevent* current = &mEventList[mEventIndex];
        event.SetIdent(current->ident);
        event.SetFilter(current->filter);
        event.SetFlags(current->flags);
        event.SetFilterFlags(current->fflags);
        event.SetData(current->data);
        event.SetUserData(current->udata);
        ++mEventIndex;
        return true;
    }
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

const struct kevent* KernelQueue::getEventList()
{
    mEventCount = kevent(mKqueue, NULL, 0,
                         mEventList, MAX_KEVENT_SIZE, &mTimeout);
    if (mEventCount == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to generate Event list (errno:" << errno << " - "
            << strerror(errno) << ") on kevent()";
        return NULL;
    }
    return mEventList;
}

void KernelQueue::SetTimeout(const int64 ms)
{
    mTimeout.tv_sec = ms / 1000;
    mTimeout.tv_nsec = (ms % 1000) * 1000 * 1000;
}

}
