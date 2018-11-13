#include "AnimatedRect.hpp"
#include "Asserts.hpp"
#include "cfgSystem.hpp"


CAnimatedRect::CAnimatedRect() {
	_lastTime = zog::timer().timestamp();
	_delay = 100;
	_framesX = 1;
	_framesY = 1;
	_frameCount = 1;
	_maxFrames = 1;
	_curFrame = 0;
	_position.x = 0; _position.y = 0;
}


CAnimatedRect::CAnimatedRect(const sf::Vector2f& size) : CAnimatedRect() {
	_shape.setSize(size);
}


void CAnimatedRect::create(const sf::Texture* texture, const sf::Vector2u& size, size_t delayMs, size_t framesCount, const sf::Vector2u& startPos) {
	_shape.setTexture(texture);
	_position = startPos;
	_shape.setTextureRect(sf::IntRect(_position.x, _position.y, size.x, size.y));
	_size = size;
	_delay = delayMs;
	auto textureSize = texture->getSize();
	_framesX = (textureSize.x - _position.x) / _size.x;
	_framesY = (textureSize.y - _position.y) / _size.y;
	_maxFrames = _framesX * _framesY;
	setFramesCount(framesCount);
	_curFrame = 0;
	printf("create animation: pos: [%i, %i], real_frame_size: [%i, %i],\n texture_size: [%i, %i], _framesX: %li _framesY: %li framesCount: %li\n", _position.x, _position.y, _size.x, _size.y, textureSize.x, textureSize.y, _framesX, _framesY, framesCount);
}


void CAnimatedRect::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto now = zog::timer().timestamp();
	size_t time = static_cast<size_t>((now - _lastTime).milli());
	if (time > _delay) {
		_curFrame += time / _delay;
		_curFrame %= _frameCount;
		size_t y = _curFrame / _framesX;
		size_t x = _curFrame - y * _framesX;
		_shape.setTextureRect(sf::IntRect(x * _size.x + _position.x, y * _size.y + _position.y, _size.x, _size.y));
		_lastTime = now;
	}
	target.draw(_shape, states);
}



void CAnimatedRect::setPosition			(const sf::Vector2f& pos)		{ _shape.setPosition(pos); }
void CAnimatedRect::setPosition			(float x, float y)				{ _shape.setPosition(x, y); }
void CAnimatedRect::setSize				(const sf::Vector2f& size)		{ _shape.setSize(size); }
void CAnimatedRect::setScale			(const sf::Vector2f& factor)	{ _shape.setScale(factor); }
void CAnimatedRect::scale				(const sf::Vector2f& factor)	{ _shape.scale(factor); }
void CAnimatedRect::setScale			(float factorX, float factorY)	{ _shape.setScale(factorX, factorY); }
void CAnimatedRect::scale				(float factorX, float factorY)	{ _shape.scale(factorX, factorY); }
void CAnimatedRect::move				(const sf::Vector2f& offset)	{ _shape.move(offset); }
void CAnimatedRect::move				(float offsetX, float offsetY)	{ _shape.move(offsetX, offsetY); }
void CAnimatedRect::setRotation			(float angle)					{ _shape.setRotation(angle); }
void CAnimatedRect::rotate				(float angle)					{ _shape.rotate(angle); }
void CAnimatedRect::setOrigin			(const sf::Vector2f& origin)	{ _shape.setOrigin(origin); }
void CAnimatedRect::setOrigin			(float x, float y)				{ _shape.setOrigin(x, y); }
void CAnimatedRect::setOutlineColor		(const sf::Color& color)		{ _shape.setOutlineColor(color); }
void CAnimatedRect::setOutlineThickness	(float thickness)				{ _shape.setOutlineThickness(thickness); }


sf::FloatRect			CAnimatedRect::getGlobalBounds() const			{ return _shape.getGlobalBounds(); }
sf::FloatRect			CAnimatedRect::getLocalBounds() const			{ return _shape.getLocalBounds(); }
const sf::Transform&	CAnimatedRect::getInverseTransform() const		{ return _shape.getInverseTransform(); }
const sf::Transform&	CAnimatedRect::getTransform() const				{ return _shape.getTransform(); }
const sf::Vector2f&		CAnimatedRect::getOrigin() const				{ return _shape.getOrigin(); }
const sf::Color&		CAnimatedRect::getOutlineColor() const			{ return _shape.getOutlineColor(); }
float					CAnimatedRect::getOutlineThickness() const		{ return _shape.getOutlineThickness(); }
sf::Vector2f			CAnimatedRect::getPoint(size_t index) const		{ return _shape.getPoint(index); }
size_t					CAnimatedRect::getPointCount() const			{ return _shape.getPointCount(); }
const sf::Vector2f&		CAnimatedRect::getPosition() const				{ return _shape.getPosition(); }
float					CAnimatedRect::getRotation() const				{ return _shape.getRotation(); }
const sf::Vector2f&		CAnimatedRect::getScale() const					{ return _shape.getScale(); }
const sf::Vector2f&		CAnimatedRect::getSize() const					{ return _shape.getSize(); }


void CAnimatedRect::setAreaStart(const sf::Vector2u& startPos) {
	_position = startPos;
}


void CAnimatedRect::setFramesCount(size_t count) {
	if (count > _maxFrames) {
		_frameCount = _maxFrames;
		return;
	}
	_frameCount = count;
}

void CAnimatedRect::restart() {
	_curFrame = 0;
	_shape.setTextureRect(sf::IntRect(_position.x, _position.y, _size.x, _size.y));
	_lastTime = zog::timer().timestamp();
}


void CAnimatedRectControlPoints::loadControlPoints(const shared_str& animName) {
	size_t controlPointId = 0;

	while (true) {
		// получаем имя секции контрол поинтов
		shared_str cpName;
		cpName.sprintf("%s_cp%i", animName.c_str(), controlPointId);

		// если секции с контрол поинтами закончились - заканчиваем загрузку
		if (!zog::readConfig().is_section_exist(cpName))
			return;

		// загружаем контрол поенты для секции
		std::vector<sf::Vector2u> points;
		for (size_t i = 0; i < _frameCount; ++i) {
			shared_str num;
			num.sprintf("%i", i);
			auto tmp = zog::readConfig().r_array<U32>(cpName, num);
			points.emplace_back(sf::Vector2u(tmp.at(0), tmp.at(1)));
		}
	}
}


sf::Vector2u CAnimatedRectControlPoints::getControlPoint(U32 control_point_id) const {
	ASSERTF(control_point_id < _controlPoints.size(), "Can't find control %i point", control_point_id);
	return _controlPoints.at(control_point_id).at(_curFrame);
}