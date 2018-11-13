#include "collisionLine.hpp"
#include "../objects/physicObject.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

CCollisionLine::CCollisionLine(CPhysicObject* owner, const vector2d& point2, const vector2d& shift) : inherited(owner, shift) {
	COLLISION_TYPE = ECOLLISION_LINE;

	_pointTwo = point2;
}

void CCollisionLine::updatePreCollisionRect(const vector2d& displacement) {
	// позиция
	_point = _owner->getPos() + _shift;

	// перемещение в кадре
	_displacement = displacement;

	auto point2 = _point + _pointTwo;
	// прямоугольник пре коллизии
	_preCollisionRect.left	= fmin(fmin(_point.getX(), point2.getX()), fmin(_point.getX() + _displacement.getX(), point2.getX() + _displacement.getX()));
	_preCollisionRect.top	= fmin(fmin(_point.getY(), point2.getY()), fmin(_point.getY() + _displacement.getY(), point2.getY() + _displacement.getY()));
	_preCollisionRect.width = fmax(fmax(_point.getX(), point2.getX()), fmax(_point.getX() + _displacement.getX(), point2.getX() + _displacement.getX())) - _preCollisionRect.left;
	_preCollisionRect.height= fmax(fmax(_point.getY(), point2.getY()), fmax(_point.getY() + _displacement.getY(), point2.getY() + _displacement.getY())) - _preCollisionRect.top;
}

bool CCollisionLine::draw(sf::RenderWindow* window, const vector2d& viewpoint, double pixelToMeters) const {
	sf::Color movRectColor = _preCollisionOn ? sf::Color(255, 200, 120) : sf::Color(255, 160, 0);
	sf::Color startColor(255, 20, 20);
	sf::Color endColor(100, 255, 20);

	sf::RectangleShape shapeP;
	shapeP.setFillColor(startColor);
	shapeP.setSize(sf::Vector2f(7.0f, 7.0f));

	sf::RectangleShape shapePEnd;
	shapePEnd.setFillColor(endColor);
	shapePEnd.setSize(sf::Vector2f(5.0f, 5.0f));

	sf::VertexArray line(sf::PrimitiveType::Lines, 2);		// линия
	sf::VertexArray line1Mov(sf::PrimitiveType::Lines, 2);	// перемещение точки 1
	sf::VertexArray line2Mov(sf::PrimitiveType::Lines, 2);	// перемещение точки 2
	sf::VertexArray lineEnd(sf::PrimitiveType::Lines, 2);	// перемещенная линия

	// прямоугольник перемещения
	sf::RectangleShape rectMov;
	rectMov.setFillColor(sf::Color(0, 0, 0, 0));
	rectMov.setOutlineColor(movRectColor);
	if (_preCollisionOn)
		rectMov.setOutlineThickness(4.0f);
	else
		rectMov.setOutlineThickness(2.0f);
	
	// получаем координаты относительно камеры
	auto temp = _point;
	auto temp2 = _pointTwo + temp;
	auto temp1Mov = _point; temp1Mov.add(_displacement);	// перемещенная точка 1
	auto temp2Mov = temp2; temp2Mov.add(_displacement); // перемещенная точка 2

	sf::Rect<double> movingRect = _preCollisionRect;


	temp.sub(viewpoint);
	temp2.sub(viewpoint);
	temp1Mov.sub(viewpoint);
	temp2Mov.sub(viewpoint);
	movingRect.left -= viewpoint.getX();
	movingRect.top -= viewpoint.getY();


	// переводим в пиксели
	temp.mul(pixelToMeters);
	temp2.mul(pixelToMeters);
	temp1Mov.mul(pixelToMeters);
	temp2Mov.mul(pixelToMeters);
	movingRect.left *= pixelToMeters;
	movingRect.top *= pixelToMeters;
	movingRect.width *= pixelToMeters;
	movingRect.height *= pixelToMeters;

	// добавляем пол экрана
	temp.add(window->getView().getCenter());
	temp2.add(window->getView().getCenter());
	temp1Mov.add(window->getView().getCenter());
	temp2Mov.add(window->getView().getCenter());
	movingRect.left += window->getView().getCenter().x;
	movingRect.top += window->getView().getCenter().y;

	line[0] = temp.asSfmlVecF();
	line[1] = temp2.asSfmlVecF();
	line[0].color = startColor;
	line[1].color = startColor;

	line1Mov[0] = temp.asSfmlVecF();
	line1Mov[1] = temp1Mov.asSfmlVecF();
	line1Mov[0].color = startColor;
	line1Mov[1].color = endColor;

	line2Mov[0] = temp2.asSfmlVecF();
	line2Mov[1] = temp2Mov.asSfmlVecF();
	line2Mov[0].color = startColor;
	line2Mov[1].color = endColor;

	lineEnd[0] = temp1Mov.asSfmlVecF();
	lineEnd[1] = temp2Mov.asSfmlVecF();
	lineEnd[0].color = endColor;
	lineEnd[1].color = endColor;

	rectMov.setPosition(movingRect.left, movingRect.top);
	rectMov.setSize(sf::Vector2f(movingRect.width, movingRect.height));


	// мутим интерсект
	sf::FloatRect rect = { 0.0f, 0.0f, window->getView().getSize().x, window->getView().getSize().y };
	if (rect.intersects(sf::FloatRect(rectMov.getPosition(), rectMov.getSize()))) {
		window->draw(rectMov);
		window->draw(line);
		window->draw(line1Mov);
		window->draw(line2Mov);
		window->draw(lineEnd);
		temp.add(vector2d(-3.5, -3.5));
		shapeP.setPosition(temp.asSfmlVecF());
		window->draw(shapeP);
		temp2.add(vector2d(-3.5, -3.5));
		shapeP.setPosition(temp2.asSfmlVecF());
		window->draw(shapeP);
		temp1Mov.add(vector2d(-2.5, -2.5));
		shapePEnd.setPosition(temp1Mov.asSfmlVecF());
		window->draw(shapePEnd);
		temp2Mov.add(vector2d(-2.5, -2.5));
		shapePEnd.setPosition(temp2Mov.asSfmlVecF());
		window->draw(shapePEnd);
		return true;
	}
	return false;
}