#include "sharedString.hpp"

namespace ne_laz {

#ifdef SHARED_DEBUG
	void check_hash(U32 _length, U32 _unic, const void* key) {
		SharedHash		hash;
		hash._length	= _length;
		hash._unic		= _unic;
		if (SharedCharge<char>::_instance().is_exist(hash)) {
			R_ASSERTF(strcmp(SharedCharge<char>::_instance().val_at(hash), reinterpret_cast<const char*>(key)) == 0,
				"Hash collision <char> [%i : %i] adding = \"%s\", old = \"%s\"",
				reinterpret_cast<const char*>(key), SharedCharge<char>::_instance().val_at(hash));
		}
	}
#endif // !SHARED_DEBUG

}