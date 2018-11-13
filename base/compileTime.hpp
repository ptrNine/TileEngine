#ifndef COMPILE_TIME_H
#define COMPILE_TIME_H

namespace details {
    //constexpr power
    template<typename T>
    constexpr T constPowCalc(T a, int e, T start) noexcept {
        return e > 0 ? constPowCalc(a * start, e - 1, start) : a;
    }

    template<typename T>
    constexpr T constPow(T a, int e) noexcept {
        return e > 0 ? constPowCalc(a, e - 1, a) : 1;
    }

    //is types same
    template <class A, class B>
    struct isSameTypes
    {
        static const bool value = false;
    };

    template <class A>
    struct isSameTypes<A, A>
    {
        static const bool value = true;
    };
}

class CpTime {

public:
    template <class A, class B>
    struct areTypesSame
    {
        static const bool value = details::isSameTypes<A, B>::value;
    };

    template<typename T>
    static constexpr T pow(T a, int e) noexcept {
        return details::constPow<T>(a, e);
    }
};
#endif // !COMPILE_TIME_H