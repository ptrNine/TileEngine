#include "panel.h"
#include "window.hpp"


void CPanelBase::setFilling(const sf::Color& color) {
	this->_shape.setFillColor(color);
}


void CPanelBase::setTexture(const sf::Texture* texture) {
	_shape.setTexture(texture);
}


void CPanelBase::setBorder(const sf::Color& color, float t) {
	this->_shape.setOutlineThickness(t);
	this->_shape.setOutlineColor(color);
}


void CPanelBase::setBorderSize( float t) {
	this->_shape.setOutlineThickness(t);
}


void CPanelBase::setBorderColor(const sf::Color& color) {
	this->_shape.setOutlineColor(color);
}

void CPanelBase::setRect(const sf::FloatRect& rect) {
	setPos(sf::Vector2f(rect.left, rect.top));
	setSize(sf::Vector2f(rect.width, rect.height));
}


void CPanelBase::setAligment(U32 align) {
	inherited::setAligment(align);
}


sf::FloatRect CPanelBase::getRect() const {
	return _shape.getGlobalBounds();
}


sf::Vector2f CPanelBase::getPos() const {
	return _shape.getPosition();
}


sf::Vector2f CPanelBase::getSize() const {
	return _shape.getSize();
}


// Virtual methods


void CPanelBase::setPos(const sf::Vector2f& pos) {
	this->_shape.setPosition(sf::Vector2f(pos));
}


void CPanelBase::setSize(const sf::Vector2f& size) {
	this->_shape.setSize(sf::Vector2f(size));
}


void CPanelBase::move(const sf::Vector2f& offset) {
	setPos(sf::Vector2f(getPos().x + offset.x, getPos().y + offset.y));
}


void CPanelBase::update(CWindow& wnd) {
}


void CPanelBase::draw(CWindow& wnd) {
	wnd.draw(this->_shape, _transform);
}


void CPanelBase::resize(const sf::Vector2f& factors, const sf::Vector2f& offset) {
	_transform = sf::Transform();
	_transform.scale(factors);

	auto pos = getPos();
	auto size = getSize();

	if ((_align & 1U) == 1U) {
		if ((_align & 4U) == 4U)
			size.x += offset.x;
		else
			pos.x += offset.x;
	}
	if ((_align & 2U) == 2U) {
		if ((_align & 8U) == 8U)
			size.y += offset.y;
		else
			pos.y += offset.y;
	}

	setPos(pos);
	setSize(size);
}





CPanel::CPanel() {
	setFilling(sf::Color(60, 60, 60, 255));
	_parent = nullptr;
	setAligment(Aligment::none);
}


CPanel::CPanel(const sf::FloatRect& rect, U32 align) : CPanel() {
	setRect(rect);
	setAligment(align);
}


void CPanel::resize(const sf::Vector2f& factors, const sf::Vector2f& offset) {
	inherited::resize(factors, offset);
	for (auto i : components) {
		i->resize(factors, offset);
	}
}


void CPanel::update(CWindow& wnd) {
	for (S32 i = components.size() - 1; i >= 0; --i)
		components[i]->update(wnd);
	inherited::update(wnd);
}


void CPanel::draw(CWindow& wnd) {
	inherited::draw(wnd);
	for (auto& i : components)
		i->draw(wnd);
}


void CPanel::addComponent(CComponent* component) {
	components.push_back(component);
	component->_parent = this;
	component->setAligment(component->_align);
}