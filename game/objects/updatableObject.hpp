#ifndef _UPDATABLE_OBJECT_HPP_
#define _UPDATABLE_OBJECT_HPP_

#include "../../base/types.hpp"


///////////////////////// UPDATEBLE OBJECT ABSTRACT CLASS ///////////////////////////////
//
//	Объект, который может обновляться.
//	Обновление происходит из других объектов.
//	Не обновляется напрямую из EngineKernel
//
/////////////////////////////////////////////////////////////////////////////////////////


// все типы объектов, наследуемых от CUpdatableObjects
enum EObjectCast {
    EUpdatableObject		= 1U << 0U,
    EPhysicObject			= 1U << 1U,
    ERectangleObject		= 1U << 2U,
    EProp					= 1U << 3U,
    ECollisionableObject	= 1U << 4U,
    EBullet					= 1U << 5U,
    ECBoxObject				= 1U << 6U
};


class CUpdatableObject {
protected:
    U32 OBJECT_TYPE;

public:
    U32 GET_TYPE				()					{ return OBJECT_TYPE; }

public:
    CUpdatableObject			();
    virtual ~CUpdatableObject	() = default;
    virtual void update			(double timestep)	{}
    bool testType				(U32 type)			{ return (OBJECT_TYPE & type) == type; }
};



template <U32 EN, typename OBJ>
bool OBJECT_CAST(OBJ* object) {
    return (object->GET_TYPE() & EN) == EN;
}


#endif // !_UPDATABLE_OBJECT_HPP_
