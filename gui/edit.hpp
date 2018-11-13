#ifndef _EDIT_HPP_
#define _EDIT_HPP_

#include "label.hpp"

class CWindow;

class CEdit : public CLabel {
private:
    typedef CLabel inherited;

public:
    CEdit();
    CEdit(sf::String str, const sf::FloatRect& rect, TextAligment alignType = TextAligment::adaptive, U32 align = Aligment::none, bool editable = true);

protected:
    void update		(CWindow& wnd) override;
    void contextMenuCreating() override;


};



#endif // !_EDIT_HPP_
