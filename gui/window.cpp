#include "window.hpp"
#include "cursorIcons.hpp"
#include "../base/io/keyboard.hpp"
#include "../base/io/mouse.hpp"


CWindow::CWindow() : sf::RenderWindow() {
    _lastSize = this->getSize();
    setMouseCursorVisible(false);
    _isActive = true;
}


CWindow::CWindow(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings) : sf::RenderWindow(mode, title, style, settings) { 
    _lastSize = this->getSize();
    setMouseCursorVisible(false);
    _isActive = true;
}


CWindow::CWindow(sf::WindowHandle handle, const sf::ContextSettings& settings) : sf::RenderWindow(handle, settings) { 
    _lastSize = this->getSize();
    setMouseCursorVisible(false);
    _isActive = true;
}


void CWindow::operateGui() {
    // Обновляем клавиатуру и мышь
    zog::mouse()._update();
    zog::keyboard()._update();

    auto evt = sf::Event();
    while (this->pollEvent(evt)) {
        switch (evt.type) {
        // мышка передвинулась - обновняем позицию
        case sf::Event::EventType::MouseMoved:
            zog::mouse()._moved(evt.mouseMove.x, evt.mouseMove.y);
            break;
        // нажали или отпустили кнопку мыши - обновляем нажатия мыши
        case sf::Event::MouseButtonPressed:
            zog::mouse()._press(evt.mouseButton.button);
            break;
        case sf::Event::MouseButtonReleased:
            zog::mouse()._release(evt.mouseButton.button);
            break;
        // нажали или отпустили кнопку клавиатуры - обновляем нажатия клавы
        case sf::Event::KeyPressed:
            zog::keyboard()._press(evt.key.code);
            break;
        case sf::Event::KeyReleased:
            zog::keyboard()._release(evt.key.code);
            break;
        // ввели символ - сохраняем его
        case sf::Event::TextEntered:
            zog::keyboard()._inputChar(evt.text.unicode);
            break;
        // закрытие окна
        case sf::Event::Closed:
            this->close();
            break;
        }
    }

    CCursorIcons::instance().setType(CCursorIcons::Type::regular);

    for (auto i = _components.rbegin(); i != _components.rend(); ++i)
        (*i)->update(*this);

    for (auto& i : _components)
        i->draw(*this);

    CCursorIcons::instance().drawCursor(*this);
}


void CWindow::addComponent(CComponent* component) {
    _components.push_back(component);
}


bool CWindow::isActive() const {
    return _isActive;
}


// Virtual methods

void CWindow::onCreate() {
}


void CWindow::onResize() {
    inherited::onResize();
    auto size = getSize();
    auto newSize = size;
    sf::Vector2f offset(float(size.x), float(size.y));
    auto factors = getView().getSize();

    if (offset.x < factors.x) { offset.x = factors.x; newSize.x = U32(factors.x); }
    if (offset.y < factors.y) { offset.y = factors.y; newSize.y = U32(factors.y); }
    offset.x -= _lastSize.x;
    offset.y -= _lastSize.y;


    factors.x /= size.x;
    factors.y /= size.y;

    for (auto i : _components)
        i->resize(factors, offset);

    CCursorIcons::instance().resize(factors);

    _lastSize = newSize;
}