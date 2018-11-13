#include "select.hpp"
#include "window.hpp"
#include "../base/io/mouse.hpp"
#include "../base/Asserts.hpp"


CSelect::CSelect(const sf::FloatRect& rect, U32 align) {
	_selectedOption = CLabel("", rect, CLabel::TextAligment::centered, align, false);
	_showOptions = CButton("S", sf::FloatRect(rect.left + rect.width + _selectedOption._shape.getOutlineThickness(), rect.top, rect.height, rect.height), [&]() { showHide(); }, align);
	_selectedOption._parent = this;
	_showOptions._parent = this;

	_selectedValue = -1;

	_optionsShape.setFillColor(sf::Color(130, 130, 130));
	_optionsShape.setOutlineThickness(2.0f);
	_optionsShape.setOutlineColor(sf::Color(30, 30, 30));

	_optionOnMouseShape.setFillColor(sf::Color(20, 20, 200));
	_optionOnMouseShape.setOutlineThickness(2.0f);
	_optionOnMouseShape.setOutlineColor(sf::Color(30, 30, 30));

	setAligment(align);
	_selectedOption.setAligment(align);
	_showOptions.setAligment(align);
	setRect(rect);
}

CSelect::~CSelect() {
}

void CSelect::addOption(const sf::String& name) {
	float pos = getSize().y;

	if (!_options.empty())
		pos += _options.back().getPos().y + _options.back()._shape.getOutlineThickness();
	else
		pos += getPos().y + _selectedOption._shape.getOutlineThickness();

	auto rect = getRect();
	rect.top = pos;

	_options.emplace_back(CLabel(name, rect, CLabel::TextAligment::centered, _align, false));
	_options.back().setAligment(_align);
}


void CSelect::setPos(const sf::Vector2f& pos) {
	//inherited::setPos(pos);
	auto size		= getSize();

	_showOptions.setPos(sf::Vector2f(pos.x + size.x + _selectedOption._shape.getOutlineThickness(), pos.y));
	_selectedOption.setPos(pos);

	for (size_t i = 0; i < _options.size(); ++i) {
		float thik = i == 0 ? _selectedOption._shape.getOutlineThickness() : _options[i - 1]._shape.getOutlineThickness();
		_options[i].setPos(sf::Vector2f(pos.x, pos.y + (i + 1) * size.y + thik * (i + 1)));
	}
}


void CSelect::setSize(const sf::Vector2f& size) {
	//inherited::setSize(size);
	auto pos = getPos();

	_showOptions.setPos(sf::Vector2f(pos.x + size.x + _selectedOption._shape.getOutlineThickness(), pos.y));
	_selectedOption.setSize(size);

	for (size_t i = 0; i < _options.size(); ++i) {
		float thik = i == 0 ? _selectedOption._shape.getOutlineThickness() : _options[i - 1]._shape.getOutlineThickness();
		_options[i].setPos(sf::Vector2f(pos.x, pos.y + (i + 1) * size.y + thik * (i + 1)));
		_options[i].setSize(size);
	}

}

void CSelect::setAligment(U32 align) {
	inherited::setAligment(align);
	_showOptions.setAligment(align);
	_selectedOption.setAligment(align);

	for (auto& i : _options)
		i.setAligment(align);
}


void CSelect::resize(const sf::Vector2f& factors, const sf::Vector2f& offset) {
	_showOptions.resize(factors, offset);
	_selectedOption.resize(factors, offset);

	for (auto& i : _options)
		i.resize(factors, offset);
}


void CSelect::setOptionsShape(const sf::RectangleShape& shape) {
	_optionsShape = shape;
}

void CSelect::setOptionsOnMouseShape(const sf::RectangleShape& shape) {
	_optionOnMouseShape = shape;
}

void CSelect::setButtonShape(const sf::RectangleShape& shape) {
	auto pos = _showOptions._shape.getPosition();
	auto size = _showOptions._shape.getSize();

	_showOptions._shape = shape;

	_showOptions._shape.setPosition(pos);
	_showOptions._shape.setSize(size);
}

void CSelect::setSelectedOptionShape(const sf::RectangleShape& shape) {
	auto pos = _selectedOption._shape.getPosition();
	auto size = _selectedOption._shape.getSize();

	_selectedOption._shape = shape;

	_selectedOption._shape.setPosition(pos);
	_selectedOption._shape.setSize(size);
}

void CSelect::setActive(S32 active) { 
	ASSERTF(active >= _options.size(), "wrong option in CSelect. %i option selected, but CSelect has %i options", active, _options.size());
	_selectedValue = active;
	_selectedOption.setText(_options[_selectedValue].getText().getString()); 
}


sf::FloatRect CSelect::getRect() const {
	return _selectedOption.getRect();
}

sf::Vector2f	CSelect::getPos() const {
	return _selectedOption.getPos();
}

sf::Vector2f	CSelect::getSize() const {
	return _selectedOption.getSize();
}

void CSelect::update(CWindow& wnd) {
	_showOptions.update(wnd);

	if (_isShown) {

		for (size_t i = 0; i < _options.size(); ++i) {
			if (zog::mouse().isReleased(0) && _options[i].getRect().contains(zog::mouse().getPositionF())) {
				_selectedOption.setText(_options[i].getText().getString());
				_selectedValue = i;
				showHide();
				return;
			}
		}

		if (zog::mouse().isReleased(0) && !_showOptions.getRect().contains(zog::mouse().getPositionF()))
			showHide();
	}
}


void CSelect::draw(CWindow& wnd) {
	_selectedOption.draw(wnd);
	_showOptions.draw(wnd);
	if (_isShown) {
		for (auto& i : _options) {
			auto pos = i._shape.getPosition();
			auto size = i._shape.getSize();

			if (i.getRect().contains(zog::mouse().getPositionF()))
				i._shape = _optionOnMouseShape;
			else
				i._shape = _optionsShape;

			i._shape.setPosition(pos);
			i._shape.setSize(size);

			i.draw(wnd);
		}
	}
	wnd.draw(_shape);
}