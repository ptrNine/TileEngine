#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

#include <SFML/Graphics.hpp>
#include "../base/types.hpp"

class CPanel;
class CPanelBase;
class CWindow;

class CComponent {
public:
	// aligmrnt parameters for components
	class Aligment {
	public:
		enum {
			none			= 0U,
			right			= 1U,
			bottom			= 2U,
			resizeRight		= 4U,
			resizeBottom	= 8U,
			movable			= right | bottom,
			rightResizable	= right | resizeRight,
			bottomResizable = bottom | resizeBottom,
			resizable		= rightResizable | bottomResizable
		};
	};

protected:
	U32				_align;
	CComponent*		_parent;

protected:
	virtual void update		(CWindow& wnd) = 0;
	virtual void draw		(CWindow& wnd) = 0;

public:
	CComponent			() { _parent = nullptr; _align = 0; }
	virtual ~CComponent	() = default;

	// Base functions
	virtual void setAligment	(U32 align);
	virtual void setPos			(const sf::Vector2f& pos) = 0;
	virtual void setSize		(const sf::Vector2f& size) = 0;
	virtual void move			(const sf::Vector2f& offset) = 0;
	virtual void resize			(const sf::Vector2f& factors, const sf::Vector2f& offset) = 0;

	// For update and draw
	friend CWindow;

	// Make adding parents possible
	friend CPanel;
	friend CPanelBase;
};

#endif //_COMPONENT_HPP_