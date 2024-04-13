/**
 * @file GlobalLogger.hpp
 * @author Jeekun Park (jeekunp@naver.com)
 * @brief 전역 로깅 시스템 클래스를 정의한 헤더
 * @version 0.1
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <unistd.h>
#include <pthread.h>

/**
 * @brief 컴파일러가 Clang이나 GCC가 아닐 경우 __PRETTY_FUNCTION__ 매크로를 __FUNCTION__으로 정의
 */
#if !defined(__clang__) && !defined(__GNUC__)
    #define __PRETTY_FUNCTION__ __FUNCTION__
#endif

/**
 * @brief 로그를 출력하는 매크로
 *
 * 타겟으로 로그가 출력 된다.\n
 * 사용예: LOG(LogLevel::Error) << "메세지";
 * 
 * @param level 해당 로그의 레벨
 */
#define LOG(level) GlobalLogger::LogStream(level, __PRETTY_FUNCTION__, __FILE__, __LINE__)

/**
 * @brief 로그의 출력 대상을 설정하는 메크로
 * 
 * fd를 지정하여 해당 파일 디스크립터로 로그 대상을 지정할 수 있다.\n
 * std::string 또한 로그 대상으로 지정할 수 있다.\n
 * std::string의 경우 append를 통해 로그를 이어 붙인다.
 * 기본값은 STDOUT fd로 설정됨.(따로 설정하지 않을 시)
 *
 * @param target 로그 메세지를 출력할 대상 (fd 또는 std::string)
 */
#define LOG_SET_TARGET(target) GlobalLogger::GetInstance().SetTarget(target)

/**
 * @brief 출력할 로그의 레벨 제한을 지정한다.
 *
 * 이 매크로를 통해 기록할 최소 메크로 레벨을 지정할 수 있다.\n
 * 예를 들어 Error 레벨로 지정한다면 Error 레벨 이하의\n
 * Informational과 같은 로그는 출력되지 않는다.
 * 기본값은 Informational로 설정됨.(따로 설정하지 않을 시)
 *
 * @param level 제한할 로그 레벨
 */
#define LOG_SET_LEVEL(level) GlobalLogger::GetInstance().SetLevel(level)

/**
 * @brief 전역으로 사용할 수 있는 로깅시스템 클래스이다.
 * 어플리케이션의 모든 로깅을 담당한다.
 * 싱글톤 패턴을 사용하여 인스턴스 관리를 한다.
 */
class GlobalLogger
{
public:

    /**
     * @enum eServerityLevel
     * @brief 로그의 심각도 수준을 나타내는 열거형
     *
     * 각 레벨은 로그의 심각도 및 우선순위를 나타냅니다.
     */
    enum eSeverityLevel
    {
        Emergency = 0,  // 시스템 사용이 불가능
        Alert,          // 즉시 조치가 필요
        Critical,       // 중대한 상황
        Error,          // 오류 상황
        Warning,        // 경고 상황
        Notice,         // 확인해야할, 혹은 알아야할 정보
        Informational,  // 일반 정보 메세지
        Debug,          // 디버그 레벨 메세지, 해당 레벨에서는 파일이름, 라인, 함수명이 출력된다.
    };

    /**
     * @brief GlobalLogger의 기본 생성자
     */
    GlobalLogger();

    /**
     * @brief GlobalLogger의 소멸자
     */
    ~GlobalLogger();

    /**
     * @brief GlobalLogger의 인스턴스를 반환하는 정적 메소드
     * 
     * 싱글턴 인스턴스에 접근하기 위한 메소드로, 해당 클래스의 유일한 인스턴스를 반환한다
     *
     * @return GlobalLogger& 로거의 유일한 인스턴스를 반환
     */
    static GlobalLogger& GetInstance();

    /**
     * @brief 로그 메세지를 기록
     * 
     * 입력된 메세지와 함꼐 로그를 기록한다.\n
     * 로그 레벨과 발생위치, 호스트이름도 함께 기록 된다.
     *
     * @param level 로그의 심각도 레벨
     * @param message 로그로 기록할 메세지
     * @param functionName 로그가 발생한 함수의 이름 (Debug 레벨만)
     * @param fileName 로그가 발생한 파일의 이름 (Debug 레벨만)
     * @param lineNumber 로그가 발생한 라인 번호 (Debug 레벨만)
     */
    void Log(eSeverityLevel level, const std::string& message,
             const char* functionName, const char* fileName,
             const int lineNumber);

    /**
     * @brief 로그 메세지의 출력 대상을 std::string으로 지정
     * 
     * 로그 메시지를 저장할 문자열 객체의 참조를 설정한다.\n
     * 이 방식을 사용하면 로그 메시지가 해당 std::string에 append된다.
     *
     * @param str 로그 메세지를 받을 std::stringd의 참조
     */
    void SetTarget(std::string& str);

    /**
     * @brief 로그 메세지의 출력 대상을 fd로 지정
     * 
     * 로그 메시지를 출력할 파일 디스크립터를 설정한다.\n
     * 이 설정을 통해 로그 메시지를 특정 파일이나 장치에 직접 출력할 수 있다.
     * 시스템 콜을 이용하기 때문에 속도가 느리다.
     *
     * @param str 로그 메세지를 받을 파일 디스크립터
     */
    void SetTarget(int fd);

    /**
     * @brief 출력할 로그의 레벨 제한을 지정한다.
     *
     * 이 메소드를 통해 기록할 최소 메크로 레벨을 지정할 수 있다.\n
     * 예를 들어 Error 레벨로 지정한다면 Error 레벨 이하의\n
     * Informational과 같은 로그는 출력되지 않는다.
     * 기본값은 Informational로 설정됨.(따로 설정하지 않을 시)
     *
     * @param level 제한할 로그 레벨
     */
    void SetLevel(eSeverityLevel level);

    /**
     * @class LogStream
     * @brief 로그 메세지를 스트림으로 받는 클래스
     * 
     * 이 클래스는 RAII 패턴으로 디자인되어 생성과 동시에 소멸을 통해 Log를 생성한다.\n
     * 연산자 오버로딩(`operator<<`)을 통해 다양한 유형의 데이터를 쉽게 로그 스트림에 추가할 수 있다.\n
     * `LOG()` 매크로를 사용하면 이 클래스의 인스턴스가 생성되며,\n
     * 스트림 연산을 통해 로그 메시지를 구성할 수 있다.
     */
    class LogStream
    {
    public:
        
        /**
         * @brief LogStream의 기본 생성자
         * 
         * 로그 메시지의 상세 정보(로그 레벨, 함수 이름, 파일 이름, 라인 번호)와 함께 로그 스트림을 초기화한다.
         *
         * @param level 로그의 심각도 
         * @param functionName 로그가 발생한 함수의 이름
         * @param fileName 로그가 발생한 파일의 이름
         * @param lineNumber 로그가 발생한 코드의 라인 번호
         */
        LogStream(eSeverityLevel level, const char* functionName,
                  const char* fileName, const int lineNumber);
        
        /**
         * @brief LogStream의 소멸자
         * 
         * 스트림에 저장된 로그 메세지를 최종적으로 처리하고 로그를 완료한다.
         */
        ~LogStream();

        /**
         * @brief 스트림에 메세지를 추가하는 연산자 오버로드
         * 
         * 이 메소드를 통해 다양한 로그 스트림에 대한 데이터 타입의 메세지를 추가할 수 있다.
         *
         * @tparam T 데이터 타입
         * @param message 로그 스트림에 추가할 메세지
         * @return LogStream& 로그 스트림 객체의 참조를 반환하여 연속 체이닝이 가능하게 한다.
         */
        template <typename T>
        LogStream& operator<<(const T& message)
        {
            mStream << message;
            return *this;
        }
    private:
        eSeverityLevel mLevel;
        const char* mFunctionName;
        const char* mFileName;
        const int mLineNumber;
        std::ostringstream mStream;
    };

private:
    GlobalLogger(const GlobalLogger&);              // = delete
    GlobalLogger& operator=(const GlobalLogger&);   // = delete
    std::string getCurrentTime();

private:
    bool bIsStringTarget;
    std::string* mStringTarget;
    int mFDTarget;
    eSeverityLevel mLevel;
    std::vector<std::string> mLevelStr;
    pthread_mutex_t mFileMutex;
    char mHostname[256];
};

/**
 * @class LogLevel
 * @brief GlobalLogger 클래스에 대한 별칭을 제공하는 클래스
 * 
 * 이 클래스는 GlobalLogger의 기능을 그대로 상속받으면서\n
 * 로그 레벨을 좀 더 명시적으로 표현하기 위해 사용돤다.\n
 * 주로 로그 레벨의 가독성을 높인다.\n
 * 이 클래스는 GlobalLogger의 모든 기능을 포함하며, 추가적인 기능은 제공하지 않는다.
 */
class LogLevel : public GlobalLogger
{};
