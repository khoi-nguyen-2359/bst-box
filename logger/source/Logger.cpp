#include "Logger.h"

Logger Logger::operator<<(const char* message) {
    #ifdef LOGGER_ON
        cout << message;
    #endif
    return *this;
}