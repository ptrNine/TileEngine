#ifndef __HASHESH__
#define __HASHESH__
#include "engineDefines.hpp"

typedef unsigned int U32;
typedef int S32;


namespace hashes {

	//rot13 hash
	inline U32 rot13(const char *str, U32 len) {
		unsigned int hash = 0U;
		unsigned int i = 0U;

		for (i = 0; i < len; str++, i++) {
			hash += (unsigned char)(*str);
			hash -= (hash << 13U) | (hash >> 19U);
		}
		return hash;
	}


	//murmur2 hash
	#define mmix(h,k) (k) *= m; (k) ^= (k) >> r; (k) *= m; (h) *= m; (h) ^= (k);

	inline U32 murmur_aligned(const void * key, U32 length, U32 seed) {
		const U32 m = 0x5bd1e995;
		const U32 r = 24;
		auto len = static_cast<S64>(length);
		auto l = length;

		auto * data = (const unsigned char *)key;

		U32 h = seed;

		while (len >= 4)
		{
			U32 k = *(U32*)data;

			mmix(h, k);

			data += 4;
			len -= 4;
		}

		U32 t = 0U;

		switch (len)
		{
		case 3: t ^= data[2] << 16U;
		case 2: t ^= data[1] << 8U;
		case 1: t ^= data[0];
		default: break; //no default
		};

		mmix(h, t);
		mmix(h, l);

		h ^= h >> 13U;
		h *= m;
		h ^= h >> 15U;

		return h;
	}

	inline U32 murmur_aligned(const void* key, U32 length) {
		return murmur_aligned(key, length, 1);
	}

#undef mmix

	//fnv1 hash
    inline U32 fnv1(const void * key, U32 length) {
		auto* data = (const char *)key;
		U32 hash = 2166136261U;
		const char* c = &data[0];
		for (U32 i = 0; i < length; ++i) {
			hash *= 16777619U;
			hash ^= *c;
			++c;
		}
		return hash;
	}

}

#endif // !__HASHESH__