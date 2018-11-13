#include "button_array.hpp"
#include "window.hpp"

CButtonArray::CButtonArray() {
    _shape.setFillColor(sf::Color(0, 0, 0, 0));
    _parent = nullptr;
    setAligment(Aligment::none);
    _dimensionX = 1;
    _buttonSize = _shape.getSize();
}


CButtonArray::CButtonArray(const sf::FloatRect& rect, size_t dimensionX, U32 align) : CButtonArray() {
    setRect(rect);
    setAligment(align);
    _dimensionX = dimensionX;
    _buttonSize.x = rect.width;
    _buttonSize.y = rect.height;
}


void CButtonArray::recalcButtons() {
    // если нет ни одной кнопки - считать нет необходимости
    if (_buttons.empty())
        return;

    // находим размеры текущей сетки
    size_t buttonsXmax = _buttons.size() > _dimensionX ? _dimensionX : _buttons.size();
    size_t buttonsY = _dimensionX > 1 ? _buttons.size() / (_dimensionX + 1) + 1 : _buttons.size();
    // толщина всех границ
    float borderlineT = _shape.getOutlineThickness();

    // новые размеры заднего шэйпа с учетом границ кнопок
    sf::Vector2f _shapeSize = _buttonSize;
    _shapeSize.x = _shapeSize.x * buttonsXmax + (buttonsXmax - 1) * borderlineT;
    _shapeSize.y = _shapeSize.y * buttonsY + (buttonsY - 1) * borderlineT;
    _shape.setSize(_shapeSize);

    // пересчитываем позиции
    for (size_t i = 0; i < _buttons.size(); ++i) {
        auto pos = _shape.getPosition();
        if (i >= _dimensionX)
            pos.y = (_buttonSize.y + borderlineT)  * U32(i / _dimensionX) + pos.y;
        if (i % _dimensionX > 0) {
            pos.x = (_buttonSize.x + borderlineT)  * (i % _dimensionX) + pos.x;
        }

        _buttons[i].setPos(pos);
        _buttons[i].setSize(_buttonSize);
    }
}


void CButtonArray::reloadTheme() {
    _shape.setFillColor(_themeButton._shape.getFillColor());
    _shape.setOutlineColor(_themeButton._shape.getOutlineColor());
    for (auto& i : _buttons) {
        i._shape.setFillColor(_themeButton._shape.getFillColor());
        i._shape.setTexture(_themeButton._shape.getTexture());
        i._pressShape.setFillColor(_themeButton._pressShape.getFillColor());
        i._pressShape.setTexture(_themeButton._pressShape.getTexture());
        i._shape.setOutlineColor(_themeButton._shape.getOutlineColor());
        i._pressShape.setOutlineColor(_themeButton._pressShape.getOutlineColor());
        i.setFont(*_themeButton._text.getFont());
        i.setFontSize(_themeButton._text.getCharacterSize());
        i.setTextColor(_themeButton._text.getFillColor());
    }
}


void CButtonArray::addButton(const sf::String& name, BFunctor const& action) {
    // создаем кнопку
    CButton button;

    button.setText(name);
    button.setAction(action);
    button.setTextAligment(CButton::TextAligment::centered);
    button._parent = _parent;
    _buttons.push_back(button);

    // пересчитываем кнопки
    recalcButtons();
    reloadTheme();
}


void CButtonArray::setThemeFromButton(const CButton& button) {
    _themeButton = button;
    _themeButton._parent = nullptr;
    reloadTheme();
}

// virtual methods

void CButtonArray::setPos(const sf::Vector2f& pos) {
    inherited::setPos(pos);
    recalcButtons();
}



void CButtonArray::setSize(const sf::Vector2f& size) {
    // находим разницу в размерах
    sf::Vector2f subSize(size.x - _shape.getSize().x, size.y - _shape.getSize().y);

    inherited::setSize(size);

    if (!_buttons.empty()) {
        // находим колличество элементов сетки
        size_t x = _buttons.size() >= _dimensionX ? _dimensionX : _buttons.size();
        size_t y = _buttons.size() / (_dimensionX + 1) + 1;

        // изменения в размере для одной ячейки
        subSize.x /= x;
        subSize.y /= y;

        _buttonSize.x += subSize.x;
        _buttonSize.y += subSize.y;
    } else {
        _buttonSize = size;
    }

    recalcButtons();
}


void CButtonArray::move(const sf::Vector2f& offset) {
    inherited::move(offset);

    for (auto& i : _buttons)
        i.move(offset);
}


void CButtonArray::resize(const sf::Vector2f& factors, const sf::Vector2f& offset) {
    inherited::resize(factors, offset);
    for (auto& i : _buttons) {
        i.resize(factors, offset);
    }
}


void CButtonArray::update(CWindow& wnd) {
    for (auto& i : _buttons)
        i.update(wnd);
    inherited::update(wnd);
}


void CButtonArray::draw(CWindow& wnd) {
    //wnd.draw(this->_shape, _transform);
    for (auto& i : _buttons)
        i.draw(wnd);
}