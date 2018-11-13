#ifndef _MATH_HPP_
#define _MATH_HPP_


#include <random>
#include "../engineDefines.hpp"
#include "../compileTime.hpp"
#include "../types.hpp"
#include "matrix.hpp"

#undef min
#undef max

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

#define CONST_PI		3.14159265

namespace ne_laz {
    extern std::mt19937 generatorU32;
}

namespace math {

    constexpr S8		abs(S8 a) { return a > 0 ? a : -a; }
    constexpr S16		abs(S16 a) { return a > 0 ? a : -a; }
    constexpr S32		abs(S32 a) { return a > 0 ? a : -a; }
    constexpr S64		abs(S64 a) { return a > 0 ? a : -a; }
    constexpr float		abs(float a) { return a > 0 ? a : -a; }
    constexpr double	abs(double a) { return a > 0 ? a : -a; }

    inline U32	rand(U32 min, U32 max) {
        std::uniform_int_distribution<> uid(min, max);
        return uid(ne_laz::generatorU32);
    }

    void rand(bytes_t& bytes, U8 min = 0x00, U8 max = 0xff);

    //void
    //double			pow	(double a, double p) { return }

    template<typename T>
    T atof(const char *p) {

        while (white_space(*p)) { ++p; }

        auto r = static_cast<T>(0);
        bool read_check = true;

        //get sign
        bool neg = false;
        if (*p == '-') {
            neg = true;
            ++p;
        }
        else if (*p == '+') {
            neg = false;
            ++p;
        }

        //get the digits before decimal point
        while (valid_digit(*p)) {
            r = (r * 10) + (*p - '0');
            ++p;
            read_check = false;
        }

        //get the digits after decimal point
        if (*p == '.') {
            T f = 0.0;
            U32 scale = 1;
            ++p;
            while (valid_digit(*p)) {
                f = (f * 10) + (*p - '0');
                ++p;
                scale *= 10;
                read_check = false;
            }
            r += f / static_cast<T>(scale);
        }

        R_ASSERTF(!read_check, "string '%s' not a number", p);

        // Get the digits after the "e"/"E" (exponenet)
        if (*p == 'e' || *p == 'E') {
            S32 e = 0;

            bool negE = false;
            ++p;
            if (*p == '-') { negE = true; ++p; }
            else if (*p == '+') { negE = false; ++p; }

            read_check = true;
            while (valid_digit(*p)) { e = (e * 10) + (*p - '0'); ++p; read_check = false; }
            R_ASSERTF(!read_check, "string '%s' not a number", p);

            if (!neg && e > std::numeric_limits<T>::max_exponent10) {
                e = std::numeric_limits<T>::max_exponent10;
            }
            else if (e < std::numeric_limits<T>::min_exponent10) {
                e = -std::numeric_limits<T>::min_exponent10;
            }

            auto scaleE = static_cast<T>(1.0);
            while (e >= 50) { scaleE *= static_cast<T>(1E50); e -= 50; }
            while (e > 0) { scaleE *= 10; --e; }

            if (negE) {
                r /= scaleE;
            }
            else {
                r *= scaleE;
            }
        }

        while (white_space(*p)) { ++p; }
        R_ASSERTF(*p == '\0', "string '%s' has not null terminator", p);

        if (neg) { r = -r; }
        return r;
    }

    template<typename T>
    T atoi(const char *p) {
        while (white_space(*p)) { ++p; }

        auto r = static_cast<T>(0);
        bool read_check = true;

        //get sign
        bool neg = false;
        if (*p == '-') {
            neg = true;
            ++p;
        }
        else if (*p == '+') {
            neg = false;
            ++p;
        }

        while (valid_digit(*p)) {
            r = (r * 10) + (*p - '0');
            ++p;
            read_check = false;
        }

        R_ASSERTF(!read_check, "string '%s' not a integer number", p);

        while (white_space(*p)) { ++p; }
        R_ASSERTF(*p == '\0', "string '%s' has not null terminator", p);

        if (neg) { r = -r; }
        return r;
    }

    template<typename T>
    T atoui(const char *p) {
        while (white_space(*p)) { ++p; }

        auto r = static_cast<T>(0);
        bool read_check = true;

        if (*p == '+') { ++p; }

        while (valid_digit(*p)) {
            r = (r * 10) + (*p - '0');
            ++p;
            read_check = false;
        }

        R_ASSERTF(!read_check, "string '%s' not a unsigned integer number", p);

        while (white_space(*p)) { ++p; }
        R_ASSERTF(*p == '\0', "string '%s' has not null terminator", p);

        return r;
    }

    template<typename T>
    T atodigit(const char *p) {
        R_ASSERT(false, "ansi to digit: unknown type");
        return NULL; // yeah
    }

    template<> float	atodigit(const char* p);
    template<> double	atodigit(const char* p);
    template<> S64		atodigit(const char* p);
    template<> S32		atodigit(const char* p);
    template<> S16		atodigit(const char* p);
    template<> S8		atodigit(const char* p);
    template<> U64		atodigit(const char* p);
    template<> U32		atodigit(const char* p);
    template<> U16		atodigit(const char* p);
    template<> U8		atodigit(const char* p);

    template <typename T>
    T max(T l, T r) {
        return l > r ? l : r;
    }

    template <typename T>
    T min(T l, T r) {
        return l < r ? l : r;
    }
}


#undef white_space

#endif // !_MATH_HPP_
