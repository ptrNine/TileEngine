#include "bullet.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "../collisions/collisionPoint.hpp"
#include "../collisions/collisionManager.hpp"
#include "bulletManager.hpp"
#include "../../GameBase.hpp"

#define CONST_PI 3.1415926535897932384

CBullet::CBullet(const vector2d& pos, const vector2d& toPoint, double speed, CUpdatableObject* owner): inherited() {
    OBJECT_TYPE |= ECollisionableObject | EBullet;
    _pos = pos;
    setDirFromPoint(toPoint);
    _speed = speed;
    _owner = owner;

    _dist = 0.0;
    _size = vector2d(1.0, 1.0);

    _collisionPoint = new CCollisionPoint(this);
    CCollisionManager::inst().add(_collisionPoint);
}

CBullet::~CBullet() {
    delete _collisionPoint;
    CCollisionManager::inst().erase(_collisionPoint);
}

void CBullet::update(double timestep) {
    vector2d movement = _dir;
    movement.mul(_speed);
    movement.mul(timestep);

    // добавляем смещение в обновлятор коллизий
    _collisionPoint->updatePreCollisionRect(movement);

    _dist += movement.magnitude();

    // передвигаем объект
    _pos.add(movement);

    //printf("dist = %f\n", _dist);
}

bool CBullet::draw(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const {
    // если дистанция маловата - не рисуем
    if (_dist < 2.0)
        return false;
    // получаем координаты относительно камеры
    auto temp = _pos;
    temp.sub(viewpoint);
    // переводим в пиксели
    temp.mul(pixelToMeters);

    // добавляем пол экрана
    temp.add(window->getView().getCenter());

    // мутим интерсект
    sf::FloatRect rect = { -400.0f, -400.0f, window->getView().getSize().x + 400.0f, window->getView().getSize().y + 400.0f };
    if (!rect.contains(temp.asSfmlVecF()))
        return false;

    double size = _collisionPoint->getDisplacement().magnitude();

    if (CEngineState::inst().timestep < 1.0 / 60.0)
        size *= (1.0 / 60.0) / CEngineState::inst().timestep;

    auto& trail = CBulletManager::inst()._trail;
    trail.setSize(sf::Vector2f(_size.getX() * pixelToMeters, size * pixelToMeters));

    trail.setPosition(temp.asSfmlVecF());


    // находим угол
    double angle  = atan2(_dir.getY(), _dir.getX());
    //bullet.setRotation((angle / CONST_PI) * 180.0 + 90.0);
    trail.setRotation((angle / CONST_PI) * 180.0 + 90.0);

    window->draw(trail);
    //window->draw(bullet);

    return true;
}


void CBullet::setSize(const vector2d& size) {
    _size = size;
}