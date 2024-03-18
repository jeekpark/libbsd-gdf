#include "BSD-GDF/Assert.hpp"
#include "BSD-GDF/Logger.hpp"

int main()
{
    LOG(LogLevel::Informational) << "hello";
    ASSERT(1 != 1) << "hello";
}