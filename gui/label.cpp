#include "label.hpp"
#include "window.hpp"
#include "../fontManager.hpp"
#include "../textureManager.hpp"
#include "cursorIcons.hpp"
#include "../base/io/mouse.hpp"
#include "../base/io/keyboard.hpp"
#include "context_menu.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

CLabel::CLabel() {
    _shape.setFillColor(sf::Color(0, 0, 0, 0));
    _text.setCharacterSize(32);
    _text.setFillColor(sf::Color(0, 0, 0, 255));
    _text.setFont(base::FontManager().getFont("font_anonymous"));
    setBorder(sf::Color(0, 0, 0, 255), 2);
    _parent = nullptr;
    setAligment(Aligment::none);
    _textAlignType = TextAligment::adaptive;
    _highlighterPos = 0;
    _cursorPos = 0xffffffff;	// :))
    _isEdit = false;
    _cursor.create(CTextureManager::inst().load("cursor.png"), sf::Vector2u(1, 1), 500, 2);
    _highlighterSample.setFillColor(sf::Color(0, 255, 255, 127));
    refreshCursor();
    _editable = false;
    _contextMenu = nullptr;
}


CLabel::CLabel(sf::String str, const sf::FloatRect& rect, TextAligment alignType, U32 align, bool editable) : CLabel() {
    this->_text.setString(str);
    setRect(rect);
    _textAlignType = alignType;
    alignText();
    setAligment(align);
    _editable = editable;
}


CLabel::~CLabel() {
    delete _contextMenu;
}

void CLabel::sizeAdaptiveSetSize(const sf::Vector2f& size) {
    _shape.setSize(size);
}


void CLabel::alignText() {
    // Reset position
    auto textBounds = _text.getLocalBounds();
    _text.setPosition(getPos());

    // Moving to left-top bound
    _text.move(-textBounds.left, -textBounds.top);

    float outlineThickness = _shape.getOutlineThickness();
    sf::Vector2f tempVec;

    switch (_textAlignType) {
    case TextAligment::centered :
        tempVec = _shape.getSize();
        textBounds = _text.getLocalBounds();
        tempVec.x -= textBounds.width;
        tempVec.y -= textBounds.height;
        tempVec.x /= 2.0f;
        tempVec.y /= 2.0f;
        _text.move(tempVec);
        break;

    case TextAligment::sizeAdaptive :
        textBounds = _text.getLocalBounds();
        _text.move(outlineThickness + 2.0f, outlineThickness + 2.0f);
        sizeAdaptiveSetSize(sf::Vector2f(textBounds.width + outlineThickness * 2.0f + 4, textBounds.height + outlineThickness * 2.0f + 4));
        break;

    }
    //switch (_textAlignType) {
    //case TextAligment::adaptiveMaximized:
    //	float shapeH = _shape.getSize().y - _shape.getOutlineThickness() * 2.f;
    //	_text.setCharacterSize(U32(shapeH));
    //case TextAligment::adaptive:
    //	_text.setPosition(_shape.getPosition());
    //	float shapeW = _shape.getSize().x - _shape.getOutlineThickness() * 2.f;
    //	float shapeH = _shape.getSize().y - _shape.getOutlineThickness() * 2.f;
    //	float textW = _text.getLocalBounds().width;
    //
    //	if (textW > shapeW)
    //		_text.setCharacterSize(U32(_text.getCharacterSize() * (shapeW / textW)));
    //
    //
    //	float textH = _text.getLocalBounds().height;
    //
    //	if (textH > shapeH)
    //		_text.setCharacterSize(U32(_text.getCharacterSize() * (shapeH / textH)));
    //
    //	auto bounds = _text.getLocalBounds();
    //	sf::Vector2f padding((shapeW - bounds.width) / 2.0f, (shapeH - bounds.height) / 2.0f - bounds.top);
    //	_text.move(padding);
    //	break;
    //}
}


void CLabel::setFont(const sf::Font& font) {
    _text.setFont(font);
    refreshCursor();
}


void CLabel::setText(const sf::String &str, U32 size, const sf::Color &color) {
    _text.setCharacterSize(size);
    _text.setString(str);
    _text.setFillColor(color);
    alignText();
}

void CLabel::setTextColor(const sf::Color& color) {
    _text.setFillColor(color);
}

void CLabel::setText(const sf::String& str) {
    this->_text.setString(str);
    alignText();
}


void CLabel::setTextAligment(TextAligment type) {
    _textAlignType = type;
    alignText();
}


void CLabel::setFontSize(U32 size) {
    if (_textAlignType == TextAligment::adaptive) return;
    _text.setCharacterSize(size);
    alignText();
    refreshCursor();
}


void CLabel::addText(const sf::String& str) {
    this->_text.setString(_text.getString() + str);
    alignText();
}


const sf::Text& CLabel::getText() {
    return this->_text;
}


// Virtual methods

void CLabel::setPos(const sf::Vector2f& pos) {
    auto diff = pos - getPos();
    inherited::setPos(pos);
    _text.move(diff);
}


void CLabel::setSize(const sf::Vector2f& size) {
    if (_textAlignType == TextAligment::sizeAdaptive)
        return;

    inherited::setSize(size);
    alignText();
}

void CLabel::resize(const sf::Vector2f& factors, const sf::Vector2f& offset) {
    printf("%f %f %f %f\n", factors.x, factors.y, offset.x, offset.y);
    inherited::resize(factors, offset);

    if (_contextMenu != nullptr)
        _contextMenu->resize(factors, offset);
}


void CLabel::move(const sf::Vector2f& offset) {
    inherited::move(offset);
    _text.move(offset);
}

void CLabel::setEditiable(bool editing) {
    if (!_editable && editing)
        contextMenuCreating();
    _editable = editing;
}


void CLabel::contextMenuCreating() {
    _contextMenu = new CContextMenu(sf::FloatRect(0, 0, 100, 20), _align);
    _contextMenu->_parent = this;

    CButton btn;
    btn.setFontSize(16);
    btn.setTextColor(sf::Color(200, 200, 200, 255));
    btn.setFilling(sf::Color(25, 25, 25, 255));
    btn.setBorderColor(sf::Color(5, 5, 5, 255));
    btn.setPressFilling(sf::Color(100, 100, 100, 255));
    btn.setPressBorderColor(sf::Color(50, 50, 50, 255));

    _contextMenu->setThemeFromButton(btn);
    _contextMenu->addButton("copy", [&]() {
        copyToClipboard();
    });
}


void CLabel::update(CWindow& wnd) {
    inherited::update(wnd);

    if (_editable && (_contextMenu == nullptr || !_contextMenu->isShown())) {

        // созданм контекстное меню, если еще не создали
        if (_contextMenu == nullptr)
            contextMenuCreating();

        // update cursor
        auto mousePos = zog::mouse().getPositionF();
        bool onMouse = _shape.getGlobalBounds().contains(mousePos);

        if (onMouse && zog::mouse().isPressed(sf::Mouse::Left) && !zog::mouse().isCaptured()) {
            _isEdit = true;
            _cursor.restart();
        }
        else if (zog::mouse().isPressed(sf::Mouse::Left)) {
            _isEdit = false;
        }

        if (_isEdit) {
            zog::mouse()._capture();
            size_t symbol = 0;
            size_t strSize = _text.getString().getSize();

            // Holding, calculate cursor position

            if (onMouse && zog::mouse().isHolding(sf::Mouse::Left) && !zog::mouse().isDoubleHolding(sf::Mouse::Left) && _text.getString().getSize() != 0) {
                for (size_t i = 1; i < strSize; ++i) {
                    auto pos = _text.findCharacterPos(i);
                    if (pos.y < mousePos.y) {
                        if (pos.x < mousePos.x)
                            symbol = i;
                    }
                    else {
                        if (_text.getString()[symbol] == '\n' && symbol > 0)
                            symbol--;
                        break;
                    }
                }
                auto bounds = _text.getFont()->getGlyph(_text.getString()[symbol], _text.getCharacterSize(), _text.getStyle() & 1U, _text.getOutlineThickness()).bounds;
                auto pos = _text.findCharacterPos(symbol);
                float diff = mousePos.x - pos.x;
                if (((bounds.width - diff) / bounds.width) < 0.2f)
                    symbol++;
            }
            else {
                symbol = _cursorPos;
            }


            // First click
            if (onMouse) {
                if (zog::mouse().isPressed(sf::Mouse::Left)) {
                    if (!zog::keyboard().isHolding(sf::Keyboard::LShift))
                        _highlighterPos = symbol;
                    _cursor.restart();
                    _highlighter.clear();
                }
                if (zog::mouse().isDoublePressed(sf::Mouse::Left)) {
                    _highlighterPos = 0;
                    symbol = strSize;
                    _cursor.restart();
                    _highlighter.clear();
                }
                // context menu
                if (zog::mouse().isReleased(sf::Mouse::Right)) {
                    if (_contextMenu != nullptr) {
                        _contextMenu->show();
                        _contextMenu->setPos(zog::mouse().getPositionF());
                    }
                }
            }

            // Keyboard update
            if (_highlighterPos != symbol && zog::keyboard().isHolding(sf::Keyboard::LControl) && zog::keyboard().isPressed(sf::Keyboard::C))
                copyToClipboard();


            if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Right) && symbol < strSize)
                symbol++;
            if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Left) && symbol > 0)
                symbol--;
            if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Up) && symbol > 0) {
                size_t newSymbolPos = symbol;

                while (newSymbolPos > 0 && _text.getString()[newSymbolPos] != '\n')
                    newSymbolPos--;

                if (_text.getString()[newSymbolPos] == '\n')
                    newSymbolPos--;

                float difference = std::abs(_text.findCharacterPos(newSymbolPos).x - _text.findCharacterPos(symbol).x);
                while (newSymbolPos > 0) {
                    newSymbolPos--;
                    float newDiff = std::abs(_text.findCharacterPos(newSymbolPos).x - _text.findCharacterPos(symbol).x);
                    if (newDiff < difference) {
                        difference = newDiff;
                    }
                    else {
                        newSymbolPos++;
                        break;
                    }
                }
                symbol = newSymbolPos;
            }
            if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Down) && symbol < strSize) {
                size_t newSymbolPos = symbol;

                while (newSymbolPos < strSize && _text.getString()[newSymbolPos] != '\n')
                    newSymbolPos++;

                if (_text.getString()[newSymbolPos] == '\n')
                    newSymbolPos++;

                float difference = std::abs(_text.findCharacterPos(newSymbolPos).x - _text.findCharacterPos(symbol).x);
                while (newSymbolPos < strSize) {
                    newSymbolPos++;
                    float newDiff = std::abs(_text.findCharacterPos(newSymbolPos).x - _text.findCharacterPos(symbol).x);
                    if (newDiff < difference) {
                        difference = newDiff;
                    }
                    else {
                        newSymbolPos--;
                        break;
                    }
                }
                symbol = newSymbolPos;
            }


            if ((zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Right) ||
                zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Left) ||
                zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Up) ||
                zog::keyboard().isDelayHoldOrPress(sf::Keyboard::Down)) &&
                !zog::keyboard().isHolding(sf::Keyboard::LShift))
                _highlighterPos = symbol;



            // If cursor has been moved
            if (symbol != _cursorPos) {
                _cursor.setPosition(_text.findCharacterPos(symbol));
                _cursor.restart();
            }

            // Redraw highlight box if cursor has been moved
            if (symbol != _cursorPos) {				// symbol != _highlighterPos (?)
                _highlighter.clear();
                size_t min = symbol, max = _highlighterPos;
                if (min > max)
                    std::swap(min, max);

                auto& text = _text.getString();
                while (text[min] == '\n') min++;
                bool setFirst = true;
                float lineHeight = _text.getFont()->getLineSpacing(_text.getCharacterSize());

                for (size_t i = min; i < max + 1; ++i) {
                    if (setFirst) {
                        auto pos = _text.findCharacterPos(i);
                        _highlighter.push_back(_highlighterSample);
                        _highlighter.back().setPosition(pos);
                        setFirst = false;
                        continue;
                    }
                    if (text[i] == '\n' || (i == max)) {
                        auto pos = _text.findCharacterPos(i);
                        pos.y += lineHeight;
                        _highlighter.back().setSize(pos - _highlighter.back().getPosition());
                        while ((i + 1) < (max - 1) && text[i + 1] == '\n') i++;
                        setFirst = true;
                    }
                }
            }

            // Must be last
            _cursorPos = symbol;
        }
    }

    // обновляем контекстное меню
    if (_contextMenu != nullptr)
        _contextMenu->update(wnd);
}


void CLabel::draw(CWindow& wnd) {
    inherited::draw(wnd);
    if (_isEdit) {
        for (auto& i : _highlighter) {
            wnd.draw(i, _transform);
        }
        wnd.draw(_cursor, _transform);
    }
    wnd.draw(this->_text, _transform);

    if (_editable && getRect().contains(zog::mouse().getPositionF()))
        CCursorIcons::instance().setType(CCursorIcons::Type::text);

    // рисуем контекстное меню
    if (_isEdit && _contextMenu != nullptr)
        _contextMenu->draw(wnd);
}


#ifdef _WIN32

void ToClipboard(const wchar_t* text)
{
    if (OpenClipboard(0))
    {
        EmptyClipboard();
        wchar_t* clip_data = (wchar_t*)(GlobalAlloc(GMEM_FIXED, MAX_PATH));
        lstrcpyW(clip_data, text);
        SetClipboardData(CF_UNICODETEXT, (HANDLE)(clip_data));
        LCID* lcid = (DWORD*)(GlobalAlloc(GMEM_FIXED, sizeof(DWORD)));
        *lcid = MAKELCID(MAKELANGID(LANG_RUSSIAN, SUBLANG_NEUTRAL), SORT_DEFAULT);
        SetClipboardData(CF_LOCALE, (HANDLE)(lcid));
        CloseClipboard();
    }
}

#endif


void CLabel::copyToClipboard() {
    size_t min = _cursorPos, max = _highlighterPos;
    if (min > max)
        std::swap(min, max);

    auto str = _text.getString().substring(min, max - min);

#ifdef _WIN32
    ToClipboard(str.toWideString().c_str());
#endif
}


void CLabel::refreshCursor() {
    _cursor.setSize(sf::Vector2f(2.0f, float(_text.getFont()->getLineSpacing(_text.getCharacterSize()))));
    auto color = _shape.getFillColor();
    if ((size_t(color.r) + size_t(color.g) + size_t(color.b)) / 3 > 127)
        _cursor.setAreaStart(sf::Vector2u(0, 1));
    else
        _cursor.setAreaStart(sf::Vector2u(0, 0));
}