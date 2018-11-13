#ifndef _BULLET_MANAGER_HPP_
#define _BULLET_MANAGER_HPP_

#include <list>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../../base/math/game_vector.hpp"
#include "bullet.hpp"

/////////////////////// BULLET MANAGER SINGLETON ////////////////////////////////
//
//	Занимается созданием, удалением, обновлением и отрисовкой пуль
//
/////////////////////////////////////////////////////////////////////////////////

class CBulletManager {
protected:
    std::list<CBullet*> _bullets;

public:
    CBullet*	add		(const vector2d& pos, const vector2d& toPoint, double speed, CUpdatableObject* owner);
    void		update	(double timestep);
    U32			draw	(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const;

    CBullet*	getBack	() { return _bullets.back(); }
    size_t		getCount() { return _bullets.size(); }

    sf::RectangleShape	_trail;



// singleton realization
private:
    CBulletManager				();
    ~CBulletManager				() = default;

public:
    CBulletManager				(const CBulletManager&) = delete;
    CBulletManager& operator=	(const CBulletManager&) = delete;

public:
    static CBulletManager& inst() {
        static CBulletManager instance;
        return instance;
    }
};

namespace zog {
    inline CBulletManager& bullet_manager() {
        return CBulletManager::inst();
    }
}

#endif // !_BULLET_MANAGER_HPP_
