/**
 * @file DisplayBuffer.hpp
 * @author Jeekun Park (jeekunp@naver.com)
 * @brief 디스플레이를 위한 버퍼 클래스를 정의하는 헤더
 * @version 0.1
 * @date 2024-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <ctime>
#include <iostream>
#include <deque>
#include <sstream>
#include <iomanip>

#include "../Config.hpp"

namespace gdf
{

/**
 * @class DisplayBuffer
 * @brief 디스플레이에 표시되어야하는 텍스트를 저장하는 클래스
 * 
 * 텍스트 콘텐츠를 색상, 시간과 함께 저장하고, 버퍼에 저장된 컨텐츠를 관리한다.
 * 헤더와 푸터를 설정할 수 있으며, 버퍼 용량도 설정 가능하다.
 */
class DisplayBuffer
{
public:

    /**
     * @enum eColor
     * @brief 텍스트 컨텐츠의 색상을 정의하는 열거형
     */
    enum eColor
    {
        Default = 0,
        Cyan,
        Red,
        Green,
        BrightBlue,
        WhiteCharRedBG,
        WhiteCharBlueBG,
        WhiteCharGrayBG,
    };

    /**
     * @struct Content
     * @brief 텍스트 컨텐츠를 색상, 시간 정보와 함께 저장하는 구조체
     */
    struct Content
    {
        std::string TimeStamp;
        std::string String;
        eColor Color;
    };

    /**
     * @brief DisplayBuffer 클래스의 기본 생성자
     * 
     * DisplayBuffer 객체를 초기화한다.\n
     * 헤더 기본값: "HEADER"\n
     * 푸터 기본값: "FOOTHER"\n
     * 버퍼 크기 기본값: 1024
     */
    DisplayBuffer();

    /**
     * @brief DisplayBuffer의 가상 소멸자
     * 
     * 가상 소멸자를 제공하여, DisplayBuffer가 상속될 경우 파생 클래시의 소멸자도 올바르게 호출된다.
     */
    virtual ~DisplayBuffer();

    /**
     * @brief DisplayBuffer의 복사 생성자
     * 
     * 모든 컨텐츠와 설정을 복사한다.
     *
     * @param copy 복사할 DisplayBuffer 객체 참조
     */
    DisplayBuffer(const DisplayBuffer& copy);

    /**
     * @brief DisplayBuffer의 복사 대입 연산자 오버로드
     * 
     * 모든 컨텐츠와 설정을 복사한다.
     *
     * @param copy 복사할 DisplayBuffer 객체 참조
     * @return DisplayBuffer& 자기 자신의 참조를 반환
     */
    DisplayBuffer& operator=(const DisplayBuffer& IN copy);

    /**
     * @brief 버퍼의 모든 컨텐츠를 지운다.
     */
    void Clear();

    /**
     * @brief 새로운 텍스트 컨텐츠를 버퍼에 추가한다.
     * 
     * @param str 추가할 텍스트 컨텐츠
     * @param color 텍스트의 색상
     */
    void PushContent(const std::string& IN str, DisplayBuffer::eColor IN color);

    /**
     * @brief 현재 컨텐츠 버퍼를 반환한다.
     * 
     * @return const std::deque<DisplayBuffer::Content>& 현재 컨텐츠 버퍼의 상수 참조
     */
    const std::deque<DisplayBuffer::Content>& GetContentBuffer() const;

    /**
     * @brief 현재 설정된 헤더 문자열을 반환한다.
     * 
     * @return const std::string& 현제 헤더 문자열의 상수 참조
     */
    const std::string& GetHeader() const;

    /**
     * @brief 현재 설정된 푸터 문자열을 반환한다.
     * 
     * @return const std::string& 현제 푸터 문자열의 상수 참조
     */
    const std::string& GetFooter() const;

    /**
     * @brief 헤더 문자열을 설정한다.
     * 
     * @param str 새로운 헤더 문자열
     */
    void SetHeader(const std::string& IN str);

    /**
     * @brief 푸터 문자열을 설정한다.
     * 
     * @param str 새로운 푸터 문자열
     */
    void SetFooter(const std::string& IN str);

    /**
     * @brief 버퍼의 용량을 설정한다,
     *
     * 저장된 용량을 버퍼의 최대 콘텐츠 저장 가능 수로 설정한다.
     * 이는 버퍼의 크기를 관리하는 데 사용된다.
     * 
     * @param capacity 새로운 버퍼의 용량
     */
    void SetContentBufferCapacity(const uint64 IN capacity);

private:
    void getCurrentTimeString(std::string& OUT str);

private:
    std::string mHeader;
    std::string mFooter;
    uint64 mContentBufferCapacity;
    std::deque<Content> mContentBuffer;
    
};

} // namespace gdf
