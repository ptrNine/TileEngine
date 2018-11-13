#include "rectangleObject.hpp"


CRectangleObject::CRectangleObject() : inherited() {
    OBJECT_TYPE |= ERectangleObject;
    _size.set(0.1, 0.1);
}

void CRectangleObject::setSize(const vector2d& size) {
    _size = size;
}