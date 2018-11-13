#include "logger.hpp"
#include "timer.hpp"
#include "Asserts.hpp"
#include "cfgSystem.hpp"

namespace ne_laz {
    // for block recursive call
    bool isLoggerCreated = true;
}

//TODO: create directory if not exist
Logger::Logger() {
    ne_laz::isLoggerCreated = false;

    auto time = zog::timer().getSystemDateTime();
    auto path = ConfigManager::readPath("logs");
    char filename[256];

    sprintf(&filename[0], "%02i-%02i-%04i %02i-%02i-%02i.log",
            time.day, time.month, time.year,
            time.hour, time.min, time.sec);

    if (!std::experimental::filesystem::exists(path))
        std::experimental::filesystem::create_directories(path);

    path /= filename;
    _path = path.string();

    R_ASSERTF((file = fopen(path.c_str(), "w")) != nullptr , "can't create log file in \"%s\"", path.string().c_str());

    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LOG START ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    ne_laz::isLoggerCreated = true;
}

Logger::~Logger() {
    auto time = zog::timer().getSystemDateTime();

    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LOG DESTROY [%02i:%02i:%02i]~~~~~~~~~~~~~~~~~~~~~~~~~~~~", time.hour, time.min, time.sec);

    fclose(file);
    ne_laz::isLoggerCreated = false;
}

void Logger::print(const char* format, ...) const {
    auto time = zog::timer().getSystemDateTime();
    fprintf(file, "[%02i:%02i:%02i.%03i]: ", time.hour, time.min, time.sec, time.ms);
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
    fprintf(file, "\n");

    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void Logger::close() {
    fclose(file);
}

void Logger::open() {
    file = fopen(_path.c_str(), "a");
}

void Logger::reopen() {
    fclose(file);
    file = fopen(_path.c_str(), "a");
}