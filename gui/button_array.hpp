#ifndef _BUTTON_ARRAY_HPP_
#define _BUTTON_ARRAY_HPP_

#include <functional>
#include <vector>
#include "component.hpp"
#include "button.hpp"

class CWindow;

class CButtonArray : public CPanelBase {
private:
	typedef CPanelBase inherited;
	typedef std::function<void()> BFunctor;


protected:
	size_t _dimensionX;				// максимальный размер в кнопках по иксу
	std::vector<CButton> _buttons;	// кнопки
	sf::Vector2f _buttonSize;		// размер одной кнопки. равен заданному размеру баттон аррэй при создании
	CButton		 _themeButton;		// кнопка для хранения темы


protected:
	void recalcButtons		();
	void reloadTheme		();
	void update		(CWindow& wnd) override;
	void draw		(CWindow& wnd) override;

public:
	void setPos		(const sf::Vector2f& pos) override;
	void setSize	(const sf::Vector2f& size) override;
	void move		(const sf::Vector2f& offset) override;
	void resize		(const sf::Vector2f& factors, const sf::Vector2f& offset) override;


public:
	CButtonArray();
	CButtonArray(const sf::FloatRect& rect, size_t dimensionX, U32 align = Aligment::none);

	void addButton				(const sf::String& name, BFunctor const& action);
	void setThemeFromButton		(const CButton& button);							// применяет визуальные параметры кнопки для всех кнопок
};




#endif // !_BUTTON_ARRAY_HPP_
