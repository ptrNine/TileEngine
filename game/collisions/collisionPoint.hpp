#ifndef _COLLISION_POINT_HPP_
#define _COLLISION_POINT_HPP_

#include "../../base/types.hpp"
#include "../../base/math/game_vector.hpp"
#include <SFML/Graphics/Rect.hpp>


enum ECOLLISION_TYPE {
	ECOLLISION_POINT		= 1U << 0U,
	ECOLLISION_LINE			= 1U << 1U,
	ECOLLISION_RECTANGLE	= 1U << 2U
};

class CPhysicObject;
namespace sf { class RenderWindow; }

////////////////////// COLLISION POINT CLASS ////////////////////////////////
//
//	Класс точки для расчета коллизий. Точка не коллизится с точкой
//
////////////////////////////////////////////////////////////////////////////

class CCollisionPoint {
protected:
	friend class CEngineKernel;
	friend class CCollisionManager;
	U32					COLLISION_TYPE;		// тип коллижн объекта из ECOLLISION_TYPE
	CPhysicObject*		_owner;				// объект-владелец
	vector2d			_shift;				// сдвиг относительно позиции
	vector2d			_point;				// позиция
	vector2d			_displacement;		// вектор перемещения
	sf::Rect<double>	_preCollisionRect;	// прямоугольник для определения пре коллизий

	CCollisionPoint*	_nextFrameIgnore;	// игнорируем этот объект в следующем кадре после столкновения

	U32					_group;				// группа
	std::vector<U32>	_allowedGroups;		// разрешенные группы для столкновений. если пустой - коллизится со всеми

	bool				_alreadyTested;		// флаг, показывающий обрабатывали ли мы на коллизии этот объект. снимается перед началом обнаружения коллизий
	bool				_preCollisionOn;	// флаг, показывающий был ли задет прямоугольник перемещения во время обновления

public:
	// создается только при наличии объекта-владельца
	CCollisionPoint			(CPhysicObject* owner, const vector2d& shift = vector2d(0.0, 0.0));
	virtual ~CCollisionPoint();

	// обновление позиции и смещения
	// !!! следует обновлять сразу после получения смещения в обновлении физики объекта
	// !!! позиция записывается в объект после вызова этого метода
	// позиция записывается в left, top, смещение в кадре - в width, height
	void setShift			(const vector2d& shift)				{ _shift = shift; updatePreCollisionRect(vector2d(0.0, 0.0)); }
	void setGroup			(U32 group)							{ _group = group; }
	void setAllowedGroups	(const std::vector<U32>& groups)	{ _allowedGroups = groups; }

	virtual void updatePreCollisionRect	(const vector2d& displacement);

	// тест коллизии
	// если коллизия произошла, на пару (тройку) столкнувшихся объектов ставим _alreadyTested
	virtual void test();


	// метод рисования объекта коллизии для теста
	virtual bool draw(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const;

	const vector2d& getDisplacement() { return _displacement; }

};

#endif // !_COLLISION_POINT_HPP_