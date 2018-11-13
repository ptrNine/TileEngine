#include "mouse_tests.hpp"
#include "SFML/Window.hpp"
#include "../../base/io/mouse.hpp"
#include "../../base/new_types/sharedString.hpp"


CMouseClickTest::CMouseClickTest(sf::Window* wnd) {
	_wnd = wnd;
}

void CMouseClickTest::run() {
	while (true) {
		zog::mouse()._update();

		sf::Event evt = sf::Event();
		while (_wnd->pollEvent(evt)) {
			switch (evt.type) {
				// передвинули мышь
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
			case sf::Event::Closed:
				_wnd->close();
				return;
			}
		}

		for (size_t i = 0; i < 3; i++) {
			shared_str btn_name;
			switch (i) {
			case 0: btn_name = "left"; break;
			case 1: btn_name = "right"; break;
			case 2: btn_name = "middle"; break;
			}
			if (zog::mouse().isPressed(i)) printf("%s button pressed\n", btn_name.c_str());
			else if (zog::mouse().isReleased(i)) printf("%s button released\n", btn_name.c_str());
			
			if (zog::mouse().isDoublePressed(i)) printf("%s button double-click pressed\n", btn_name.c_str());
			else if (zog::mouse().isDoubleReleased(i)) printf("%s button double-click released\n", btn_name.c_str());
		}
		//printf("mouse pos: [%i, %i]\n", zog::mouse().getPosition().x, zog::mouse().getPosition().y);
	}
}