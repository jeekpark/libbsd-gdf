#pragma once

#include <BSD-GDF/Config.hpp>

namespace gdf
{

class KernelEvent
{
public:
    KernelEvent();
    virtual ~KernelEvent();
    KernelEvent(const KernelEvent& IN copy);
    KernelEvent& operator=(const KernelEvent& IN copy);
    bool IdentifyFD(const int32 IN fd) const;
    bool IdentifySocket(const int32 IN socket) const;
    bool IsReadType() const;
    bool IsWriteType() const;

    uint64 GetIdentifier() const;
    int16 GetFilter() const;
    uint16 GetFlags() const;
    uint32 GetFilterFlags() const;
    int64 GetData() const;
    const void* GetUserData() const;

    void SetIdent(const uint64 IN ident);
    void SetFilter(const int16 IN filter);
    void SetFlags(const uint16 IN flags);
    void SetFilterFlags(const uint32 IN fflags);
    void SetData(const int64 IN data);
    void SetUserData(void* IN udata);

private:
    uint64 mIdentifier;
    int16 mFilter;
    uint16 mFlags;
    uint32 mFilterFlags;
    int64 mData;
    void* mUserData;
};

}