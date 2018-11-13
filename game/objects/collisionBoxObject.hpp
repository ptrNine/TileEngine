#ifndef _COLLISION_BOX_OBJECT_HPP_
#define _COLLISION_BOX_OBJECT_HPP_

#include "rectangleObject.hpp"


class CCollisionRectangle;

///////////////////////// COLLISION OBJECT ABSTRACT CLASS ////////////////////////////////////////////////////
//
// Объект, имеющий коллижн бокс
//
/////////////////////////////////////////////////////////////////////////////////////////
class CCBoxObject : public CRectangleObject {
private:
    typedef CRectangleObject inherited;

public:
    friend class CEngineKernel;

    CCBoxObject				();
    ~CCBoxObject			() override;

    void update		(double timestep) override {};
    bool draw		(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const override { return false; };

protected:
    CCollisionRectangle * _cRect;

public:
    virtual CCollisionRectangle* getCollisionRect() { return _cRect; }
};



#endif // !_COLLISION_BOX_OBJECT_HPP_
