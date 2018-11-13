#ifndef _ANIMATED_RECT_HPP_
#define _ANIMATED_RECT_HPP_

#include <SFML/Graphics.hpp>
#include "new_types/sharedString.hpp"
#include "types.hpp"
#include "timer.hpp"
#include <vector>


// Todo: раньше наследовался от Drawable, теперь от rectangle shape. требуется реализация использующая методы предков")
class CAnimatedRect : public sf::RectangleShape {
private:
	typedef sf::RectangleShape inherited;

protected:
	sf::Vector2u		_size;
	sf::Vector2u		_position;
	mutable sf::RectangleShape	_shape;
	mutable zog::CTimer::Timestamp _lastTime;
	mutable size_t _curFrame;
	size_t _delay;
	size_t _maxFrames;
	size_t _frameCount;
	size_t _framesX;
	size_t _framesY;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	CAnimatedRect();
	explicit CAnimatedRect(const sf::Vector2f& size);
	
	void setPosition		(const sf::Vector2f& pos);
	void setPosition		(float x, float y);
	void setSize			(const sf::Vector2f& size);
	void setScale			(const sf::Vector2f& factor);
	void scale				(const sf::Vector2f& factor);
	void setScale			(float x, float y);
	void scale				(float x, float y);
	void move				(const sf::Vector2f& offset);
	void move				(float offsetX, float offsetY);
	void setRotation		(float angle);
	void rotate				(float angle);
	void setOrigin			(const sf::Vector2f& origin);
	void setOrigin			(float x, float y);
	void setOutlineColor	(const sf::Color& color);
	void setOutlineThickness(float thickness);

	sf::FloatRect			getGlobalBounds() const;
	sf::FloatRect			getLocalBounds() const;
	const sf::Transform&	getInverseTransform() const;
	const sf::Transform&	getTransform() const;
	const sf::Vector2f&		getOrigin() const;
	const sf::Color&		getOutlineColor() const;
	float					getOutlineThickness() const;
	sf::Vector2f			getPoint(size_t index) const;
	size_t					getPointCount() const;
	const sf::Vector2f&		getPosition() const;
	float					getRotation() const;
	const sf::Vector2f&		getScale() const;
	const sf::Vector2f&		getSize() const;


	
	void create(
		const sf::Texture* texture,
		const sf::Vector2u& frameSize,
		size_t delayMs,
		size_t framesCount,
		const sf::Vector2u& startPos = sf::Vector2u(0, 0)
	);
	void setAreaStart		(const sf::Vector2u& startPos);
	void setFramesCount		(size_t count);
	void restart			();
};




class CAnimatedRectControlPoints : public CAnimatedRect {
private:
	typedef CAnimatedRect inherited;

public:
	CAnimatedRectControlPoints() : inherited() {}
	explicit CAnimatedRectControlPoints(const sf::Vector2f& size) : inherited(size) {}

protected:
	std::vector<std::vector<sf::Vector2u>> _controlPoints;    // points arrays for animations

public:
	void loadControlPoints(const shared_str& animName);			// add control point by anim name;
	sf::Vector2u getControlPoint(U32 control_point_id) const;	// get control point from id for current frame. must be called after draw



};


#endif //_ANIMATED_RECT_HPP_