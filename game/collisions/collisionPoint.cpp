#include "collisionPoint.hpp"
#include "../objects/physicObject.hpp"
#include "collisionManager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

CCollisionPoint::CCollisionPoint(CPhysicObject* owner, const vector2d& shift) {
    // задаем тип
    COLLISION_TYPE = ECOLLISION_POINT;

    _owner = owner;

    _shift = shift;
    _point = owner->getPos() + _shift;
    _displacement.set(0.0, 0.0);

    _alreadyTested = false;
    _preCollisionOn = false;

    _group = 0;

    _nextFrameIgnore = nullptr;
}

CCollisionPoint::~CCollisionPoint() {
}

void CCollisionPoint::updatePreCollisionRect(const vector2d& displacement) {
    // позиция
    _point = _owner->getPos() + _shift;

    // перемещение в кадре
    _displacement = displacement;

    // прямоугольник пре коллизии
    _preCollisionRect.left	= _point.getX();
    _preCollisionRect.top	= _point.getY();
    _preCollisionRect.width = _displacement.getX();
    _preCollisionRect.height = _displacement.getY();
}


void CCollisionPoint::test() {
    CCollisionManager::inst().test(this);
}


bool CCollisionPoint::draw(sf::RenderWindow* window, const vector2d& viewpoint, double pixelToMeters) const {
    sf::Color movRectColor = _preCollisionOn ? sf::Color(255, 200, 120) : sf::Color(255, 160, 0);
    sf::Color startColor(255, 20, 20);
    sf::Color endColor(100, 255, 20);

    // точка позиции
    sf::RectangleShape shape;
    shape.setFillColor(startColor);
    shape.setSize(sf::Vector2f(7.0f, 7.0f));

    // точка перемещения
    sf::RectangleShape shapeMov;
    shapeMov.setFillColor(endColor);
    shapeMov.setSize(sf::Vector2f(5.0f, 5.0f));

    // прямоугольник перемещения
    sf::RectangleShape rectMov;
    rectMov.setFillColor(sf::Color(0, 0, 0, 0));
    rectMov.setOutlineColor(movRectColor);

    if (_preCollisionOn)
        rectMov.setOutlineThickness(4.0f);
    else
        rectMov.setOutlineThickness(2.0f);


    sf::VertexArray lineMov(sf::PrimitiveType::Lines, 2);

    // получаем координаты относительно камеры
    auto temp = _point;
    auto temp2 = _point;
    temp2.add(_displacement);
    sf::Rect<double> movingRect = _preCollisionRect;

    temp.sub(viewpoint);
    temp2.sub(viewpoint);
    movingRect.left -= viewpoint.getX();
    movingRect.top -= viewpoint.getY();

    // переводим в пиксели
    temp.mul(pixelToMeters);
    temp2.mul(pixelToMeters);
    movingRect.left *= pixelToMeters;
    movingRect.top *= pixelToMeters;
    movingRect.width *= pixelToMeters;
    movingRect.height *= pixelToMeters;

    // добавляем пол экрана
    temp.add(window->getView().getCenter());
    temp2.add(window->getView().getCenter());
    movingRect.left += window->getView().getCenter().x;
    movingRect.top += window->getView().getCenter().y;

    lineMov[0] = temp.asSfmlVecF();
    lineMov[1] = temp2.asSfmlVecF();
    lineMov[0].color = startColor;
    lineMov[1].color = endColor;

    rectMov.setPosition(movingRect.left, movingRect.top);
    rectMov.setSize(sf::Vector2f(movingRect.width, movingRect.height));

    temp.add(vector2d(-3.5, -3.5));
    temp2.add(vector2d(-2.5, -2.5));

    shape.setPosition(temp.getX(), temp.getY());
    shapeMov.setPosition(temp2.getX(), temp2.getY());

    // мутим интерсект
    sf::FloatRect rect = { 0.0f, 0.0f, window->getView().getSize().x, window->getView().getSize().y };
    if (rect.intersects(sf::FloatRect(rectMov.getPosition(), rectMov.getSize()))) {
        window->draw(rectMov);
        window->draw(lineMov);
        window->draw(shape);
        window->draw(shapeMov);
        return true;
    }
    return false;
}