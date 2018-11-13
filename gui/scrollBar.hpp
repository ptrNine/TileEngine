#ifndef _SCROLL_BAR_HPP_
#define _SCROLL_BAR_HPP_


#include "panel.h"


class CScrollBar :public CPanelBase {
private:
	typedef CPanelBase inherited;
	friend class CSelect;

public:

	explicit CScrollBar(const sf::FloatRect& rect, bool isHorizontal = false, U32 align = Aligment::movable);
	
	void setPos				(const sf::Vector2f& pos) override;
	void setSize			(const sf::Vector2f& size) override;

	void setButtonShape		(const sf::RectangleShape& shape);
	void setMoveAreaShape	(const sf::RectangleShape& shape);
	void setMovableShape	(const sf::RectangleShape& shape);

	float	getProgress		() const;								// get progress of scrolling. 0.0 - 1.0
	float	getStartItem	(U32 items_count, U32 show_size) const; // get start item in scrolling area

	sf::FloatRect		getRect() const override;
	sf::Vector2f		getPos() const override;
	sf::Vector2f		getSize() const override;


protected:
	void update		(CWindow& wnd) override;
	void draw		(CWindow& wnd) override;

protected:
	sf::RectangleShape		_buttonShape1;
	sf::RectangleShape		_buttonShape2;
	sf::RectangleShape		_movableShape;
	bool					_isHorizontal;
	bool					_onScrolling;

};


#endif // !_SCROLL_BAR_HPP_