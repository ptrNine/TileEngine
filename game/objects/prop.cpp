#include "prop.hpp"
#include "../collisions/collisionRectangle.hpp"
#include <SFML/Graphics/RenderWindow.hpp>


CProp::CProp() : inherited() {
    OBJECT_TYPE |= EProp;

    _staticAccelK = -1.0;
    _minSpeed = 0.2;
}


void CProp::setShape(const sf::RectangleShape& shape) {
    auto size = _shape.getSize();
    _shape = shape;
    _shape.setSize(size);
}


void CProp::update(double timestep) {
    // если скорость ниже минимальной - объект не движется
    if (_speed < _minSpeed)
        return;

    // находим скорость с учетом трения
    _speed += _staticAccelK * timestep;
    if (_speed < 0.0)
        _speed = 0.0;

    // находим смещение
    vector2d movement = _dir;
    movement.mul(_speed);
    movement.mul(timestep);

    // добавляем смещение в обновлятор коллизий
    _cRect->updatePreCollisionRect(movement);

    // передвигаем объект
    _pos.add(movement);
}


bool CProp::draw(sf::RenderWindow*	window, const vector2d& viewpoint, double pixelToMeters) const {
    // получаем координаты относительно камеры
    auto temp = _pos;
    temp.sub(viewpoint);

    // переводим в пиксели
    temp.mul(pixelToMeters);

    // добавляем пол экрана
    temp.add(window->getView().getCenter());

    _shape.setPosition(temp.getX(), temp.getY());
    _shape.setSize(sf::Vector2f(_size.getX() * pixelToMeters, _size.getY() * pixelToMeters));

    // мутим интерсект
    sf::FloatRect rect = { 0.0f, 0.0f, window->getView().getSize().x, window->getView().getSize().y };
    //printf("view: [%f, %f, %f, %f]   temp: [%f, %f]\n", rect.left, rect.top, rect.width, rect.height, temp.getX(), temp.getY());
    if (rect.intersects(sf::FloatRect(temp.getX(), temp.getY(), _shape.getSize().x, _shape.getSize().y))) {
        window->draw(_shape);
        return true;
    }
    return false;
}


void CProp::setSize(const vector2d& size) {
    inherited::setSize(size);
}