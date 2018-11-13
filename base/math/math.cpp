#include "math.hpp"
#include <algorithm>
#include "../timer.hpp"


// Todo: замутить инициализацию наносекундами
namespace ne_laz {
	std::mt19937 generatorU32(static_cast<U32>(zog::timer().getSystemDateTime().sec));

}

namespace math {

	void rand(bytes_t& bytes, U8 min, U8 max) {
		std::uniform_int_distribution<U32> uid(min, max);
		std::generate(bytes.begin(), bytes.end(), [&]() -> U8 {
			return static_cast<U8>(uid(ne_laz::generatorU32));
		});
	}

	template<> float	atodigit(const char* p) { return atof<float>(p); }
	template<> double	atodigit(const char* p) { return atof<double>(p); }
	template<> S64		atodigit(const char* p) { return atoi<S64>(p); }
	template<> S32		atodigit(const char* p) { return atoi<S32>(p); }
	template<> S16		atodigit(const char* p) { return atoi<S16>(p); }
	template<> S8		atodigit(const char* p) { return atoi<S8>(p); }
	template<> U64		atodigit(const char* p) { return atoui<U64>(p); }
	template<> U32		atodigit(const char* p) { return atoui<U32>(p); }
	template<> U16		atodigit(const char* p) { return atoui<U16>(p); }
	template<> U8		atodigit(const char* p) { return atoui<U8>(p); }

}