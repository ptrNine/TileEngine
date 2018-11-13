#ifndef _RECTANGLE_OBJECT_HPP_
#define _RECTANGLE_OBJECT_HPP_

#include "physicObject.hpp"


///////////////////////// RECTANGLE OBJECT CLASS ///////////////////////////////
//
// Объект, имеющий размеры прямоугольника
//
/////////////////////////////////////////////////////////////////////////////////////////
class CRectangleObject : public CPhysicObject {
private:
	typedef CPhysicObject inherited;

public:
	friend class CEngineKernel;
	CRectangleObject			();
	~CRectangleObject	() override = default;

protected:
	vector2d			_size;		// размер объекта

public:
	virtual void setSize				(const vector2d& size);

};


#endif // !_RECTANGLE_OBJECT_HPP_
