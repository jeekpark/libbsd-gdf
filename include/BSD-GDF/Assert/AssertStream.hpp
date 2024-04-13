#pragma once

#include <sstream>
#include <iostream>

namespace gdf
{

/**
 * @class AssertStream
 * @brief 조건 검증을 위한 스트림 클래스 (RAII 패턴으로 메소드 처럼 사용)
 * 
 * 이 클래스는 조건이 거짓일 경우 메세지와 함께 abort로 프로세스를 종료시키는 용도로 사용된다.\n
 * 개발 중 버그를 찾거나 예상치 못한 조건을 검증하는데 사용된다.\n
 * 스트림을 통해 추가적인 정보를 받아 로그로 기록할 수 있다.\n
 * NDEBUG를 정의할 경우 해당 클래스가 호출된 라인은 컴파일 되지 않는다.(릴리즈용)
 */
class AssertStream
{
public:
    /**
     * @brief AssertStream의 기본 생성자
     * 
     * 조건, 표현식 리터럴, 함수 이름, 파일 이름을 받아 초기화한다.\n
     * 조건이 거짓일 경우, 제공된 정보와 함께 로그를 출력한다.
     *
     * @param condition 검증할 조건
     * @param expressionString 표현식 리터럴
     * @param functionName 로그가 발생한 함수의 이름
     * @param fileName 로그가 발생한 파일의 이름
     * @param lineNumber 로그가 발생한 코드의 라인 번호
     */
    AssertStream(bool condition, const char* expressionString,
                 const char* functionName, const char* fileName,
                 const int lineNumber);
    
    /**
     * @brief AssertStream의 소멸자
     * 소멸 시 조건이 거짓이면 스트림에 입력된 메세지와 함께 에러 정보를 출력한다.
     */
    ~AssertStream();

    /**
     * @brief 스트림에 메세지를 추가하는 연산자 오버로드
     * 
     * 이 메소드를 통해 다양한 어썰트 스트림에 대한 데이터 타입의 메세지를 추가할 수 있다.
     *
     * @tparam T 데이터 타입
     * @param message 어썰트 스트림에 추가할 메세지
     * @return AssertStream& 어썰트 스트림 객체의 참조를 반환하여 연속 체이닝이 가능하게 한다.
     */
    template<typename T>
    AssertStream& operator<<(const T& message)
    {
        mStream << message;
        return *this;
    }

private:
    bool bCondition;
    const char* mExpressionString;
    const char* mFunctionName;
    const char* mFileName;
    const int mLineNumber;
    std::ostringstream mStream;
};

} // namespace gdf
