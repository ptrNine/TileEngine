#include "button.hpp"
#include "window.hpp"
#include "../fontManager.hpp"
#include <SFML/Window/Mouse.hpp>
#include "cursorIcons.hpp"
#include "../base/io/mouse.hpp"


CButton::CButton() {
    _shape.setFillColor(sf::Color(90, 90, 90, 255));
    setBorder(sf::Color(120, 120, 120, 255), 2);
    _pressShape.setFillColor(sf::Color(120, 120, 120, 255));
    setPressBorder(sf::Color(160, 160, 160, 255), 2);

    _text.setCharacterSize(32);
    _text.setFillColor(sf::Color(0, 0, 0, 255));
    _text.setFont(base::FontManager().getFont("font_anonymous"));
    _parent = nullptr;
    setAligment(Aligment::none);
    _textAlignType = TextAligment::centered;
    _isEdit = false;
}


CButton::CButton(sf::String name, const sf::FloatRect& rect, U32 align) : CButton() {
    setRect(rect);
    setText(name);
    setAligment(align);
}


CButton::CButton(sf::String name, const sf::FloatRect& rect, BFunctor const& action, U32 align) : CButton(name, rect) {
    _action = action;
    setAligment(align);
}


void CButton::setPressFilling(const sf::Color& color) {
    _pressShape.setFillColor(color);
}


void CButton::setPressTexture(const sf::Texture* texture) {
    _pressShape.setTexture(texture);
}


void CButton::setPressBorder(const sf::Color& color, float t) {
    _pressShape.setOutlineThickness(t);
    _pressShape.setOutlineColor(color);
}


void CButton::setPressBorderSize(float t) {
    _pressShape.setOutlineThickness(t);
}


void CButton::setPressBorderColor(const sf::Color& color) {
    _pressShape.setOutlineColor(color);
}


void CButton::setAction(BFunctor const& action) {
    _action = action;
}


// Virtual methods

void CButton::setPos(const sf::Vector2f& pos) {
    inherited::setPos(pos);
    _pressShape.setPosition(pos);
}


void CButton::setSize(const sf::Vector2f& size) {
    inherited::setSize(size);
    _pressShape.setSize(size);
}


void CButton::move(const sf::Vector2f& offset) {
    inherited::move(offset);
    _pressShape.move(offset);
}


void CButton::update(CWindow& wnd) {
    //inherited update skipped
    bool onMouse = _shape.getGlobalBounds().contains(zog::mouse().getPositionF());

    // захватываем владение мышью
    if (onMouse && (zog::mouse().isPressed(sf::Mouse::Left) ||
        zog::mouse().isHolding(sf::Mouse::Left) ||
        zog::mouse().isReleased(sf::Mouse::Left)) &&
        !zog::mouse().isCaptured()) {

        zog::mouse()._capture();

        _isEdit = zog::mouse().isHolding(sf::Mouse::Left) && onMouse;

        if (zog::mouse().isReleased(sf::Mouse::Left) && onMouse && _action != nullptr)
            _action();
    }
    else {
        _isEdit = false;
    }
}

void CButton::draw(CWindow& wnd) {
    if (_isEdit)
        wnd.draw(_pressShape, _transform);
    else
        wnd.draw(_shape, _transform);

    wnd.draw(_text, _transform);

    if (getRect().contains(zog::mouse().getPositionF()))
        CCursorIcons::instance().setType(CCursorIcons::Type::button);
}