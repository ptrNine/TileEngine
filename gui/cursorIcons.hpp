#ifndef _CURSOR_ICONS_HPP_
#define _CURSOR_ICONS_HPP_

#include "SFML/Graphics.hpp"
#include "../base/types.hpp"


class CCursorIcons {
private:
	static const U32	_iconsCount = 3;	// 0 - regular, 1 - text
	sf::Texture*		_icons[_iconsCount];
	sf::RectangleShape	_sprite;
	sf::RenderStates	_renderStates;

	CCursorIcons();
	~CCursorIcons();
public:
	CCursorIcons(const CCursorIcons&) = delete;
	CCursorIcons& operator= (const CCursorIcons&) = delete;

public:
	enum class Type {
		regular,
		text,
		button
	};


	void drawCursor(sf::RenderWindow& wnd);
	void setType(Type type);
	void resize(const sf::Vector2f& factors);


	static CCursorIcons& instance() {
		static CCursorIcons inst;
		return inst;
	}

};


#endif //_CURSOR_ICONS_HPP_