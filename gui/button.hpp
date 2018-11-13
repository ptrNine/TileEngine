#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

#include <functional>
#include <SFML/Graphics.hpp>
#include "label.hpp"


class CButton : public CLabel {
private:
	typedef CLabel inherited;
	typedef std::function<void()> BFunctor;

protected:
	sf::RectangleShape	_pressShape;
	BFunctor			_action;
	//BFunctor			_secondAction;

public:
	friend class CButtonArray;
	friend class CSelect;

	CButton();
	CButton(sf::String name, const sf::FloatRect& rect, U32 align = Aligment::none);
	CButton(sf::String name, const sf::FloatRect& rect, BFunctor const& action, U32 align = Aligment::none);


	void setPressFilling	(const sf::Color& color);
	void setPressTexture	(const sf::Texture* texture);
	void setPressBorder		(const sf::Color& color, float t);
	void setPressBorderSize	(float t);
	void setPressBorderColor(const sf::Color& color);
	void setAction			(BFunctor const& action);

	
	void setPos		(const sf::Vector2f& pos) override;
	void setSize	(const sf::Vector2f& size) override;
	void move		(const sf::Vector2f& offset) override;

	
protected:
	void update		(CWindow& wnd) override;
	void draw		(CWindow& wnd) override;
};



#endif //_BUTTON_HPP_