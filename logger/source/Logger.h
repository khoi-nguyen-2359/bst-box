#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

using std::cout;

class Logger {
    public:
        Logger operator<<(const char* message);
};

#endif