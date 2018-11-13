#ifndef _COLLISION_RECTANGLE_HPP_
#define _COLLISION_RECTANGLE_HPP_

#include "collisionLine.hpp"

////////////////////// COLLISION RECTANGLE CLASS ////////////////////////////////
//
//	Класс прямоугольника для расчета коллизий
//
////////////////////////////////////////////////////////////////////////////

class CCollisionRectangle : public CCollisionPoint {
private:
    typedef CCollisionPoint inherited;

protected:
    friend class CEngineKernel;
    friend class CCollisionManager;
    vector2d			_point2;
    vector2d			_point3;
    vector2d			_point4;

public:

    // установка размера
    void setSize(const vector2d& size);

    // создается только при наличии объекта-владельца
    // size - размер прямоугольника
    CCollisionRectangle(CPhysicObject* owner, const vector2d& size, const vector2d& shift = vector2d(0.0, 0.0));

    // обновление позиции и смещения
    // !!! следует обновлять сразу после получения смещения в обновлении физики объекта
    // !!! позиция записывается в объект после вызова этого метода
    // позиция записывается в left, top, смещение в кадре - в width, height
    void updatePreCollisionRect(const vector2d& displacement) override;

    // метод рисования объекта коллизии для теста
    bool draw(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const override;

};

#endif // !_COLLISION_RECTANGLE_HPP_