/**
 * @file DisplayConsole.hpp
 * @author Jeekun Park (jeekunp@naver.com)
 * @brief 콘솔 디스플레이를 정의하는 헤더
 * @version 0.1
 * @date 2024-04-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <deque>
#include <map>
#include <sstream>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#include "../Config.hpp"

#include "./DisplayBuffer.hpp"

namespace gdf
{

/**
 * @class DisplayConsole
 * @brief 콘솔화면에 텍스트와 프롬프트를 관리하고 출력하는 클래스
 * 
 * 이 클래스는 콘솔 화면에 텍스트 콘텐츠를 동적하고 관리한다.\n
 * 사용자는 헤더, 푸터, 본문 컨텐츠의 텍스트와 색상을 설정할 수 있다.\n
 */
class DisplayConsole
{
public:
    
    /**
     * @brief DisplayConsole의 기본 생성자
     * 
     * 콘솔을 위한 초기 헤더와 푸터를 설정할 수 있다.
     *
     * @param header 초기 헤더 텍스트 (기본값: "header")
     * @param footer 초기 푸터 텍스트 (기본값: "footer")
     * @param headerColor 헤더의 색상 (기본값: WhiteCharBlueBG)
     * @param footerColor 푸터의 색상 (기본값: WhiteCharBlueBG)
     */
    DisplayConsole(const std::string& IN header = "header",
                   const std::string& IN footer = "footer",
                   const DisplayBuffer::eColor IN headerColor = DisplayBuffer::WhiteCharBlueBG,
                   const DisplayBuffer::eColor IN footerColor = DisplayBuffer::WhiteCharBlueBG);
    
    /**
     * @brief DisplayConsole 클래스의 가상 소멸자
     * 
     * DisplayConsole 객체가 소멸될 때 필요한 정리 작업을 수행한다.
     */
    virtual ~DisplayConsole();
    
    /**
     * @brief 콘솔 화면을 갱신한다.
     * 
     * 해당 작업은 비동기 IO로 작동된다.
     * 현재 설정된 헤더, 푸터, 그리고 콘텐츠를 바탕으로 콘솔 화면을 새로고침한다.
     */
    void Refresh();

    /**
     * @brief 스크린의 갱신 상태를 설정한다.
     * 
     * @param status 스크린이 갱신되었는지 여부의 상태
     */
    void SetIsScreenUpdated(bool status);

    /**
     * @brief 문자를 프롬프트 문자열에 추가한다.
     * 
     * getchar()와 같은 함수로 문자를 하나 받아서 인자로 넣어주면 된다.
     *
     * @param ch 입력할 문자
     */
    void PushCharPrompt(const char IN ch);

    /**
     * @brief 프롬프트 큐에서 문자열을 폴링하여 가져온다.
     * 
     * @param prompt 폴링하여 가져온 문자열이 저장될 변수의 참조
     * @return bool 폴링 성공 여부
     */
    bool PollPromptQueue(std::string& OUT prompt);

    /**
     * @brief 콘텐츠를 콘솔 화면에 추가한다.
     * 
     * 주어진 텍스트와 색상을 사용하여 콘텐츠를 콘솔에 추가한다.
     * 
     * @param content 추가할 콘텐츠의 텍스트
     * @param color 콘텐츠의 텍스트 색상 (기본값: Default)
     */
    void PushContent(const std::string& IN content, DisplayBuffer::eColor IN color = DisplayBuffer::Default);
    
    /**
     * @brief 콘솔의 모든 콘텐츠를 지운다.
     */
    void ClearContent();

    /**
     * @brief 콘솔의 실패 상태를 반환한다.
     * 
     * @return bool 콘솔의 현재 실패 상태
     */
    bool IsFailed();

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
     * @brief 콘솔의 헤더 색상을 설정한다.
     * 
     * @param color 새로운 헤더 색상
     */
    void SetHeaderColor(const DisplayBuffer::eColor IN color);

    /**
     * @brief 콘솔의 푸터 색상을 설정한다.
     * 
     * @param color 새로운 푸터 색상
     */
    void SetFooterColor(const DisplayBuffer::eColor IN color);

    /**
     * @brief 타임스탬프 표시 여부를 설정한다.
     * 
     * @param enable 타임스탬프를 표시할지 여부
     */
    void SetTimestamp(const bool IN enable);
    
private: // function
    void setTerminalMode(const bool IN enable);
    bool updateConsoleSize();
    std::string cursorToLine(const int32 line) const;
    uint64 strlenMultiByte(const std::string& str) const;
    
    void renderScreenString(std::string& OUT screenBuffer);
    void appendHeader(std::string& OUT screenBuffer);
    void appendContent(std::string& OUT screenBuffer);
    void appendFooter(std::string& OUT screenBuffer);
    void appendPrompt(std::string& OUT screenBuffer);
private: // variables
    static uint64 sStaticInstanceCount;
    static struct termios sStaticOldTerminal;
    
    DisplayBuffer mDisplay;
    // general
    std::map<DisplayBuffer::eColor, std::string> mANSIColors;
    std::string mHeaderColor;
    std::string mFooterColor;
    bool bIsTimestampEnabled;
    bool bIsFailed;

    // input
    std::string mPromptBuffer;
    std::deque<std::string> mPromptQueue;

    // attr
    uint64 mConsoleWidth;
    uint64 mConsoleHeight;

    // screen buffer
    bool bIsScreenUpdated;
    std::string mScreenBuffer;
    uint64 mScreenBufferIndex;
    bool bIsScreenBufferRemain;
};

} // namespace gdf
