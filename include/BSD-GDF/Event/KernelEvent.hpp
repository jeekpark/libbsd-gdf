/**
 * @file KernelEvent.hpp
 * @author Jeekun Park (jeekunp@naver.com)
 * @brief 시스템의 커널 이벤트를 관리하는 클래스를 정의
 * @version 0.1
 * @date 2024-04-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <sys/event.h>
#include <BSD-GDF/Config.hpp>

namespace gdf
{

/**
 * @brief BSD계열 유닉스 커널의 이벤트를 추상화한 클래스
 * 시스템 내부의 다양한 이벤트(FD,Socket)을 식별하고 이벤트 유형을 파악하는 관리 기능을 제공한다.
 */
class KernelEvent
{
public:

    /**
     * @brief KernelEvent의 기본 생성자
     */
    KernelEvent();

    /**
     * @brief KernelEvent의 가상 소멸자
     */
    virtual ~KernelEvent();

    /**
     * @brief KernelEvent의 복사 생성자
     * @param copy 복사할 객체의 참조
     */
    KernelEvent(const KernelEvent& IN copy);

    /**
     * @brief KernelEvent의 복사 대입 연산자
     * @param copy 복사할 객체의 참조
     * @return KernelEvent& 업데이트된 객체의 참조를 반환
     */
    KernelEvent& operator=(const KernelEvent& IN copy);

    /**
     * @brief 주어진 파일 디스크립터가 이 이벤트와 연관되어 있는지 식별한다.
     * @param fd 검사할 파일 디스크립터
     * @return true 연관되어있을 시
     * @return false 연관되어있지 않을 시
     */
    bool IdentifyFD(const int32 IN fd) const;

    /**
     * @brief 주어진 소켓이 이 이벤트와 연관되어 있는지 식별한다.
     * @param fd 검사할 소켓
     * @return true 연관되어있을 시
     * @return false 연관되어있지 않을 시
     */
    bool IdentifySocket(const int32 IN socket) const;

    /**
     * @brief 이 이벤트가 읽기 유형의 이벤트인지 식별한다.
     * @return true 읽기 유형이라면
     * @return false 읽기 유형이 아니라면
     */
    bool IsReadType() const;

    /**
     * @brief 이 이벤트가 쓰기 유형의 이벤트인지 식별한다.
     * @return true 쓰기 유형일마면
     * @return false 쓰기 유형이 아니라면
     */
    bool IsWriteType() const;

    /**
     * @brief 이벤트의 식별자를 반환한다.
     * @return uint64 이벤트의 식별자
     */
    uint64 GetIdentifier() const;

    /**
     * @brief 이벤트의 필터값을 반환한다.
     * @return int16 이벤트의 필터값
     */
    int16 GetFilter() const;

    /**
     * @brief 이벤트의 플래그값을 반환한다.
     * @return uint16 이벤트의 플래그값
     */
    uint16 GetFlags() const;

    /**
     * @brief 이벤트의 필터 플래그값을 반환한다.
     * @return uint32 이벤트의 필터 플래그값
     */
    uint32 GetFilterFlags() const;

    /**
     * @brief 필터에 의해 반환된 데이터를 반환한다.
     * @return int64 필터에 의해 반환된 데이터
     */
    int64 GetData() const;

    /**
     * @brief 사용자 정의 포인터를 반환한다.
     * @return const void* 사용자 정의 포인터
     */
    const void* GetUserData() const;

    /**
     * @brief 이벤트의 식별자를 설정한다.
     * @param ident 새로운 식별자
     */
    void SetIdent(const uint64 IN ident);

    /**
     * @brief 이벤트의 필터를 설정한다.
     * @param filter 새로운 필터값
     */
    void SetFilter(const int16 IN filter);

    /**
     * @brief 이벤트의 플래그를 설정한다.
     * @param flags 새로운 플래그값
     */
    void SetFlags(const uint16 IN flags);

    /**
     * @brief 이벤트의 필터 플래그를 설정한다.
     * @param fflags 새로운 필터 플래그값
     */
    void SetFilterFlags(const uint32 IN fflags);

    /**
     * @brief 필터에 연관된 데이터를 설정한다.
     * @param data 새로운 데이터
     */
    void SetData(const int64 IN data);

    /**
     * @brief 사용자 정의 포인터를 설정한다.
     * @param udata 사용자 정의 포인터
     */
    void SetUserData(void* IN udata);

private:
    uint64 mIdentifier;
    int16 mFilter;
    uint16 mFlags;
    uint32 mFilterFlags;
    int64 mData;
    void* mUserData;
};

} // namespace gdf
