#include "collisionRectangle.hpp"
#include "../objects/physicObject.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


CCollisionRectangle::CCollisionRectangle(CPhysicObject* owner, const vector2d& size, const vector2d& shift) : inherited(owner, shift) {
	COLLISION_TYPE = ECOLLISION_RECTANGLE;

	_point2.set(size.getX(), 0.0);
	_point3.set(size);
	_point4.set(0.0, size.getY());
}

void CCollisionRectangle::updatePreCollisionRect(const vector2d& displacement) {
	// позиция
	_point = _owner->getPos() + _shift;

	// перемещение в кадре
	_displacement = displacement;

	// прямоугольник пре коллизии
	vector2d points[8] = {
		_point, _point2 + _point, _point3 + _point, _point4 + _point,
		_point + displacement, _point2 + _point + displacement, 
		_point3 + _point + displacement, _point4 + _point + displacement
	};

	_preCollisionRect.left		= std::min_element(&points[0], &points[8], [&](const vector2d& l, const vector2d& r) { return l.getX() > r.getX(); })->getX();
	_preCollisionRect.top		= std::min_element(&points[0], &points[8], [&](const vector2d& l, const vector2d& r) { return l.getY() > r.getY(); })->getY();
	_preCollisionRect.width		= std::max_element(&points[0], &points[8], [&](const vector2d& l, const vector2d& r) { return l.getX() > r.getX(); })->getX() - _preCollisionRect.left;
	_preCollisionRect.height	= std::max_element(&points[0], &points[8], [&](const vector2d& l, const vector2d& r) { return l.getY() > r.getY(); })->getY() - _preCollisionRect.top;
}

void CCollisionRectangle::setSize(const vector2d& size) {
	_point2.setX(size);
	_point3.set(size);
	_point4.setY(size);
	updatePreCollisionRect(vector2d(0.0, 0.0));
}

bool CCollisionRectangle::draw(sf::RenderWindow* window, const vector2d& viewpoint, double pixelToMeters) const {
	//printf("%f, %f, %f, %f, %f, %f\n", _point2.getX(), _point2.getY(), _point3.getX(), _point3.getY(), _point4.getX(), _point4.getY());
	sf::Color movRectColor = _preCollisionOn ? sf::Color(255, 200, 120) : sf::Color(255, 160, 0);
	sf::Color startColor(255, 20, 20);
	sf::Color endColor(100, 255, 20);

	sf::RectangleShape shapeP;
	shapeP.setFillColor(startColor);
	shapeP.setSize(sf::Vector2f(7.0f, 7.0f));

	sf::RectangleShape shapePEnd;
	shapePEnd.setFillColor(endColor);
	shapePEnd.setSize(sf::Vector2f(5.0f, 5.0f));

	sf::VertexArray line1(sf::PrimitiveType::Lines, 2);		// линия 1
	sf::VertexArray line2(sf::PrimitiveType::Lines, 2);		// линия 2
	sf::VertexArray line3(sf::PrimitiveType::Lines, 2);		// линия 3
	sf::VertexArray line4(sf::PrimitiveType::Lines, 2);		// линия 4
	sf::VertexArray line1Mov(sf::PrimitiveType::Lines, 2);	// перемещение точки 1
	sf::VertexArray line2Mov(sf::PrimitiveType::Lines, 2);	// перемещение точки 2
	sf::VertexArray line3Mov(sf::PrimitiveType::Lines, 2);	// перемещение точки 3
	sf::VertexArray line4Mov(sf::PrimitiveType::Lines, 2);	// перемещение точки 4
	sf::VertexArray lineEnd1(sf::PrimitiveType::Lines, 2);	// перемещенная линия 1
	sf::VertexArray lineEnd2(sf::PrimitiveType::Lines, 2);	// перемещенная линия 2
	sf::VertexArray lineEnd3(sf::PrimitiveType::Lines, 2);	// перемещенная линия 3
	sf::VertexArray lineEnd4(sf::PrimitiveType::Lines, 2);	// перемещенная линия 4

															// прямоугольник перемещения
	sf::RectangleShape rectMov;
	rectMov.setFillColor(sf::Color(0, 0, 0, 0));
	rectMov.setOutlineColor(movRectColor);
	if (_preCollisionOn)
		rectMov.setOutlineThickness(4.0f);
	else
		rectMov.setOutlineThickness(2.0f);

	auto tpoint1 = _point;
	auto tpoint2 = tpoint1 + _point2;
	auto tpoint3 = tpoint1 + _point3;
	auto tpoint4 = tpoint1 + _point4;
	auto tMovPoint1 = _point + _displacement;
	auto tMovPoint2 = tpoint2 + _displacement;
	auto tMovPoint3 = tpoint3 + _displacement;
	auto tMovPoint4 = tpoint4 + _displacement;


	//printf("[%f, %f][%f, %f][%f, %f][%f, %f]\n", tpoint1.getX(), tpoint1.getY(), tpoint2.getX(), tpoint2.getY(), tpoint3.getX(), tpoint3.getY(), tpoint4.getX(), tpoint4.getY());

	sf::Rect<double> movingRect = _preCollisionRect;

	//printf("[%f, %f, %f, %f\n", movingRect.left, movingRect.top, movingRect.width, movingRect.height);

	// получаем координаты относительно камеры
	tpoint1.sub(viewpoint);
	tpoint2.sub(viewpoint);
	tpoint3.sub(viewpoint);
	tpoint4.sub(viewpoint);
	tMovPoint1.sub(viewpoint);
	tMovPoint2.sub(viewpoint);
	tMovPoint3.sub(viewpoint);
	tMovPoint4.sub(viewpoint);
	movingRect.left -= viewpoint.getX();
	movingRect.top -= viewpoint.getY();


	// переводим в пиксели
	tpoint1.mul(pixelToMeters);
	tpoint2.mul(pixelToMeters);
	tpoint3.mul(pixelToMeters);
	tpoint4.mul(pixelToMeters);
	tMovPoint1.mul(pixelToMeters);
	tMovPoint2.mul(pixelToMeters);
	tMovPoint3.mul(pixelToMeters);
	tMovPoint4.mul(pixelToMeters);
	movingRect.left *= pixelToMeters;
	movingRect.top *= pixelToMeters;
	movingRect.width *= pixelToMeters;
	movingRect.height *= pixelToMeters;

	// добавляем пол экрана
	tpoint1.add(window->getView().getCenter());
	tpoint2.add(window->getView().getCenter());
	tpoint3.add(window->getView().getCenter());
	tpoint4.add(window->getView().getCenter());
	tMovPoint1.add(window->getView().getCenter());
	tMovPoint2.add(window->getView().getCenter());
	tMovPoint3.add(window->getView().getCenter());
	tMovPoint4.add(window->getView().getCenter());
	movingRect.left += window->getView().getCenter().x;
	movingRect.top += window->getView().getCenter().y;

	line1[0] = tpoint1.asSfmlVecF();
	line1[1] = tpoint2.asSfmlVecF();
	line1[0].color = startColor;
	line1[1].color = startColor;

	line2[0] = tpoint2.asSfmlVecF();
	line2[1] = tpoint3.asSfmlVecF();
	line2[0].color = startColor;
	line2[1].color = startColor;

	line3[0] = tpoint3.asSfmlVecF();
	line3[1] = tpoint4.asSfmlVecF();
	line3[0].color = startColor;
	line3[1].color = startColor;

	line4[0] = tpoint1.asSfmlVecF();
	line4[1] = tpoint4.asSfmlVecF();
	line4[0].color = startColor;
	line4[1].color = startColor;

	line1Mov[0] = tpoint1.asSfmlVecF();
	line1Mov[1] = tMovPoint1.asSfmlVecF();
	line1Mov[0].color = startColor;
	line1Mov[1].color = endColor;

	line2Mov[0] = tpoint2.asSfmlVecF();
	line2Mov[1] = tMovPoint2.asSfmlVecF();
	line2Mov[0].color = startColor;
	line2Mov[1].color = endColor;

	line3Mov[0] = tpoint3.asSfmlVecF();
	line3Mov[1] = tMovPoint3.asSfmlVecF();
	line3Mov[0].color = startColor;
	line3Mov[1].color = endColor;

	line4Mov[0] = tpoint4.asSfmlVecF();
	line4Mov[1] = tMovPoint4.asSfmlVecF();
	line4Mov[0].color = startColor;
	line4Mov[1].color = endColor;

	lineEnd1[0] = tMovPoint1.asSfmlVecF();
	lineEnd1[1] = tMovPoint2.asSfmlVecF();
	lineEnd1[0].color = endColor;
	lineEnd1[1].color = endColor;

	lineEnd2[0] = tMovPoint2.asSfmlVecF();
	lineEnd2[1] = tMovPoint3.asSfmlVecF();
	lineEnd2[0].color = endColor;
	lineEnd2[1].color = endColor;

	lineEnd3[0] = tMovPoint3.asSfmlVecF();
	lineEnd3[1] = tMovPoint4.asSfmlVecF();
	lineEnd3[0].color = endColor;
	lineEnd3[1].color = endColor;

	lineEnd4[0] = tMovPoint1.asSfmlVecF();
	lineEnd4[1] = tMovPoint4.asSfmlVecF();
	lineEnd4[0].color = endColor;
	lineEnd4[1].color = endColor;

	rectMov.setPosition(movingRect.left, movingRect.top);
	rectMov.setSize(sf::Vector2f(movingRect.width, movingRect.height));


	// мутим интерсект
	sf::FloatRect rect = { 0.0f, 0.0f, window->getView().getSize().x, window->getView().getSize().y };
	if (rect.intersects(sf::FloatRect(rectMov.getPosition(), rectMov.getSize()))) {
		window->draw(rectMov);
		window->draw(line1);
		window->draw(line2);
		window->draw(line3);
		window->draw(line4);
		window->draw(line1Mov);
		window->draw(line2Mov);
		window->draw(line3Mov);
		window->draw(line4Mov);
		window->draw(lineEnd1);
		window->draw(lineEnd2);
		window->draw(lineEnd3);
		window->draw(lineEnd4);
		tpoint1.add(vector2d(-3.5, -3.5));	shapeP.setPosition(tpoint1.asSfmlVecF());	window->draw(shapeP);
		tpoint2.add(vector2d(-3.5, -3.5));	shapeP.setPosition(tpoint2.asSfmlVecF());	window->draw(shapeP);
		tpoint3.add(vector2d(-3.5, -3.5));	shapeP.setPosition(tpoint3.asSfmlVecF());	window->draw(shapeP);
		tpoint4.add(vector2d(-3.5, -3.5));	shapeP.setPosition(tpoint4.asSfmlVecF());	window->draw(shapeP);
		tMovPoint1.add(vector2d(-3.5, -3.5));	shapePEnd.setPosition(tMovPoint1.asSfmlVecF());	window->draw(shapePEnd);
		tMovPoint2.add(vector2d(-3.5, -3.5));	shapePEnd.setPosition(tMovPoint2.asSfmlVecF());	window->draw(shapePEnd);
		tMovPoint3.add(vector2d(-3.5, -3.5));	shapePEnd.setPosition(tMovPoint3.asSfmlVecF());	window->draw(shapePEnd);
		tMovPoint4.add(vector2d(-3.5, -3.5));	shapePEnd.setPosition(tMovPoint4.asSfmlVecF());	window->draw(shapePEnd);
		return true;
	}
	return false;
}