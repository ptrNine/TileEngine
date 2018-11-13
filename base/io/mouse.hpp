#ifndef _MOUSE_HPP_
#define _MOUSE_HPP_
#include "../timer.hpp"
#include <SFML/System/Vector2.hpp>


////////////////// MOUSE SINGLETON ////////////////////////////////////////////
//
// Мышь. Можно вызывать и использовать из любого места.
//
// !!! _update должен вызываться первым, перед всеми остальными методами CMouse
// !!! _press, _release, _moved обновляются в цикле евентов окна
// !!! _capture используется из элементов gui, когда необходимо захватить мышь
//
///////////////////////////////////////////////////////////////////////////////
class CMouse {
public:
	// класс кнопки, хранит состояния кнопки и таймстамп последнего клика
	class CButton {
	public:
		friend CMouse;

	protected:
		zog::CTimer::Timestamp _lastClickT;		// последнее время клика

		bool isHolding;			// удерживаем
		bool isPressed;			// нажали единожды
		bool isReleased;		// отпустили

		bool isDoubleHolding;	// то же для двойных кликов
		bool isDoublePressed;
		bool isDoubleReleased;

		CButton();
	};


public:
	bool				isHolding			(size_t sfmlBtnNumber) const;
	bool				isPressed			(size_t sfmlBtnNumber) const;
	bool				isReleased			(size_t sfmlBtnNumber) const;
	bool				isDoubleHolding		(size_t sfmlBtnNumber) const;
	bool				isDoublePressed		(size_t sfmlBtnNumber) const;
	bool				isDoubleReleased	(size_t sfmlBtnNumber) const;
	bool				isCaptured			() const;		// is mouse captured by something
	const sf::Vector2i& getPosition			() const;		// get cursor position in window coordinates
	const sf::Vector2f  getPositionF		() const;
	

public:
	void			_press		(size_t sfmlBtnNumber);
	void			_release	(size_t sfmlBtnNumber);
	void			_moved		(int x, int y);
	void			_update		();
	void			_capture	();		// захватить мышь

protected:
	static const U32 _BUTTONS_COUNT = 3;
	CButton			 _buttons[_BUTTONS_COUNT];	// 0 - left, 1 - right, 2 - middle
	double			 _doubleClickDelay;			// CFG load
	sf::Vector2i	 _cursorPos;
	bool			 _captureFlag;				// захвачена ли мышка каким либо объектом


// singleton pattern
public:
	static CMouse& inst() {
		static CMouse instance;
		return instance;
	}

private:
	CMouse();
	~CMouse() = default;

public:
	CMouse(const CMouse&) = delete;
	CMouse& operator= (const CMouse&) = delete;
};


namespace zog {
	inline CMouse& mouse() {
		return CMouse::inst();
	}
}



#endif // !_MOUSE_HPP_
