#ifndef _COLLISION_LINE_HPP_
#define _COLLISION_LINE_HPP_

#include "collisionPoint.hpp"

////////////////////// COLLISION LINE CLASS ////////////////////////////////
//
//	Класс линии для расчета коллизий
//
////////////////////////////////////////////////////////////////////////////

class CCollisionLine : public CCollisionPoint {
private:
    typedef CCollisionPoint inherited;

protected:
    friend class CEngineKernel;
    friend class CCollisionManager;

    vector2d			_pointTwo;		// вторая точка, заданная относительно первой

public:
    // создается только при наличии объекта-владельца
    // point2 - вторая точка линии, заданная относительно первой (0, 0)
    CCollisionLine(CPhysicObject* owner, const vector2d& point2, const vector2d& shift = vector2d(0.0, 0.0));

    // обновление позиции и смещения
    // !!! следует обновлять сразу после получения смещения в обновлении физики объекта
    // !!! позиция записывается в объект после вызова этого метода
    // позиция записывается в left, top, смещение в кадре - в width, height
    void updatePreCollisionRect(const vector2d& displacement) override;

    // метод рисования объекта коллизии для теста
    bool draw(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const override;

};


#endif // !_COLLISION_LINE_HPP_
