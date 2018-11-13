#include "mouse.hpp"
#include "../cfgSystem.hpp"

CMouse::CButton::CButton() {
    isHolding = false;
    isPressed = false;
    isReleased = false;
    isDoubleHolding = false;
    isDoublePressed = false;
    isDoubleReleased = false;
    _lastClickT = zog::timer().timestamp();
}

CMouse::CMouse() {
    _doubleClickDelay = zog::readConfig().r_double("mouse_params", "double_click_delay");

    for (auto& i : _buttons)
        i = CButton();

    _cursorPos.x = 0;
    _cursorPos.y = 0;

    _captureFlag = false;
}


void CMouse::_press(size_t sfmlBtnNumber) {
    if (sfmlBtnNumber >= _BUTTONS_COUNT)
        return;

    auto curClickT = zog::timer().timestamp();
    auto& btn = _buttons[sfmlBtnNumber];

    // двойное нажатие
    if ((curClickT - btn._lastClickT).sec() < _doubleClickDelay) {
        btn.isDoublePressed  = true;
        btn.isDoubleHolding = true;
    }

    btn.isPressed = true;
    btn.isHolding = true;

    btn._lastClickT = curClickT;
}


void CMouse::_release(size_t sfmlBtnNumber) {
    if (sfmlBtnNumber >= _BUTTONS_COUNT)
        return;

    auto& btn = _buttons[sfmlBtnNumber];
    btn.isReleased = true;
    btn.isHolding = false;

    if (btn.isDoubleHolding) {
        btn.isDoubleReleased = true;
        btn.isDoubleHolding = false;
    }
}

void CMouse::_moved(int x, int y) {
    _cursorPos.x = x;
    _cursorPos.y = y;
}

void CMouse::_update() {
    for (auto& i : _buttons) {
        i.isPressed = false;
        i.isReleased = false;
        i.isDoublePressed = false;
        i.isDoubleReleased = false;
    }

    // апдэйет снимает захват мыши
    _captureFlag = false;
}

void CMouse::_capture() {
    _captureFlag = true;
}


// public
bool CMouse::isHolding(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isHolding : false;
}

bool CMouse::isPressed(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isPressed : false;
}

bool CMouse::isReleased(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isReleased : false;
}

bool CMouse::isDoubleHolding(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isDoubleHolding : false;
}

bool CMouse::isDoublePressed(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isDoublePressed : false;
}

bool CMouse::isDoubleReleased(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isDoubleReleased : false;
}

bool CMouse::isCaptured() const {
    return _captureFlag;
}

const sf::Vector2i& CMouse::getPosition() const {
    return _cursorPos;
}

const sf::Vector2f CMouse::getPositionF() const {
    return sf::Vector2f(_cursorPos);
}