#include "keyboard.hpp"
#include "../cfgSystem.hpp"

CKeyboard::CButton::CButton() {
    isHolding = false;
    isPressed = false;
    isReleased = false;
    isDelayedHolding = false;
    _lastClickT = zog::timer().timestamp();
}

CKeyboard::CKeyboard() {
    _delayedHoldingDelay = zog::readConfig().r_u32("keyboard_params", "delayed_holding_delay") / 1000.0;

    for (auto& i : _buttons)
        i = CButton();

    _unicodeChar = 0;
    _captureFlag = false;
    _symbolInputed = false;
}


void CKeyboard::_press(size_t sfmlBtnNumber) {
    if (sfmlBtnNumber >= _BUTTONS_COUNT)
        return;

    auto& btn = _buttons[sfmlBtnNumber];

    // защита от делай холдинга в сфмл
    if (!btn.isHolding) btn.isPressed = true;

    btn.isHolding = true;

    btn._lastClickT = zog::timer().timestamp();
}


void CKeyboard::_release(size_t sfmlBtnNumber) {
    if (sfmlBtnNumber >= _BUTTONS_COUNT)
        return;

    auto& btn = _buttons[sfmlBtnNumber];
    btn.isReleased = true;
    btn.isHolding = false;
    btn.isDelayedHolding = false;
}


void CKeyboard::_inputChar(U32 unicodeChar) {
    _unicodeChar = unicodeChar;
    _symbolInputed = true;
}


void CKeyboard::_update() {
    for (auto& i : _buttons) {
        i.isPressed = false;
        i.isReleased = false;

        if (i.isHolding) {
            auto curTime = zog::timer().timestamp();
            if ((curTime - i._lastClickT).sec() > _delayedHoldingDelay)
                i.isDelayedHolding = true;
        }
    }

    // апдэйет снимает захват клавы
    _captureFlag = false;
    _symbolInputed = false;
}


void CKeyboard::_capture() {
    _captureFlag = true;
}


// public
bool CKeyboard::isHolding(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isHolding : false;
}

bool CKeyboard::isPressed(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isPressed : false;
}

bool CKeyboard::isReleased(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isReleased : false;
}

bool CKeyboard::isDelayedHolding(size_t sfmlBtnNumber) const {
    return sfmlBtnNumber < _BUTTONS_COUNT ? _buttons[sfmlBtnNumber].isDelayedHolding : false;
}

bool CKeyboard::isDelayHoldOrPress(size_t sfmlBtnNumber) const {
    return isPressed(sfmlBtnNumber) || isDelayedHolding(sfmlBtnNumber);
}

U32 CKeyboard::getUnicodeChar() const {
    return _unicodeChar;
}

bool CKeyboard::isCaptured() const {
    return _captureFlag;
}

bool CKeyboard::isInput() const {
    return _symbolInputed;
}
