#ifndef _PROP_HPP_
#define _PROP_HPP_

#include "collisionBoxObject.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

///////////////////////// PROP CLASS ////////////////////////////////////////////////////
//
// Предмет, валяющийся на карте
//
/////////////////////////////////////////////////////////////////////////////////////////
class CProp : public CCBoxObject {
private:
	typedef CCBoxObject inherited;

public:
	friend class CEngineKernel;

	CProp			();
	~CProp	() override = default;

	void update	(double timestep) override;
	bool draw	(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const override;

protected:
	mutable sf::RectangleShape	_shape;			// вид в 2д мире
	double						_staticAccelK;	// замедление при трении. отрицательное значение
	double						_minSpeed;		// минимальная скорость, ниже которой объект считается остановившимся

public:
	void setPhysicParams	(double staticAccelK, double minSpeed)	{ _staticAccelK = staticAccelK; _minSpeed = minSpeed; }
	void setShape			(const sf::RectangleShape& shape);


	void setSize	(const vector2d& size) override;
};

#endif // !_PROP_HPP_
