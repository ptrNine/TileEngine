#include "scrollBar.hpp"
#include "window.hpp"
#include "../base/io/mouse.hpp"


CScrollBar::CScrollBar(const sf::FloatRect& rect, bool isHorizontal, U32 align) {
	_isHorizontal = isHorizontal;
	_onScrolling = false;

	_shape.setFillColor(sf::Color(210, 210, 210));
	_shape.setOutlineColor(sf::Color(40, 40, 40));
	_shape.setOutlineThickness(2.0f);

	_buttonShape1.setFillColor(sf::Color(150, 150, 150));
	_buttonShape1.setOutlineColor(sf::Color(40, 40, 40));
	_buttonShape1.setOutlineThickness(2.0f);

	_buttonShape2.setFillColor(sf::Color(150, 150, 150));
	_buttonShape2.setOutlineColor(sf::Color(40, 40, 40));
	_buttonShape2.setOutlineThickness(2.0f);

	_movableShape.setFillColor(sf::Color(150, 150, 150));
	_movableShape.setOutlineColor(sf::Color(40, 40, 40));
	_movableShape.setOutlineThickness(2.0f);

	setPos(sf::Vector2f(rect.left, rect.top));
	setSize(sf::Vector2f(rect.width, rect.height));
	setAligment(align);
}

void CScrollBar::update(CWindow& wnd) {
	auto min = _isHorizontal ? _buttonShape1.getPosition().x + _buttonShape1.getSize().x : _buttonShape1.getPosition().y + _buttonShape1.getSize().y;
	auto max = _isHorizontal ? 
		_buttonShape2.getPosition().x - _movableShape.getSize().x : 
		_buttonShape2.getPosition().y - _movableShape.getSize().y;

	auto mouse_pos = zog::mouse().getPositionF();
	auto mov_shape_rect = sf::FloatRect(_movableShape.getPosition(), _movableShape.getSize());

	if (zog::mouse().isPressed(0) && mov_shape_rect.contains(mouse_pos) && !zog::mouse().isCaptured()) {
		_onScrolling = true;
		zog::mouse()._capture();
	}

	if (zog::mouse().isReleased(0) && _onScrolling) {
		_onScrolling = false;
		zog::mouse()._capture();
	}

	if (zog::mouse().isHolding(0) && _onScrolling && !zog::mouse().isCaptured()) {
		auto pos = _movableShape.getPosition();

		if (_isHorizontal) {
			pos.x = mouse_pos.x - mov_shape_rect.width / 2.0f;
			if (pos.x < min) pos.x = min;
			else if (pos.x > max) pos.x = max;
		}
		else {
			pos.y = mouse_pos.y - mov_shape_rect.height / 2.0f;
			if (pos.y < min) pos.y = min;
			else if (pos.y > max) pos.y = max;
		}

		_movableShape.setPosition(pos);

		zog::mouse()._capture();
	}
}

void CScrollBar::draw(CWindow& wnd) {
	wnd.draw(_buttonShape1, _transform);
	wnd.draw(_shape, _transform);
	wnd.draw(_buttonShape2, _transform);
	wnd.draw(_movableShape, _transform);
}


void CScrollBar::setButtonShape(const sf::RectangleShape& shape) {
	auto pos1	= _buttonShape1.getPosition();
	auto size1	= _buttonShape1.getSize();
	auto pos2	= _buttonShape2.getPosition();
	auto size2	= _buttonShape2.getSize();

	_buttonShape1 = shape;
	_buttonShape1.setPosition(pos1);
	_buttonShape1.setSize(size1);

	_buttonShape2 = shape;
	_buttonShape2.setRotation(180.0f);
	_buttonShape2.setPosition(pos2);
	_buttonShape2.setSize(size2);
}

void CScrollBar::setMoveAreaShape(const sf::RectangleShape& shape) {	
	auto pos	= _shape.getPosition();
	auto size	= _shape.getSize();

	_shape = shape;
	_shape.setPosition(pos);
	_shape.setSize(size);
}

void CScrollBar::setMovableShape(const sf::RectangleShape& shape) {		
	auto pos	= _movableShape.getPosition();
	auto size	= _movableShape.getSize();

	_movableShape = shape;
	_movableShape.setPosition(pos);
	_movableShape.setSize(size);
}


sf::FloatRect CScrollBar::getRect() const {
	auto pos1 = _buttonShape1.getPosition();
	auto pos2 = _buttonShape2.getPosition() + _buttonShape2.getSize();

	return sf::FloatRect(pos1, pos2 - pos1);
}

float CScrollBar::getProgress() const {
	if (_isHorizontal)
		return (_movableShape.getPosition().x - _shape.getPosition().x) / (_shape.getSize().x - _movableShape.getSize().x);
	else
		return (_movableShape.getPosition().y - _shape.getPosition().y) / (_shape.getSize().y - _movableShape.getSize().y);
}

float CScrollBar::getStartItem(U32 items_count, U32 show_size) const {
	return getProgress() * (items_count - show_size + 1);
}

sf::Vector2f CScrollBar::getPos() const {
	return _buttonShape1.getPosition();
}

sf::Vector2f CScrollBar::getSize() const {
	auto pos1 = _buttonShape1.getPosition();
	auto pos2 = _buttonShape2.getPosition() + _buttonShape2.getSize();
	return sf::Vector2f(pos2 - pos1);
}

void CScrollBar::setPos(const sf::Vector2f& pos) {
	auto mov = pos - _buttonShape1.getPosition();
	_buttonShape1.move(mov);
	_shape.move(mov);
	_buttonShape2.move(mov);
	_movableShape.move(mov);
}

void CScrollBar::setSize(const sf::Vector2f& size) {
	if (_isHorizontal) {
		printf("true");
		float areaX = size.x - size.y * 2;
		if (areaX < 50.0f) areaX = 50.0f;

		auto shapePos = _shape.getPosition();
		auto btn1Pos = _buttonShape1.getPosition();
		auto btn2Pos = _buttonShape2.getPosition();
		auto movablePos = _movableShape.getPosition();

		shapePos.x = btn1Pos.x + size.y;
		btn2Pos.x = shapePos.x + areaX;
		if (_movableShape.getSize().x > 0.0f)
			movablePos.x += shapePos.x + getProgress() * (areaX - _movableShape.getSize().x);
		else
			movablePos.x += shapePos.x;

		_buttonShape2.setPosition(btn2Pos);
		_shape.setPosition(shapePos);
		_movableShape.setPosition(movablePos);

		_buttonShape1.setSize(sf::Vector2f(size.y, size.y));
		_buttonShape2.setSize(sf::Vector2f(size.y, size.y));
		if (_movableShape.getSize().x > 0.0f && _shape.getSize().x > 0.0f)
			_movableShape.setSize(sf::Vector2f(size.y, (_movableShape.getSize().x / _shape.getSize().x) * areaX));
		else
			_movableShape.setSize(sf::Vector2f(size.y, (areaX / 10.0f)));
		_shape.setSize(sf::Vector2f(size.y, areaX));
	}
	else {
		float areaY = size.y - size.x * 2;
		if (areaY < 50.0f) areaY = 50.0f;

		auto shapePos = _shape.getPosition();
		auto btn1Pos = _buttonShape1.getPosition();
		auto btn2Pos = _buttonShape2.getPosition();
		auto movablePos = _movableShape.getPosition();

		shapePos.y = btn1Pos.y + size.x;
		btn2Pos.y = shapePos.y + areaY;
		if (_movableShape.getSize().y > 0.0f)
			movablePos.y = shapePos.y + getProgress() * (areaY - _movableShape.getSize().y);
		else
			movablePos.y = shapePos.y;

		_buttonShape2.setPosition(btn2Pos);
		_shape.setPosition(shapePos);
		_movableShape.setPosition(movablePos);

		_buttonShape1.setSize(sf::Vector2f(size.x, size.x));
		_buttonShape2.setSize(sf::Vector2f(size.x, size.x));
		if (_movableShape.getSize().y > 0.0f && _shape.getSize().y > 0.0f)
			_movableShape.setSize(sf::Vector2f(size.x, (_movableShape.getSize().y / _shape.getSize().y) * areaY));
		else
			_movableShape.setSize(sf::Vector2f(size.x, (areaY / 10.0f)));

		_shape.setSize(sf::Vector2f(size.x, areaY));
	}
}
