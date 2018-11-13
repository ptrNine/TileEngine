#ifndef _KEYBOARD_HPP_
#define _KEYBOARD_HPP_

#include "SFML/Window/Keyboard.hpp"
#include "../timer.hpp"


///////////////////////// KEYBOARD SINGLETON ////////////////////////////////
//
//	Клавиатура. Вызывается и используется из любого места
//
// !!! _update должен вызываться первым, перед всеми остальными методами CKeyboard
// !!! _press, _release, _inputChar обновляются в цикле евентов окна
// !!! _capture используется когда необходимо захватить клаву
//
/////////////////////////////////////////////////////////////////////////////
class CKeyboard {
    // класс кнопки, хранит состояния кнопки и таймстамп последнего клика
    class CButton {
    public:
        friend CKeyboard;

    protected:
        zog::CTimer::Timestamp _lastClickT;		// последнее время клика

        bool isHolding;			// удерживаем
        bool isDelayedHolding;	// удерживаем после некоторой задержки
        bool isPressed;			// нажали единожды
        bool isReleased;		// отпустили

        CButton();
    };


public:
    bool				isHolding			(size_t sfmlBtnNumber) const;
    bool				isPressed			(size_t sfmlBtnNumber) const;
    bool				isReleased			(size_t sfmlBtnNumber) const;
    bool				isDelayedHolding	(size_t sfmlBtnNumber) const;
    bool				isDelayHoldOrPress	(size_t sfmlBtnNumber) const;
    bool				isCaptured			() const;		// is keyboard captured by something
    U32					getUnicodeChar		() const;		// get unicode symbol that inputing from keyboaard
    bool				isInput				() const;		// is symbol inputed


public:
    void			_press		(size_t sfmlBtnNumber);
    void			_release	(size_t sfmlBtnNumber);
    void			_inputChar	(U32 unicodeChar);
    void			_update		();
    void			_capture	();		// захватить клавиатуру


protected:
    static const U32 _BUTTONS_COUNT = sf::Keyboard::KeyCount;
    CButton			 _buttons[_BUTTONS_COUNT];
    double			 _delayedHoldingDelay;		// CFG load
    U32				 _unicodeChar;				// введеный символ
    bool			 _captureFlag;				// захвачена ли клавиатура каким либо процесом
    bool			 _symbolInputed;			// введен ли символ

    // singleton pattern
public:
    static CKeyboard& inst() {
        static CKeyboard instance;
        return instance;
    }

private:
    CKeyboard();
    ~CKeyboard() = default;

public:
    CKeyboard(const CKeyboard&) = delete;
    CKeyboard& operator= (const CKeyboard&) = delete;
};


namespace zog {
    inline CKeyboard& keyboard() {
        return CKeyboard::inst();
    }
}


#endif // ! _KEYBOARD_HPP_