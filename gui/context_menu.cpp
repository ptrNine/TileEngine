#include "context_menu.hpp"
#include "window.hpp"
#include "../base/io/mouse.hpp"


CContextMenu::CContextMenu() : inherited() {
	_isShown = false;
}

CContextMenu::CContextMenu(const sf::FloatRect& rect, U32 align) : inherited(rect, 1, align) {
	_isShown = false;
}


void CContextMenu::show() {
	_isShown = true;
}

void CContextMenu::close() {
	_isShown = false;
}

bool CContextMenu::isShown() {
	return _isShown;
}


void CContextMenu::update(CWindow& wnd) {
	if (_isShown) {
		inherited::update(wnd);

		bool onMouse = _shape.getGlobalBounds().contains(zog::mouse().getPositionF());
		if (onMouse && zog::mouse().isReleased(sf::Mouse::Left)) {
			close();
		}
		else if (!onMouse && (zog::mouse().isPressed(sf::Mouse::Left) || zog::mouse().isReleased(sf::Mouse::Left))) {
			close();
		}
	}
}

void CContextMenu::draw(CWindow& wnd) {
	if (_isShown) {
		inherited::draw(wnd);
	}
}