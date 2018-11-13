#ifndef _PANEL_HPP_
#define _PANEL_HPP_

#include <SFML/Graphics.hpp>
#include "component.hpp"
#include "../base/types.hpp"

////////////////// PANEL BASE ABSTRACT CLASS /////////////////////////
//
//	Base component
//
//////////////////////////////////////////////////////////////////////
class CPanelBase : public CComponent {
private:
    typedef CComponent inherited;

protected:
    sf::RectangleShape	_shape;			// base shape
    sf::Transform		_transform;		// for transformations

protected:
    void update		(CWindow& wnd) override;
    void draw		(CWindow& wnd) override;

public:
    // Old functions declarations
    void setAligment(U32 align);
    void setPos		(const sf::Vector2f& pos) override;
    void setSize	(const sf::Vector2f& size) override;
    void move		(const sf::Vector2f& offset) override;
    void resize		(const sf::Vector2f& factors, const sf::Vector2f& offset) override;


    void setFilling			(const sf::Color& color);		// fill shape with color
    void setTexture			(const sf::Texture* texture);	// fill shape with texture
    void setBorder			(const sf::Color& color, float t);	// set colored border to component with t thickness
    void setBorderSize		(float t);					// set border thickness
    void setBorderColor		(const sf::Color& color); // set color of border
    void setRect			(const sf::FloatRect& rect);			// set operating rectangle


    virtual sf::FloatRect		getRect() const;
    virtual sf::Vector2f		getPos() const;
    virtual sf::Vector2f		getSize() const;
};


class CPanel : public CPanelBase {
private:
    typedef CPanelBase inherited;
    std::vector<CComponent*> components;

protected:
    void update(CWindow& wnd) override;
    void draw(CWindow& wnd) override;

public:
    CPanel();
    explicit CPanel(const sf::FloatRect& rect, U32 align = Aligment::none);

    void addComponent(CComponent* component);		// adding component to panel


    void resize	(const sf::Vector2f& factors, const sf::Vector2f& offset) override;
};

#endif //_PANEL_HPP_