#include "bulletManager.hpp"
#include "../../textureManager.hpp"

CBulletManager::CBulletManager() {
    auto texture = CTextureManager::inst().load("tracer.png");
    _trail.setTexture(texture);
}

CBullet* CBulletManager::add(const vector2d& pos, const vector2d& toPoint, double speed, CUpdatableObject* owner) {
    _bullets.push_back(new CBullet(pos, toPoint, speed, owner));
    return _bullets.back();
}

void CBulletManager::update(double timestep) {
    // обновляем
    for (auto bullet : _bullets) {
        bullet->update(timestep);
    }

    // удаляем
    _bullets.remove_if([](CBullet* bullet) {
        if (bullet->getDist() > bullet->_maxDist) {
            delete bullet;
            return true;
        }
        return false;
    });
}

U32 CBulletManager::draw(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const {
    U32 count = 0;
    for (auto bullet : _bullets) {
        if (bullet->draw(window, viewpoint, pixelToMeters))
            count++;
    }
    return count;
}

