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

/**
 * @brief 커널 이벤트 큐를 관리하는 클래스이다.
 * 이 클래스는 시스템 커널 레벨의 이벤트 핸들링을 위해 kqueue를 사용한다.
 */
class KernelQueue
{
public:

    /**
     * @brief KernelQueue의 기본 생성자
     */
    KernelQueue();

    /**
     * @brief KernelQueue의 가상소멸자
     */
    virtual ~KernelQueue();

    /**
     * @brief 이벤트 큐를 초기화한다.
     * 
     * @return true 성공시
     * @return false 실패시
     */
    bool Init();

    /**
     * @brief 읽기 이벤트를 추가하여 해당 fd를 감시한다.
     * 
     * @param fd 감시할 파일 디스크립터
     * @return true 성공시
     * @return false 실패시
     */
    bool AddReadEvent(const int32 fd);

    /**
     * @brief 쓰기 이벤트를 추가하여 해당 fd를 감시한다.
     * 
     * @param fd 감시할 파일 디스크립터
     * @return true 성공시
     * @return false 실패시
     */
    bool AddWriteEvent(const int32 fd);
    
    /**
     * @brief 이벤트 큐를 폴링하고 다음 이벤트를 반환한다.
     * 
     * @param event 반환할 KernelEvent 객체의 참조
     * @return true 이벤트가 존재할 경우
     * @return false 이벤트가 존재하지 않을 경우
     */
    bool Poll(KernelEvent& event);

    /**
     * @brief 이벤트가 발생할 때까지 함수가 얼마나 대기할지 지정한다.
     * 
     * @param ms 밀리초 단위의 타임아웃 시간
     */
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
