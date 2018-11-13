#ifndef _KEYBOARD_UPDATER_HPP_
#define _KEYBOARD_UPDATER_HPP_

//#include <unordered_map>
#include "../base/types.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

class CWindow;


class CKeyboardUpdater {
public:
	struct SButton {
		bool isHolding;
		bool isPressed;
		bool isReleased;
		bool isDelayedHolding;
		SButton();
	};

private:
	SButton _buttons[sf::Keyboard::KeyCount];
	U32		_unicodeInput;
	bool	_updateHappen;
	std::vector<sf::Keyboard::Key> _readyToNull;

public:
	CKeyboardUpdater		();

	bool			isPressed		(sf::Keyboard::Key button) const;
	bool			isReleased		(sf::Keyboard::Key button) const;
	bool			isHolding		(sf::Keyboard::Key button) const;
	bool			isDelayedHolding(sf::Keyboard::Key button) const;
	SButton			getState		(sf::Keyboard::Key button) const;
	bool			isInput			() const;
	U32				getInput		() const;


protected:
	friend CWindow;

	void update			(const sf::Event& evt);
	void setInput		(U32 unicode);
	void nullStates		();
};

#endif // _KEYBOARD_UPDATER_HPP_
