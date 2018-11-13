#include "fontManager.hpp"
#include "base/cfgSystem.hpp"

sf::Font& CFontManager::getFont(const shared_str& name) {
    auto finder = fonts.find(name);
    if (finder != fonts.end())
        return *(finder->second);

    auto* font = new sf::Font();


    font->loadFromFile((zog::readConfig().readPath("fonts") / zog::readConfig().r_string(name, "path").c_str()).c_str());
    fonts[name] = font;
    return *font;
}