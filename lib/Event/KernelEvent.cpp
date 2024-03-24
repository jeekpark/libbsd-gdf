#include <BSD-GDF/Event/KernelEvent.hpp>

#include <sys/event.h>

namespace gdf
{

KernelEvent::KernelEvent()
: mIdentifier(0)
, mFilter(0)
, mFlags(0)
, mFilterFlags(0)
, mData(0)
, mUserData(0)
{
}

KernelEvent::~KernelEvent()
{
}

KernelEvent::KernelEvent(const KernelEvent& IN copy)
: mIdentifier(copy.mIdentifier)
, mFilter(copy.mFilter)
, mFlags(copy.mFlags)
, mFilterFlags(copy.mFilterFlags)
, mData(copy.mData)
, mUserData(copy.mUserData)
{
}

KernelEvent& KernelEvent::operator=(const KernelEvent& IN copy)
{
    if (this != &copy)
    {
        mIdentifier = copy.mIdentifier;
        mFilter = copy.mFilter;
        mFlags = copy.mFlags;
        mFilterFlags = copy.mFilterFlags;
        mData = copy.mData;
        mUserData = copy.mUserData;
    }
    return *this;
}

bool KernelEvent::IdentifyFD(const int32 IN fd) const
{
    if (mIdentifier == static_cast<uint64>(fd))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool KernelEvent::IdentifySocket(const int32 IN socket) const
{
    if (mIdentifier == static_cast<uint64>(socket))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool KernelEvent::IsReadType() const
{
    return mFilter == EVFILT_READ;
}

bool KernelEvent::IsWriteType() const
{
    return mFilter == EVFILT_WRITE;
}

uint64 KernelEvent::GetIdentifier() const
{
    return mIdentifier;
}

int16 KernelEvent::GetFilter() const
{
    return mFilter;
}

uint16 KernelEvent::GetFlags() const
{
    return mFlags;
}

uint32 KernelEvent::GetFilterFlags() const
{
    return mFilterFlags;
}

int64 KernelEvent::GetData() const
{
    return mData;
}

const void* KernelEvent::GetUserData() const
{
    return mUserData;
}

void KernelEvent::SetIdent(const uint64 IN ident)
{
    mIdentifier = ident;
}

void KernelEvent::SetFilter(const int16 IN filter)
{
    mFilter = filter;
}

void KernelEvent::SetFlags(const uint16 IN flags)
{
    mFlags = flags;
}

void KernelEvent::SetFilterFlags(const uint32 IN fflags)
{
    mFilterFlags = fflags;
}

void KernelEvent::SetData(const int64 IN data)
{
    mData = data;
}

void KernelEvent::SetUserData(void* IN udata)
{
    mUserData = udata;
}

} // namespace gdf
