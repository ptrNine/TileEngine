#include "collisionBoxObject.hpp"
#include "../collisions/collisionManager.hpp"
#include "../collisions/collisionRectangle.hpp"



CCBoxObject::CCBoxObject() : inherited() {
    OBJECT_TYPE |= ECBoxObject;
    _cRect = new CCollisionRectangle(this, vector2d(1.0, 1.0));
    CCollisionManager::inst().add(_cRect);

    // инициализируем коллижн рект
    _cRect->updatePreCollisionRect(vector2d(0.0, 0.0));
}


CCBoxObject::~CCBoxObject() {
    CCollisionManager::inst().erase(_cRect);
    delete _cRect;
}