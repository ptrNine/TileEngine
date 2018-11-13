#ifndef _KEYBOARD_TESTS_HPP_
#define _KEYBOARD_TESTS_HPP_

#include "../keyboard.hpp"

namespace sf {
    class Window;
}

class CKeyboardClickTest {
private:
    sf::Window* _wnd;
public:
    explicit CKeyboardClickTest(sf::Window* wnd);
    void run();
};

#endif // !_KEYBOARD_TESTS_HPP_
