#ifndef ENGINE_BASE_TYPES
#define	ENGINE_BASE_TYPES

#include "types.hpp"
#include "new_types/fixedPoint.hpp"
#include "new_types/sharedString.hpp"


typedef Dec64<24>				fixed;
typedef SharedArray<char>		shared_str;
typedef SharedArray<wchar_t>	shared_wstr;

#endif // ! ENGINE_BASE_TYPES
