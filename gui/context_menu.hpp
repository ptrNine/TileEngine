#ifndef  _CONTEXT_MENU_HPP_
#define _CONTEXT_MENU_HPP_

#include "button_array.hpp"


////////////// CONTEXT MENU CLASS //////////////////////////////////
//
// Контекстное меню.  Для правильного функционирования требуется 
// вызов из родительского объекта методов update, draw, resize
//
////////////////////////////////////////////////////////////////////

class CContextMenu : public CButtonArray {
public:
    friend class CLabel;		// для апдейта

private:
    typedef CButtonArray inherited;

public:
    CContextMenu();
    explicit CContextMenu(const sf::FloatRect& rect, U32 align = Aligment::none);

    void show();	// показать контекстное меню
    void close();	// скрыть контекстное меню
    bool isShown();



protected:
    bool _isShown;	// показывается ли контекстное меню


    // virtual methods
protected:
    void update(CWindow& wnd) override;
    void draw(CWindow& wnd) override;
};



#endif // ! _CONTEXT_MENU_HPP_
