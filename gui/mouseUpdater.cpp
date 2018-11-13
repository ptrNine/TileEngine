#include "mouseUpdater.hpp"


CMouseUpdater::SButton::SButton() {
	isHolding = false;
	isPressed = false;
	isReleased = false;
	isDoubleHolding = false;
	isDoublePressed = false;
	isDoubleReleased = false;
	_lastClickT = zog::timer().timestamp();
}


CMouseUpdater::CMouseUpdater() {
	_updateHappen = false;
	_doubleClickDelay = 0.3;

	for (auto& i : buttons)
		i = SButton();
	_listener = nullptr;
}


void CMouseUpdater::update(const sf::Event& evt) {
	_updateHappen = true;
	switch (evt.mouseButton.button) {
	case sf::Mouse::Button::Left:
		updateButton(evt, ButtonLeft);
		break;
	case sf::Mouse::Button::Right:
		updateButton(evt, ButtonRight);
		break;
	case sf::Mouse::Button::Middle:
		updateButton(evt, ButtonMiddle);
		break;
	default:
		break;
	}
}


bool CMouseUpdater::isClicking(Buttons button) const {
	return buttons[button].isPressed;
}


bool CMouseUpdater::isReleasing(Buttons button) const {
	return buttons[button].isReleased;
}


bool CMouseUpdater::isHolding(Buttons button) const {
	return buttons[button].isHolding;
}


bool CMouseUpdater::isDoubleClicking(Buttons button) const {
	return buttons[button].isDoublePressed;
}


bool CMouseUpdater::isDoubleReleasing(Buttons button) const {
	return buttons[button].isDoubleReleased;
}


bool CMouseUpdater::isDoubleHolding(Buttons button) const {
	return buttons[button].isHolding;
}


const CMouseUpdater::SButton& CMouseUpdater::getState(Buttons button) const {
	return buttons[button];
}


void CMouseUpdater::setListener(CComponent* component) {
	_listener = component;
}


bool CMouseUpdater::isCaptured(const CComponent* component) const {
	return component == _listener;
}


bool CMouseUpdater::hasListener() {
	return _listener;
}


void CMouseUpdater::nullStates() {
	if (!_updateHappen)
		return;

	for (auto& i : buttons) {
		i.isPressed = false;
		i.isReleased = false;
		i.isDoublePressed = false;
		i.isDoubleReleased = false;
	}

	_updateHappen = false;
}


void CMouseUpdater::updateButton(const sf::Event& evt, Buttons button) {
	auto& curButton = buttons[button];
	if (evt.type == sf::Event::MouseButtonPressed) {
		curButton.isPressed = true;
		curButton.isReleased = false;
		curButton.isHolding = true;

		auto now = zog::timer().timestamp();
		if ((now - curButton._lastClickT).sec() < _doubleClickDelay) {
			curButton.isDoublePressed = true;
			curButton.isDoubleHolding = true;
		}
		curButton._lastClickT = now;
	}

	if (evt.type == sf::Event::MouseButtonReleased) {
		curButton.isPressed = false;
		curButton.isReleased = true;
		curButton.isHolding = false;

		if (curButton.isDoubleHolding)
			curButton.isDoubleReleased = true;

		curButton.isDoubleHolding = false;
	}
}