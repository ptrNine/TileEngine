#ifndef __LOGGERH__
#define __LOGGERH__
#include <fstream>

namespace ne_laz {
    extern bool isLoggerCreated;
}

class Logger {
public:
    FILE*			file;
    std::string 	_path;

private:
    Logger();
    ~Logger();
public:
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

public:
    static Logger& _instance() { static Logger instance; return instance; }

public:
    void print(const char* format, ...) const;
    void close();
    void open();
    void reopen();
};

#define Log(format, ...)	if(ne_laz::isLoggerCreated) Logger::_instance().print(format, ##__VA_ARGS__)

#define LogWrite			if(ne_laz::isLoggerCreated) Logger::_instance().reopen

#define LogClose			if(ne_laz::isLoggerCreated) Logger::_instance().close
#define LogOpen				if(ne_laz::isLoggerCreated) Logger::_instance().open


#ifdef DEBUG
    #define DLog(format, ...) Log(format, __VA_ARGS__)
#else
    #define DLog(format, ...) ((void)0)
#endif // NDEBUG


#endif // !__LOGGERH__