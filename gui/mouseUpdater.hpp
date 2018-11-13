#ifndef _MOUSE_UPDATER_HPP_
#define _MOUSE_UPDATER_HPP_

#include <vector>
#include "../base/timer.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include "../base/types.hpp"

class CComponent;
class CWindow;

class CMouseUpdater {
public:
    class SButton {
        friend class CMouseUpdater;

    protected:
        zog::CTimer::Timestamp _lastClickT;

    public:
        bool isHolding;
        bool isPressed;
        bool isReleased;

        bool isDoubleHolding;
        bool isDoublePressed;
        bool isDoubleReleased;

        SButton();
    };

private:
    static const U32 buttonCount = 3;
    bool			_updateHappen;
    SButton			 buttons[buttonCount];	// 0 - left, 1 - right, 2 - middle
    CComponent*		_listener;
    double			_doubleClickDelay;

public:
    CMouseUpdater();

    enum Buttons {
        ButtonLeft = 0,
        ButtonRight = 1,
        ButtonMiddle = 2
    };

    void update				(const sf::Event& evt);
    bool isClicking			(Buttons button) const;
    bool isReleasing		(Buttons button) const;
    bool isHolding			(Buttons button) const;
    bool isDoubleHolding	(Buttons button) const;
    bool isDoubleClicking	(Buttons button) const;
    bool isDoubleReleasing	(Buttons button) const;

    const SButton& getState	(Buttons button) const;
    void setListener		(CComponent* component);
    bool isCaptured			(const CComponent* component) const;
    bool hasListener		();


protected:
    friend CWindow;
    void nullStates();

private:
    void updateButton(const sf::Event& evt, Buttons button);

};


#endif //_MOUSE_UPDATER_HPP_