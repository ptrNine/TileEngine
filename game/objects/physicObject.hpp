#ifndef _PHYSIC_OBJECT_HPP_
#define _PHYSIC_OBJECT_HPP_

#include "updatableObject.hpp"
#include "../../base/math/game_vector.hpp"

///////////////////////// PHYSIC OBJECT ABSTRACT CLASS ///////////////////////////////
//
// Объект, который существует в 2д пространстве и может передвигаться.
// Обновляется напрямую из EngineKernel
//
/////////////////////////////////////////////////////////////////////////////////////////

namespace sf {
    class RenderWindow;
}
class CPhysicObject : public CUpdatableObject {
private:
    typedef CUpdatableObject inherited;


public:
    CPhysicObject			();
    ~CPhysicObject	() override = default;

    void update		(double timestep) override;
    virtual bool draw		(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const	{ return false; };

protected:
    double			_speed;				// не может быть отрицательной!
    vector2d		_pos;
    vector2d		_dir;


public:
    virtual void setPos				(const vector2d& pos);
    virtual void setPos				(double x, double y);
    virtual void setDirFromPoint	(const vector2d& point);
    virtual void setDir				(const vector2d& dir)		{ _dir = dir; } // DIR MUST BE NORMALIZED!
    virtual void setSpeed			(double speed);

    const vector2d& getPos		() const		{ return _pos; }
    const vector2d& getDir		() const		{ return _dir; }
    double			getSpeed	() const		{ return _speed; }
};


#endif // !_PHYSIC_OBJECT_HPP_
