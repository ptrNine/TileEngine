#include <cstring>
#include "keyboardUpdater.hpp"


CKeyboardUpdater::SButton::SButton() {
	isPressed = false;
	isReleased = false;
	isHolding = false;
}

CKeyboardUpdater::CKeyboardUpdater() {
	_unicodeInput = 0;
	_updateHappen = false;
	memset(_buttons, 0, sizeof(_buttons));
}


bool CKeyboardUpdater::isPressed(sf::Keyboard::Key button) const {
	if (button != sf::Keyboard::Unknown)
		return _buttons[button].isPressed;
	return false;
}


bool CKeyboardUpdater::isReleased(sf::Keyboard::Key button) const {
	if (button != sf::Keyboard::Unknown)
		return _buttons[button].isReleased;
	return false;
}


bool CKeyboardUpdater::isHolding(sf::Keyboard::Key button) const {
	if (button != sf::Keyboard::Unknown)
		return _buttons[button].isHolding;
	return false;
}


bool CKeyboardUpdater::isDelayedHolding(sf::Keyboard::Key button) const {
	if (button != sf::Keyboard::Unknown)
		return _buttons[button].isDelayedHolding;
	return false;
}


bool CKeyboardUpdater::isInput() const {
	return _unicodeInput != 0;
}


U32 CKeyboardUpdater::getInput() const {
	return _unicodeInput;
}


void CKeyboardUpdater::update(const sf::Event& evt) {
	_updateHappen = true;
	if (evt.key.code != sf::Keyboard::Unknown) {
		_readyToNull.push_back(evt.key.code);
		if (evt.type == sf::Event::EventType::KeyPressed) {
			if (!_buttons[evt.key.code].isHolding) {
				_buttons[evt.key.code].isHolding = true;
				_buttons[evt.key.code].isPressed = true;
				_buttons[evt.key.code].isReleased = false;
				_buttons[evt.key.code].isDelayedHolding = true;
			} else {
				_buttons[evt.key.code].isDelayedHolding = true;
			}
		}
		if (evt.type == sf::Event::EventType::KeyReleased) {
			_buttons[evt.key.code].isHolding = false;
			_buttons[evt.key.code].isPressed = false;
			_buttons[evt.key.code].isReleased = true;
			_buttons[evt.key.code].isDelayedHolding = false;
		}
	}
}


void CKeyboardUpdater::setInput(U32 unicode) {
	_unicodeInput = unicode;
}


void CKeyboardUpdater::nullStates() {
	_unicodeInput = 0;

	if (_updateHappen) {
		for (auto i : _readyToNull) {
			_buttons[i].isPressed = false;
			_buttons[i].isReleased = false;
			_buttons[i].isDelayedHolding = false;
		}
	}
}