#ifndef _FONT_MANAGER_HPP_
#define _FONT_MANAGER_HPP_

#include <unordered_map>
#include <SFML/Graphics/Font.hpp>
#include "base/new_types/sharedString.hpp"

class CFontManager {
private:
    CFontManager() = default;
    ~CFontManager() {
        for (auto i : fonts) {
            delete i.second;
        }
    }
public:
    CFontManager(const CFontManager&) = delete;
    CFontManager& operator=(const CFontManager&) = delete;

public:
    static CFontManager& inst() {
        static CFontManager instance;
        return instance;
    }

private:
    std::unordered_map<shared_str, sf::Font*> fonts;

public:
    sf::Font& getFont(const shared_str& name);
};

namespace base {
    inline CFontManager& FontManager() {
        return CFontManager::inst();
    }
}


#endif //_FONT_MANAGER_HPP_