#include "keyboard_tests.hpp"
#include <SFML/Window.hpp>


CKeyboardClickTest::CKeyboardClickTest(sf::Window* wnd) {
    _wnd = wnd;
}

void CKeyboardClickTest::run() {
    while (true) {
        zog::keyboard()._update();

        sf::Event evt = sf::Event();
        while (_wnd->pollEvent(evt)) {
            switch (evt.type) {
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
            case sf::Event::Closed:
                _wnd->close();
                return;
            }
        }

        for (size_t i = 0; i < sf::Keyboard::KeyCount; i++) {

            if (zog::keyboard().isPressed(i)) { printf("key %li pressed  char: %i\n", i, zog::keyboard().getUnicodeChar()); }
            //if (zog::keyboard().isHolding(i)) { printf("key %ui holded\n", i); }
            if (zog::keyboard().isReleased(i)) { printf("key %li release\n", i); }
            if (zog::keyboard().isDelayedHolding(i)) { printf("key %li delayed holding\n", i); }

        }
    }
}