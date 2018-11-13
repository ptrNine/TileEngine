#include "physicObject.hpp"


CPhysicObject::CPhysicObject() : inherited() {
	OBJECT_TYPE |= EPhysicObject;

	_pos.set(0.0, 0.0);
	_speed = 0.0;
	_dir.set(1.0, 0.0);
}

void CPhysicObject::setPos(const vector2d& pos) {
	_pos = pos;
}

void CPhysicObject::setPos(double x, double y) {
	_pos.set(x, y);
}

void CPhysicObject::setDirFromPoint(const vector2d& point) {
	_dir = point;
	_dir.sub(_pos);
	_dir.normalize();
}

void CPhysicObject::setSpeed(double speed) {
	_speed = speed;
}

void CPhysicObject::update(double timestep) {
}