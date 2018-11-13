/*
Shared string implementation
Author:				pdrnine
Last update:		11/01/17
*/

#ifndef SHARED_STRING
#define SHARED_STRING
#include <unordered_map>
#include <limits>
#include <cstring>
#include <algorithm>
#include "../types.hpp"
#include "../engineDefines.hpp"
#include "../Asserts.hpp"   
#include "../allocators/array_allocator.hpp"
#include "../hashes.hpp"

#define MAX_STR_LEN			( std::numeric_limits<U32>::max()/ std::numeric_limits<U8>::max())


extern ArrayPool sharedAlloc;


namespace ne_laz {

#ifdef SHARED_DEBUG

	template<typename T>
	class SharedStatistics {
	public:
		static std::unordered_map<U32, U32>& memory_usability() {
			static std::unordered_map<U32, U32> memory_usab;
			return memory_usab;
		}
	};


#endif //SHARED_DEBUG


	inline size_t getLen(const char* str)		{ return strnlen(str, MAX_STR_LEN); }
	//FC_INL size_t getLen(const wchar_t* str)	{ return strnlen(str, MAX_STR_LEN / 2); }

	template<size_t T>
	void vshprintf_s(char(&buffer)[T], const char* format, va_list args) {
		vsprintf(buffer, format, args);
	}

	template<size_t T>
	void vshprintf_s(wchar_t(&buffer)[T], const wchar_t* format, va_list args) {
	    vsprintf(buffer, format, args);
	}



	template<typename T>
	class SharedItem
	{

	private:
		char*					value;
		U32						users;

	public:
		SharedItem								(const T* str, U32 len)
		{
			value	= (T*)sharedAlloc.malloc(len + 1);
			users	= 1;
			memcpy(value, str, len + 1);

#ifdef SHARED_DEBUG
			auto finder = SharedStatistics<T>::memory_usability().find(len);
			if (finder != SharedStatistics<T>::memory_usability().end())
				finder->second++;
			else
				SharedStatistics<T>::memory_usability()[len] = 1;
#endif //SHARED_DEBUG

		}
		~SharedItem								()									{ sharedAlloc.free(value); }

		U32						getUsers		()									{ return users; }
		void					decrUsers		()									{ --users; }
		void					incUsers		()									{ ++users; }
		const T*				getVal			() const							{ return value; }
		T*						getVal			()									{ return value; }
	};

#ifdef SHARED_DEBUG
	extern void check_hash(U32, U32, const void*);
#endif // !SHARED_DEBUG


	struct	SharedHash
	{
		U32						_unic;
		U32						_length;

		SharedHash									()							{ _unic = 0; _length = 0; }
		SharedHash									(const void* key, U32 length)
		{
			_unic	= 0;
			_length = length;
			if (_length > 0)
				_unic	= hashes::fnv1(key, _length);
#ifdef SHARED_DEBUG
			if (_length > 0) { check_hash(_length, _unic, key); }
#endif // !SHARED_DEBUG
		}

		bool					operator==			(const SharedHash& right) const
		{
			return this->_length	== right._length
				&& this->_unic		== right._unic;
		}
		bool					operator!=			(const SharedHash& right) const
		{
			return this->_length	!= right._length
				|| this->_unic		!= right._unic;
		}
		SharedHash&				operator=			(const SharedHash& right) = default;
	};

	struct SharedHasher
	{
		U64 operator() (const SharedHash& hash) const {
			return (static_cast<U64>(hash._unic) << 32U) + hash._length;
		}
	};




	//strings charge class
	template<typename T>
	class	SharedCharge
	{
	private:
		std::unordered_map<SharedHash, SharedItem<T>*, SharedHasher>	strMap;

	private:
		SharedCharge								()									= default;
		~SharedCharge								()									= default;

	public:
		SharedCharge								(const SharedCharge&)				= delete;
		SharedCharge&			operator=			(const SharedCharge&)				= delete;
		void					_addIfNotExist		(SharedHash hash, const T* str)
		{
			auto findVal	= strMap.find(hash);
			if (findVal == strMap.end())
				strMap[hash]	= new SharedItem<T>(str, hash._length);
			else
				findVal->second->incUsers();
		}

	public:
		SharedHash				getHash				(const T* str)
		{
			U32 length	= getLen(str);

			if (length == 0) {
				SharedHash hash;
				return hash;
			}

			ASSERT(length < MAX_STR_LEN, "str length too big");
			SharedHash hash(str, length);
			_addIfNotExist(hash, str);
			return hash;
		}
		void					delete_at			(SharedHash hash)
		{
			//when program exiting, map can be cleared, but some str's may be not delete already
			if (hash._length == 0 || strMap.size() == 0)
				return;

			auto val = strMap[hash];

			if (val->getUsers() > 1) {
				val->decrUsers();
			}
			else {
				delete val;
				strMap.erase(hash);
			}
		}
		void					multiply			(SharedHash hash)					{ if (hash._length == 0) return; strMap[hash]->incUsers(); }
		T*						val_at				(SharedHash hash)					{ if (hash._length == 0) return nullptr; return strMap[hash]->getVal(); }
		const T*				val_at				(SharedHash hash) const				{ if (hash._length == 0) return nullptr; return strMap.at(hash)->getVal(); }
		bool					is_exist			(SharedHash hash) const				{ auto finding = strMap.find(hash); return finding != strMap.end(); }
		size_t					size				() const							{ return strMap.size(); }

		static SharedCharge&	_instance			()									{ static SharedCharge instance; return instance; }

	public:
		U32						_getUsers			(SharedHash hash)					{ return strMap[hash]->getUsers(); }

#ifdef  SHARED_DEBUG
	public:
		void					_print_map			() { 
			Log("SharedCharge map:");
			for (auto& i : strMap) {
				Log("hash[%08Xi] = %s", i.first._unic, i.second->getVal());
			}
		}

#endif //SHARED_DEBUG
	};
}





#define charge ne_laz::SharedCharge<T>::_instance()

template<typename T>
class SharedArray
{

private:
	static const U32		SPRINTF_MAX_BUFFER = 1024 * 4;
	ne_laz::SharedHash		_hash;

public:
	SharedArray									()									{ _hash._length = 0; _hash._unic = 0; }
	SharedArray						            (const T* str)						{ *this = str; }
	SharedArray									(const SharedArray& str)			{ *this = str; }
	~SharedArray								()									{ charge.delete_at(_hash); }

	const T*				c_str				() const							{ return charge.val_at(_hash); }
	U32						length				() const							{ return _hash._length; }
	U32						size				() const							{ return _hash._length; }
	char					back				() const							{ return charge.val_at(_hash)[_hash._length - 1]; }
	ne_laz::SharedHash		get_hash			() const							{ return _hash; }
	char					at					(U32 pos) const						{ return charge.val_at(_hash)[pos]; }
	void					erase				(U32 pos)
	{
		if (pos >= this->length())
			return;

		T save		= 0;
		T* real		= charge.val_at(_hash);

		//protection from swap with deleted str
		bool secondSwap = true;

		if (charge._getUsers(_hash) == 1)
			secondSwap = false;

		std::swap(real[pos], save);

		this->init(real);

		//if old string have another users
		if (secondSwap)
			std::swap(real[pos], save);
	}
	void					erase_front			(U32 pos)
	{
		if (static_cast<S32>(pos) >= static_cast<S32>(this->length()) - 1)
			return;

		const T* real	= charge.val_at(_hash);
		real			+= pos + 1;
		this->init(real);
	}
	void					erase_edge			(U32 front, U32 back)
	{
		if (front >= back
			&& static_cast<S32>(front) >= static_cast<S32>(this->length()) - 1 
			&& back >= this->length())
			return;

		//back
		T* real_back	= charge.val_at(_hash) + back;
		T save			= 0;
		bool secondSwap = true;

		if (charge._getUsers(_hash) == 1)
			secondSwap		= false;

		std::swap(*real_back, save);

		//front erase
		T* real_front	= charge.val_at(_hash);
		real_front		+= front + 1;

		this->init(real_front);

		//if old string have another users
		if (secondSwap)
			std::swap(*real_back, save);
	}
	U32						find				(T c) const
	{
		for (U32 i = 0; i < this->length(); ++i) {
			if ((*this)[i] == c)
				return i;
		}
		return length();
	}
	U32						find_back			(T c) const {
		for (auto i = static_cast<S32>(this->length()); i >= 0; --i) {
			if ((*this)[i] == c)
				return i;
		}
		return length();
	}
	void					init				(const T *str)
	{
		ne_laz::SharedHash newHash = charge.getHash(str);

		if (newHash != _hash) {
			if (_hash._length > 0)
				charge.delete_at(_hash);
			_hash = newHash;
		}
	}
	void					sprintf				(const T *format, ...) {
		va_list args;
		va_start(args, format);
		T buffer[SPRINTF_MAX_BUFFER];

		ne_laz::vshprintf_s(buffer, format, args);

		va_end(args);
		init(buffer);
	}

	//must be deleted in release
public:
	void					_info				() const {
		printf("%s [%i : %i] %i users\n", c_str(), _hash._unic, _hash._length, charge._getUsers(_hash));
	}

#ifdef  SHARED_DEBUG

	void static				_memory_usability	()
	{
		std::vector<std::pair<U32, U32>> mem_use;
		for (auto& i : ne_laz::SharedStatistics<T>::memory_usability())
			mem_use.push_back(i);
		
		std::sort(mem_use.begin(), mem_use.end(), [](const std::pair<U32, U32>& left, const std::pair<U32, U32>& right) {
			return right.second < left.second;
		});

		U32 bytes = 0, usages = 0;
		Log("Shared String memory usability:");
		for (auto& i : mem_use) {
			bytes	+= i.first * i.second;
			usages	+= i.second;
			Log("[%i] bytes, used %i times", i.first * sizeof(T), i.second);
		}

		Log("Bytes per string average: %i ", bytes / usages);
	}
	void static				_print_shared_map	()									{ charge._print_map(); }

#endif //  SHARED_DEBUG

	//operators
public:
	SharedArray&			operator=			(const SharedArray& right)
	{ 
		if (_hash != right._hash) {
			charge.delete_at(_hash);
			_hash = right._hash;
			charge.multiply(_hash);
		}
		return *this; 
	}
	SharedArray&			operator=			(const T* str)						{ init(str); return *this; }
	bool					operator==			(const SharedArray& right) const	{ return _hash == right._hash; }
	char					operator[]			(U32 pos) const						{ return charge.val_at(_hash)[pos]; }

	//U64 operator() () const {
	//	return ne_laz::StrHasher().operator()(_hash);
	//}
};


namespace std {
	template<typename T>
	struct hash<SharedArray<T>> 
	{
		U64 operator() (const SharedArray<T>& str) const {
			return ne_laz::SharedHasher().operator()(str.get_hash());
		}
	};
}

template <typename T>
std::ostream			&operator<<			(std::ostream &st, const SharedArray<T> &a)		{ st << a.c_str(); return st;}
template <typename T>
std::istream			&operator>>			(std::istream &st, SharedArray<T> &a)			{ std::string buff; st >> buff; a = buff.c_str(); return st; }



typedef SharedArray<char>		shared_str;
typedef SharedArray<wchar_t>	shared_wstr;

#undef charge

#endif // !SHARED_STRING
