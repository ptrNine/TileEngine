#include "edit.hpp"
#include "context_menu.hpp"
#include "../base/io/keyboard.hpp"
#include "../base/math/math.hpp"


CEdit::CEdit() : inherited() {

}


CEdit::CEdit(sf::String str, const sf::FloatRect& rect, TextAligment alignType, U32 align, bool editable) : inherited(str, rect, alignType, align, editable) {

}


void CEdit::update(CWindow& wnd) {
    inherited::update(wnd);

    if (_isEdit) {
        if (zog::keyboard().isInput()) {

            if (zog::keyboard().isPressed(sf::Keyboard::Return)) {
                _isEdit = false;
                return;
            }
            else if (zog::keyboard().isDelayHoldOrPress(sf::Keyboard::BackSpace)) {
                // если курсор и конец области выделения на нулевой позиции - удалять ничего не надо
                if (_cursorPos > 0 || _highlighterPos > 0) {
                    auto str = _text.getString();
                    // находим границы выделения
                    U32 startPos = math::min(_cursorPos, _highlighterPos);
                    U32 endPos = math::max(_cursorPos, _highlighterPos);

                    // если выделение активно - удаляем область
                    if (endPos > startPos) {
                        str.erase(startPos, endPos - startPos);
                        if (_cursorPos != startPos)
                            _cursorPos -= endPos - startPos;
                    } // просто удаляем один символ
                    else {
                        str.erase(startPos - 1);
                        _cursorPos--;
                    }

                    // обновляем текст, сбрасываем область выделения
                    _text.setString(str);
                    _highlighterPos = _cursorPos;
                    _highlighter.clear();
                    alignText();
                    _cursor.setPosition(_text.findCharacterPos(_cursorPos));
                }
                _cursor.restart();
            }
            else {
                // если ничего не осталось - вводим символы
                U32 input[2];
                input[0] = zog::keyboard().getUnicodeChar();
                sf::String insert = sf::String::fromUtf32(&input[0], &input[1]);
                auto str = _text.getString();
                // находим границы выделения
                U32 startPos = math::min(_cursorPos, _highlighterPos);
                U32 endPos = math::max(_cursorPos, _highlighterPos);

                // если активна область выделения - удаляем текст в области
                if (endPos > startPos) {
                    str.erase(startPos, endPos - startPos);
                    if (_cursorPos != startPos)
                        _cursorPos -= endPos - startPos;
                }

                // вставляем символ
                str.insert(_cursorPos, insert);
                _cursorPos++;

                // обновляем текст
                _text.setString(str);
                _highlighterPos = _cursorPos;
                _highlighter.clear();
                alignText();
                _cursor.setPosition(_text.findCharacterPos(_cursorPos));
                _cursor.restart();
            }
        }
    }

}

// TODO:_доделать эдит и весь остальной гуи. замутить поддержку загрузки гуи из xml

void CEdit::contextMenuCreating() {
    inherited::contextMenuCreating();

    _contextMenu->addButton("paste", [&]() {
        printf("paste\n");
    });
    _contextMenu->addButton("cut", [&]() {
        printf("cut\n");
        copyToClipboard();
    });
}