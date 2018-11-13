#ifndef ASSERTS_HPP
#define ASSERTS_HPP
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include "engineDefines.hpp"
#include "logger.hpp"


#ifdef NDEBUG 
#define ASSERT(expr, msg) ((void)0)
#define ASSERTF(expr, format, ...) ((void)0)
#else
#define ASSERT(expr, msg) AssertTools::_msg_assert(expr, msg, __LINE__, __FILE__, __func__)
#define ASSERTF(expr, format, ...)		AssertTools::varargAssert(expr, __LINE__, __FILE__, __func__, format, ##__VA_ARGS__);
#endif

#define R_ASSERT(expr, msg)		AssertTools::_msg_assert(expr, msg, __LINE__, __FILE__, __func__)
#define R_ASSERTF(expr, format, ...)	AssertTools::varargAssert(expr, __LINE__, __FILE__, __func__, format, ##__VA_ARGS__);


class AssertTools {

public:

    static const unsigned ASSERT_BUFF_SIZE = 1024 * 4;

    static void varargAssert(bool expr, const int line, const char* file, const char* func, const char* format, ...) {

        if (!expr) {
            va_list args;

            va_start(args, format);
            char buffer[ASSERT_BUFF_SIZE];
            vsprintf(buffer, format, args);
            va_end(args);

            Log("\n\nFATAL ERROR:\n file:     %s\n function: %s\n line:     %d\n what:     %s\n ", file, func, line, buffer);
            abort();
        }
    }


    inline static void _msg_assert(bool expr) {

        abort();

    }


    inline static void _msg_assert(bool expr, const char* msg, const int line, const char* file, const char* func) {

        if (!expr) {
            printf("FATAL ERROR: %s\n file: %s\n line: %d\n", msg, file, line);
            Log("\n\nFATAL ERROR:\n file:     %s\n function: %s\n line:     %d\n what:     %s\n ", file, func, line, msg);
            abort();
        }
    }

};


#endif // !ASSERTS_HPP