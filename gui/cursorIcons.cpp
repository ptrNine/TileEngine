#include "cursorIcons.hpp"
#include "../textureManager.hpp"


CCursorIcons::CCursorIcons() {
    _icons[0] = base::TextureManager().load("ui/mouse_regular.png");
    _icons[1] = base::TextureManager().load("ui/mouse_text.png");
	_icons[2] = base::TextureManager().load("ui/mouse_button.png");
	_sprite.setSize(sf::Vector2f(30.0f, 30.0f));
	_sprite.setTexture(_icons[0]);
}

CCursorIcons::~CCursorIcons() {
}


void CCursorIcons::drawCursor(sf::RenderWindow& wnd) {
	auto pos = sf::Mouse::getPosition(wnd);
	_sprite.setPosition(float(pos.x), float(pos.y));
	wnd.draw(_sprite, _renderStates);
}

// Todo: create functions to set origin for icons
void CCursorIcons::setType(Type type) {
	switch (type)
	{
	case Type::regular:
		_sprite.setTexture(_icons[0]);
		_sprite.setOrigin(0.0f, 0.0f);
		break;
	case Type::text:
		_sprite.setTexture(_icons[1]);
		_sprite.setOrigin(3.0f, 10.0f);
		break;
	case Type::button:
		_sprite.setTexture(_icons[2]);
		_sprite.setOrigin(0.0f, 0.0f);
		break;
	}
}


void CCursorIcons::resize(const sf::Vector2f& factors) {
	_renderStates.transform = sf::Transform();
	_renderStates.transform.scale(factors);
}

