#ifndef _BULLET_HPP_
#define _BULLET_HPP_

#include "physicObject.hpp"

// Todo: разобраться с set size

class CCollisionPoint;

///////////////////////// BULLET CLASS ////////////////////////////////////////////////////
//
// Класс пули. Создает коллижн поинт.
//
/////////////////////////////////////////////////////////////////////////////////////////
class CBullet : public CPhysicObject{
private:
	typedef CPhysicObject inherited;

public:
	friend class CEngineKernel;
	friend class CBulletManager;

	CBullet				(const vector2d& pos, const vector2d& toPoint, double speed, CUpdatableObject* owner);
	~CBullet	() override;

	void update	(double timestep) override;
	bool draw	(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const override;

protected:

	CUpdatableObject*			_owner;			// тот, кто пустил пулю
	CCollisionPoint*			_collisionPoint;// коллижн поинт
	double						_minSpeed;		// минимальная скорость, ниже которой пуля уничтожается
	double						_maxDist;		// максимальный путь, дальше которого пуля уничтожается
	vector2d					_size;			// размер трассера
	double						_dist;			// путь

public:
	virtual void setSize	(const vector2d& size);

	void setPhysicParams	(double minSpeed, double maxDist)	{ _minSpeed = minSpeed; _maxDist = maxDist; }

	double		getDist		() const							{ return _dist; }

};

#endif // !_BULLET_HPP_
