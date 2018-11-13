#ifndef _SELECT_HPP_
#define _SELECT_HPP_
#include "panel.h"
#include "button.hpp"
#include "label.hpp"

// TODO вместо сет колор сет бордер колор сет хуялор поставить тупо сет шейп. во всех нахуй классах
class CSelect : public CPanelBase {
private:
	typedef CPanelBase inherited;
	typedef std::function<void()> BFunctor;

public:
	explicit CSelect(const sf::FloatRect& rect, U32 align = Aligment::movable);
	~CSelect();
	
public:
	void setAligment(U32 align) override;
	void setPos		(const sf::Vector2f& pos) override;
	void setSize	(const sf::Vector2f& size) override;
	void resize		(const sf::Vector2f& factors, const sf::Vector2f& offset) override;

	void setOptionsShape		(const sf::RectangleShape& shape);
	void setOptionsOnMouseShape	(const sf::RectangleShape& shape);
	void setButtonShape			(const sf::RectangleShape& shape);
	void setSelectedOptionShape	(const sf::RectangleShape& shape);


	void addOption	(const sf::String& name);
	void showHide	() { _isShown = !_isShown; }
	void setActive	(S32 active);



	sf::FloatRect		getRect() const override;
	sf::Vector2f		getPos() const override;
	sf::Vector2f		getSize() const override;

protected:
	std::vector<CLabel>		_options;
	CButton					_showOptions;
	CLabel					_selectedOption;
	S32						_selectedValue;
	bool					_isShown;

	sf::RectangleShape		_optionsShape;
	sf::RectangleShape		_optionOnMouseShape;


protected:
	void update		(CWindow& wnd) override;
	void draw		(CWindow& wnd) override;
	

};


#endif //!_SELECT_HPP_