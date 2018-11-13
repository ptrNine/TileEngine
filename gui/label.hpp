#ifndef _LABEL_HPP_
#define _LABEL_HPP_

#include <vector>
#include <SFML/Graphics.hpp>
#include "../base/types.hpp"
#include "panel.h"
#include "../base/AnimatedRect.hpp"


class CContextMenu;

class CLabel : public CPanelBase {
private:
    typedef CPanelBase inherited;
    friend class CSelect;

public:
    enum class TextAligment { sizeAdaptive, adaptive, centered };

protected:
    size_t					_highlighterPos;
    bool					_isEdit;
    size_t					_cursorPos;
    CAnimatedRect			_cursor;
    std::vector<sf::RectangleShape> _highlighter;
    sf::RectangleShape		_highlighterSample;


    void					copyToClipboard();
    void					refreshCursor();

    TextAligment			_textAlignType;
    sf::Text				_text;
    bool					_editable;	// поддержка выделения и редактирования

    CContextMenu*			_contextMenu;

    void alignText();
    virtual void sizeAdaptiveSetSize(const sf::Vector2f& size);

public:
    CLabel	();
    CLabel	(sf::String str, const sf::FloatRect& rect, TextAligment alignType = TextAligment::adaptive, U32 align = Aligment::none, bool editable = false);
    ~CLabel() override;

    void setFont			(const sf::Font& font);
    void setText			(const sf::String& str);
    void addText			(const sf::String& str);
    void setText			(const sf::String &str, U32 size, const sf::Color &color);
    void setTextColor		(const sf::Color& color);
    void setTextAligment	(TextAligment type);
    void setFontSize		(U32 size);

    const sf::Text& getText();

    void setPos					(const sf::Vector2f& pos) override;
    void setSize				(const sf::Vector2f& size) override;
    void move					(const sf::Vector2f& offset) override;
    void resize					(const sf::Vector2f& factors, const sf::Vector2f& offset) override;
    virtual void setEditiable 	(bool editing);				// поддержка редактирования


protected:
    virtual void contextMenuCreating();
    void update		(CWindow& wnd) override;
    void draw		(CWindow& wnd) override;

};

#endif //_LABEL_HPP_