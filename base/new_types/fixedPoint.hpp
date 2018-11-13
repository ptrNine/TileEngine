#ifndef SIGNED_POINT
#define SIGNED_POINT
#include <limits>
#include <iostream>
#include <string>
#include "../Asserts.hpp"
#include "../math/math.hpp"
#include "../types.hpp"

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

#ifdef NDEBUG 
	#define MULT_RESULT_SIGN(x, y)	((void)0) 
	#define OVERFLOW_CHECK(x)		((void)0) 
#else
	#define MULT_RESULT_SIGN(x, y)	bool _assert_sign = (x < 0) ^ (y < 0)
	#define OVERFLOW_CHECK(x)		ASSERT(_assert_sign == (x < 0), "Dec64 multiplication overflow")
#endif

template <size_t FB>
class					Dec64
{
private:

	bool strToRawData(const char *p) {
		if (p == nullptr) { return false; };

		while (white_space(*p)) {
			p += 1;
		}
		mData = 0;

		bool neg = false;
		if (*p == '-') { neg = true;	++p; }
		else if (*p == '+') { neg = false;	++p; }

		while (valid_digit(*p)) {
			mData = (mData * 10) + (*p - '0');
			++p;
		}

		if (*p == '.' || white_space(*p) || *p == '\0') {
			if (*p != '\0') { ++p; }
		}
		else { R_ASSERTF(false, "string '%s' not a fixed number", p); }

		int fracRows = 0;
		S64 fraction = 0;

		while (valid_digit(*p)) {
			fraction = (fraction * 10) + (*p - '0');
			++fracRows;
			++p;
		}

		while (white_space(*p)) {
			++p;
		}
		if (*p != '\0') {
			R_ASSERTF(false, "string '%s' has not null terminator", p);
		}

		mData = mData * DIGITS + fraction * DIGITS / CpTime::pow(10, fracRows);

		if (neg) { mData = -mData; }
		if (neg != mData < 0)
			return false;

		return true;
	}


	//TODO: too slow in compare with double, need larger type or checking 
	//		free space and using standart multiplication algorithm

	S64	mult (const Dec64& right) const {
		MULT_RESULT_SIGN(mData, right.mData);
		S64 leftInt		= (this->mData	& intMask) >> fracBits;
		S64 rightInt	= (right.mData	& intMask) >> fracBits;
		S64 leftFrac	= this->mData	& fracMask;
		S64 rightFrac	= right.mData	& fracMask;

		S64 result		=	((leftInt * rightInt) << fracBits) + 
							(leftInt * rightFrac + leftFrac * rightInt) + 
							((leftFrac * rightFrac) >> fracBits);

		OVERFLOW_CHECK	(result);
		return			result;
	}

	//TODO: refactor this
	S64 div(const Dec64& right) const {
		S64	leftT		= mData;
		S64 rightT		= right.mData;

		bool sign		= false;
		if (leftT	< 0){ sign ^= true; leftT = -leftT;		}
		if (rightT	< 0){ sign ^= true; rightT = -rightT;	}

		S64	num			= leftT;
		S64 denum		= rightT;

		S64 x			= 1;
		S64 answer		= 0;

		while ((num >= denum) && denum >= 0) {
			x		<<= 1;
			denum	<<= 1;
		}
		
		while (x != 0) {
			if (num >= denum) {
				num -= denum;
				answer += x;
			}

			x		>>= 1;
			denum	>>= 1;
		} 

		U64 l1			= num;
		U64 l2			= rightT;
		const U64 lo	= (static_cast<U64>(num) << fracBits) / rightT;
		S64 res			= (answer << fracBits) | lo;

		MULT_RESULT_SIGN(1, 1);
		OVERFLOW_CHECK(res);

		if (sign) { res = -res; }
		return res;
	}

	Dec64				(S64 data)	{ mData = data; }

public:
	Dec64				()			{ mData = 0; }
	template <typename T>
	Dec64				(T val)		{ *this = val; }


//operators
public:
	template<typename T>
	void	set			(T val)		{ *this = val; }

	template<typename T>
	Dec64&	operator=	(T val) {
		static_assert(sizeof(T) <= 4 || CpTime::areTypesSame<T, double>::value, "Dec64 doesn't support 64/128 integer types");
		mData = static_cast<S64>(val * static_cast<T>(DIGITS));
		return *this;
	}

	Dec64&	operator=	(const char* str) {
		strToRawData(str);
		return *this;
	}
	
	bool	operator!	()	const	{
		return !mData;
	}

	Dec64	operator~	()	const	{
		return Dec64(~mData);
	}

	Dec64	operator-	()	const {
		return Dec64(-mData);
	}

	Dec64	operator+	()	const {
		return *this;
	}

	Dec64&	operator++	() {
		mData += DIGITS;
		return *this;
	}

	Dec64&	operator--	() {
		mData -= DIGITS;
		return *this;
	}

	Dec64	operator++	(int) {
		Dec64 t(*this);
		++*this;
		return t;
	}

	Dec64	operator--	(int) {
		Dec64 t(*this);
		--*this;
		return t;
	}

public:

	bool	operator==	(const Dec64& right) {
		return mData == right.mData;
	}

	bool	operator!=	(const Dec64& right) {
		return mData != right.mData;
	}

	bool	operator>	(const Dec64& right) {
		return mData > right.mData;
	}

	bool	operator<	(const Dec64& right) {
		return mData < right.mData;
	}

	bool	operator>=	(const Dec64& right) {
		return mData >= right.mData;
	}

	bool	operator<=	(const Dec64& right) {
		return mData <= right.mData;
	}

public:
	Dec64	operator+	(const Dec64& right) {
		return Dec64(mData + right.mData);
	}

	Dec64&	operator+=	(const Dec64& right) {
		mData += right.mData;
		return *this;
	}

	Dec64	operator-	(const Dec64& right) {
		return Dec64(mData - right.mData);
	}

	Dec64&	operator-=	(const Dec64& right) {
		mData -= right.mData;
		return *this;
	}

	Dec64	operator*	(const Dec64& right) {
		return Dec64(mult(right));
	}

	Dec64	operator*=	(const Dec64& right) {
		mData = mult(right);
		return *this;
	}

	Dec64	operator/	(const Dec64& right) {
		return Dec64(div(right));
	}

	Dec64	operator/=	(const Dec64& right) {
		mData = div(right);
		return *this;
	}

public:
	double to_double() const {
		return mData / static_cast<double>(DIGITS);
	}

	/*
	template <typename T>
	bool is_similar(const Dec64& num, const T& maxDifference) const {
		if (std::round(Dec64(mData - num.mData)) < maxDifference)
			return true;
		return false;
	}
	*/
private:
	const static unsigned int	fracBits	= FB;
	const static S64			maxMultBits = (63 - fracBits) / 2;
	constexpr const static S64	maxMultint	= CpTime::pow<S64>(2, maxMultBits);
	constexpr const static S64	DIGITS		= CpTime::pow<S64>(2, fracBits);
	constexpr const static S64	intMask		= ~(DIGITS - 1);
	constexpr const static S64	fracMask	= ~intMask;

private:
	S64							mData;


};

template <size_t FB>
std::ostream &operator<<(std::ostream &st, const Dec64<FB> &a) {
	st << a.to_double();
	return st;
}

template <size_t FB>
std::istream &operator>>(std::istream &st, Dec64<FB> &a) {
	std::string buff;
	st >> buff;
	a.set(buff.c_str());
	return st;
}

namespace math {
	template <size_t FB>
	Dec64<FB> round(const Dec64<FB>& a) {
		Dec64<FB> res(a);
		if (res < 0) { res = -res; }
		return res;
	}
}

#endif // !SIGNED_POINT