#ifndef L_H
#define L_H

#include <fstream>

using std::ofstream;

#ifdef LOGGABLE
    class Logger {
    public:
        static Logger instance;
        template <typename T>
        Logger& operator<<(const T& p) {
            if (loggerStream.is_open()) {
                loggerStream << p;
                loggerStream.flush();
            }
            return *this;
        }
        ~Logger() {
            if (loggerStream.is_open()) {
                loggerStream.close();
            }
        }
    private:
        ofstream loggerStream;
        Logger() {
            loggerStream = ofstream("bstbox.log", std::ios::app);
        }
    };
    #define logger Logger::instance
#else
    class Logger {
    public:
        static Logger instance;
        template <typename T>
        Logger& operator<<(const T&) noexcept { return *this; }
    private:
        Logger() = default;
    };
    #define logger Logger::instance
#endif

#endif