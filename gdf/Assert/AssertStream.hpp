#pragma once

#include <sstream>
#include <iostream>

namespace gdf
{

class AssertStream
{
public:
    AssertStream(bool condition, const char* expressionString,
                 const char* functionName, const char* fileName,
                 const int lineNumber)
    : bCondition(condition)
    , mExpressionString(expressionString)
    , mFunctionName(functionName)
    , mFileName(fileName)
    , mLineNumber(lineNumber)
    {}
    ~AssertStream()
    {
        if (!bCondition)
        {
            std::cerr << "\n[Assertion failure]\n" 
                      << " - Assert:\t(" << mExpressionString << ")\n"
                      << (mStream.str().size() > 0 ? (" - Message:\t\"" + mStream.str() + "\"\n") : "")
                      << " - File:\t" << mFileName << ':' << mLineNumber <<  '\n'
                      << " - Function:\t" << mFunctionName << "\n\n"
                      << std::flush;
            std::abort();
        }
    }
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

}
