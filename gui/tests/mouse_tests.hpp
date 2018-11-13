#ifndef _MOUSE_TESTS_HPP_
#define _MOUSE_TESTS_HPP_
namespace sf {
	class Window;
}

class CMouseClickTest {
private:
	sf::Window* _wnd;
public:
	explicit CMouseClickTest(sf::Window* wnd);
	void run();
};



#endif // !_MOUSE_TESTS_HPP_
