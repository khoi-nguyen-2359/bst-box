#ifndef L_H
#define L_H

#include <fstream>

using std::ofstream;

#ifdef LOGGABLE
    class Logger {
    public:
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }

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

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
    private:
        ofstream loggerStream;
        Logger() {
            loggerStream.open("bstbox.log", std::ios::app);
        }
    };
#else
    class Logger {
    public:
        static Logger& getInstance() {
            static Logger instance;
            return instance;
        }
        template <typename T>
        Logger& operator<<(const T&) noexcept { return *this; }
    };
#endif

#define logger Logger::getInstance()

#endif