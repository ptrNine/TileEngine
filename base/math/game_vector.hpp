#ifndef __GAME_VECTOR_HPP___
#define __GAME_VECTOR_HPP___
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
//#include "../types.hpp"


template<typename T> class CVector3d;
template<typename T>
class CVector2d {
public:
	CVector2d	(): _x(0.f), _y(0.f)		{}
	CVector2d	(T x, T y): _x(x), _y(y)	{}
	explicit CVector2d	(T val): _x(val), _y(val)	{}
	template<typename T2>
	CVector2d	(const sf::Vector2<T2>& sfml_vec) { _x = sfml_vec.x; _y = sfml_vec.y; }


	void set	(const CVector2d& right)	{ _x = right._x; _y = right._y; }
	void set	(T x, T y)					{ _x = x; _y = y; }
	void set	(T val)						{ _x = val; _y = val; }
	
	void setX	(const CVector2d& right)	{ _x = right._x; }
	void setX	(T x)						{ _x = x; }

	void setY	(const CVector2d& right)	{ _y = right._y; }
	void setY	(T y)						{ _y = y; }

	T	 getX	()	const					{ return _x; }
	T	 getY	()	const					{ return _y; }

	// base operations
	void add	(const CVector2d& right)	{ _x += right._x; _y += right._y; }
	void sub	(const CVector2d& right)	{ _x -= right._x; _y -= right._y; }
	void mul	(const CVector2d& right)	{ _x *= right._x; _y *= right._y; }
	void div	(const CVector2d& right)	{ _x /= right._x; _y /= right._y; }

	void addX	(const CVector2d& right)	{ _x += right._x; }
	void subX	(const CVector2d& right)	{ _x -= right._x; }
	void mulX	(const CVector2d& right)	{ _x *= right._x; }
	void divX	(const CVector2d& right)	{ _x /= right._x; }

	void addY	(const CVector2d& right)	{ _y += right._y; }
	void subY	(const CVector2d& right)	{ _y -= right._y; }
	void mulY	(const CVector2d& right)	{ _y *= right._y; }
	void divY	(const CVector2d& right)	{ _y /= right._y; }

	void add	(T right)					{ _x += right; _y += right; }
	void sub	(T right)					{ _x -= right; _y -= right; }
	void mul	(T right)					{ _x *= right; _y *= right; }
	void div	(T right)					{ _x /= right; _y /= right; }

	void addX	(T right)					{ _x += right; }
	void subX	(T right)					{ _x -= right; }
	void mulX	(T right)					{ _x *= right; }
	void divX	(T right)					{ _x /= right; }

	void addY	(T right)					{ _y += right; }
	void subY	(T right)					{ _y -= right; }
	void mulY	(T right)					{ _y *= right; }
	void divY	(T right)					{ _y /= right; }

	void round	()							{ _x = ::round(_x); _y = ::round(_y); }
	void roundX	()							{ _x = ::round(_x); }	
	void roundY	()							{ _y = ::round(_y); }

	void cutFrac()							{ _x = int(_x); _y = int(_y); }
	void cutFracX()							{ _x = int(_x); }
	void cutFracY()							{ _y = int(_y); }

	void inv	()							{ _x = -_x; _y = -_y; }
	void invX	()							{ _x = -_x; }
	void invY	()							{ _y = -_y; }

	// operators

	CVector2d	operator+	(const CVector2d& r) const	{ return CVector2d(_x + r.getX(), _y + r.getY()); }
	CVector2d	operator-	(const CVector2d& r) const	{ return CVector2d(_x - r.getX(), _y - r.getY()); }
	CVector2d&	operator+=	(const CVector2d& r)		{ add(r); return *this; }
	CVector2d&	operator-=	(const CVector2d& r)		{ sub(r); return *this; }

	CVector2d	operator*	(T r) const					{ return CVector2d(_x * r, _y * r); }
	CVector2d	operator/	(T r) const					{ return CVector2d(_x / r, _y / r); }
	CVector2d&	operator*=	(T r) const					{ mul(r); return *this; }
	CVector2d&	operator/=	(T r) const					{ div(r); return *this; }

	// vector operations
	T		magnitude			()	const				{ return sqrt(_x*_x + _y*_y); }
	void	normalize			()						{ T tmp = sqrt(_x*_x + _y*_y); _x/=tmp; _y/=tmp; }
	T		normalize_magn		()						{ T tmp = sqrt(_x*_x + _y * _y); _x /= tmp; _y /= tmp; return tmp; }

	CVector3d<T>	homogeousCross	(const CVector2d& right) const;
	double			dot				(const CVector2d& right) const	{ return _x * right._x + _y * right._y; }
	CVector2d		normal			() const						{ auto res = CVector2d(1.0, -_x / _y); return std::isinf(res._y) ? CVector2d(0.0, 1.0) : res; } // normal isn't normalized vector!


	sf::Vector2f asSfmlVecF() const			{ return sf::Vector2f(_x, _y); }

protected:
	T _x;
	T _y;
};



template<typename T>
class CVector3d {
public:
	CVector3d	()								: _x(0.0),	_y(0.0),	_z(0.0)			{}
	CVector3d	(T x, T y, T z)					: _x(x),	_y(y),		_z(z)			{}
	explicit CVector3d	(T val)							: _x(val),	_y(val),	_z(val)			{}
	CVector3d	(const CVector2d<T>& v, T z)	: _x(v.getX()),	_y(v.getY()),	_z(z)	{}
	
	template<typename T2>
	explicit CVector3d	(const sf::Vector3<T2>& sfml_vec) {
		_x = sfml_vec.x; 
		_y = sfml_vec.y; 
		_z = sfml_vec.z; 
	}


	void set	(const CVector3d& right)	{ _x = right._x; _y = right._y; _z = right._z; }
	void set	(T x, T y, T z)				{ _x = x; _y = y; _z = z; }
	void set	(T val)						{ _x = val; _y = val; _z = val; }
	
	void setX	(const CVector3d& right)	{ _x = right._x; }
	void setX	(T x)						{ _x = x; }

	void setY	(const CVector3d& right)	{ _y = right._y; }
	void setY	(T y)						{ _y = y; }

	void setZ	(const CVector3d& right)	{ _z = right._z; }
	void setZ	(T z)						{ _z = z; }

	T	 getX	()	const					{ return _x; }
	T	 getY	()	const					{ return _y; }
	T	 getZ	()	const					{ return _z; }

	// base operations
	void add	(const CVector3d& right)	{ _x += right._x; _y += right._y; _z += right._z; }
	void sub	(const CVector3d& right)	{ _x -= right._x; _y -= right._y; _z -= right._z; }
	void mul	(const CVector3d& right)	{ _x *= right._x; _y *= right._y; _z *= right._z; }
	void div	(const CVector3d& right)	{ _x /= right._x; _y /= right._y; _z /= right._z; }

	CVector3d cross	(const CVector3d& right) const	{
		CVector3d res; 
		res._x = _y * right._z - right._y * _z;
		res._y = -(_x * right._z - right._x * _z);
		res._z = _x * right._y - right._x * _y;
		return res;
	}

	void addX	(const CVector3d& right)	{ _x += right._x; }
	void subX	(const CVector3d& right)	{ _x -= right._x; }
	void mulX	(const CVector3d& right)	{ _x *= right._x; }
	void divX	(const CVector3d& right)	{ _x /= right._x; }

	void addY	(const CVector3d& right)	{ _y += right._y; }
	void subY	(const CVector3d& right)	{ _y -= right._y; }
	void mulY	(const CVector3d& right)	{ _y *= right._y; }
	void divY	(const CVector3d& right)	{ _y /= right._y; }

	void addZ	(const CVector3d& right)	{ _z += right._z; }
	void subZ	(const CVector3d& right)	{ _z -= right._z; }
	void mulZ	(const CVector3d& right)	{ _z *= right._z; }
	void divZ	(const CVector3d& right)	{ _z /= right._z; }

	void add	(T right)					{ _x += right; _y += right; _z += right; }
	void sub	(T right)					{ _x -= right; _y -= right; _z -= right; }
	void mul	(T right)					{ _x *= right; _y *= right; _z *= right; }
	void div	(T right)					{ _x /= right; _y /= right; _z /= right; }

	void addX	(T right)					{ _x += right; }
	void subX	(T right)					{ _x -= right; }
	void mulX	(T right)					{ _x *= right; }
	void divX	(T right)					{ _x /= right; }

	void addY	(T right)					{ _y += right; }
	void subY	(T right)					{ _y -= right; }
	void mulY	(T right)					{ _y *= right; }
	void divY	(T right)					{ _y /= right; }

	void addZ	(T right)					{ _z += right; }
	void subZ	(T right)					{ _z -= right; }
	void mulZ	(T right)					{ _z *= right; }
	void divZ	(T right)					{ _z /= right; }

	void round	()							{ _x = ::round(_x); _y = ::round(_y); _z = ::round(_z); }
	void roundX	()							{ _x = ::round(_x); }	
	void roundY	()							{ _y = ::round(_y); }
	void roundZ	()							{ _z = ::round(_z); }

	void cutFrac()							{ _x = int(_x); _y = int(_y); _z = int(_z); }
	void cutFracX()							{ _x = int(_x); }
	void cutFracY()							{ _y = int(_y); }
	void cutFracZ()							{ _z = int(_z); }

	void inv	()							{ _x = -_x; _y = -_y; _z = -_z; }
	void invX	()							{ _x = -_x; }
	void invY	()							{ _y = -_y; }
	void invZ	()							{ _z = -_z; }

	// operators

	CVector3d	operator+	(const CVector3d& r) const	{ return CVector3d(_x + r.getX(), _y + r.getY, _z + r.getZ()); }
	CVector3d	operator-	(const CVector3d& r) const	{ return CVector3d(_x - r.getX(), _y - r.getY, _z - r.getZ()); }
	CVector3d&	operator+=	(const CVector3d& r)		{ add(r); return *this; }
	CVector3d&	operator-=	(const CVector3d& r)		{ sub(r); return *this; }

	CVector3d	operator*	(T r) const					{ return CVector3d(_x * r, _y * r, _z * r); }
	CVector3d	operator/	(T r) const					{ return CVector3d(_x / r, _y / r, _z / r); }
	CVector3d&	operator*=	(T r) const					{ mul(r); return *this; }
	CVector3d&	operator/=	(T r) const					{ div(r); return *this; }

	// vector operations

#ifdef _GAME_VECTOR_HALF_SIZE_OPERATIONS_
	T		magnitude()	const				{ return sqrtf(_x*_x + _y*_y + _z*_z); }
	void	normalize()						{ T tmp = sqrtf(_x*_x + _y*_y + _z*_z); _x/=tmp; _y/=tmp; _z/=tmp; }
	T		normalize_magn()				{ T tmp = sqrtf(_x*_x + _y*_y + _z*_z); _x/=tmp; _y/=tmp; _z/=tmp; return tmp; }
#else
	T		magnitude()	const				{ return sqrt(_x*_x + _y*_y + _z*_z); }
	void	normalize()						{ T tmp = sqrt(_x*_x + _y*_y + _z*_z); _x/=tmp; _y/=tmp; _z/=tmp; }
	T		normalize_magn()				{ T tmp = sqrt(_x*_x + _y*_y + _z*_z); _x/=tmp; _y/=tmp; _z/=tmp; return tmp; }
#endif
	sf::Vector3f asSfmlVecF() const			{ return sf::Vector3f(_x, _y, _z); }
	CVector2d<T> get2D() const				{ return CVector2d<T>(_x, _y); }
	CVector2d<T> getHomogeneous() const		{ return CVector2d<T>(_x, _y) / _z; }


protected:
	T _x;
	T _y;
	T _z;
};


typedef CVector2d<double>	vector2d;
typedef CVector3d<double>	vector3d;

template<typename T>
CVector3d<T> CVector2d<T>::homogeousCross(const CVector2d<T>& right) const {
	return CVector3d<T>(
		_y - right._y, 
		right._x - _x, 
		_x * right._y - right._x * _y
	);
}


class CVectorTest {
public:
	static void test() {

	}

};

#endif //__GAME_VECTOR_HPP___