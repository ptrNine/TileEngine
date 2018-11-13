#ifndef _TEXTURE_MANAGER_HPP_
#define _TEXTURE_MANAGER_HPP_

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include "base/cfgSystem.hpp"


//////////////// TEXTURE MANAGER SINGLETON /////////////////////////
//
//	Give texture, load texture from texture path if it's not already loading
//
////////////////////////////////////////////////////////////////////
class CTextureManager {
private:
    CTextureManager() {
        path = zog::readConfig().readPath("textures");
    }
    ~CTextureManager() {
        for (auto i : textures) {
            delete i.second;
        }
    }
public:
    CTextureManager(const CTextureManager&) = delete;
    CTextureManager& operator=(const CTextureManager&) = delete;

public:
    static CTextureManager& inst() {
        static CTextureManager instance;
        return instance;
    }

private:
    std::map<sf::String, sf::Texture*> textures;
    std::experimental::filesystem::path		path;

public:
    sf::Texture* load(const sf::String &str) {
        auto texturePath = path / str.toAnsiString().c_str();
        auto finder = textures.find(str);
        if (finder != textures.end())
            return finder->second;

        auto* texture = new sf::Texture();
        texture->loadFromFile(sf::String(texturePath.string()).toAnsiString());

        //texture->setSmooth(true);

        textures[str] = texture;
        return texture;
    }
};

namespace base {
    inline CTextureManager& TextureManager() {
        return CTextureManager::inst();
    }
}


#endif //_TEXTURE_MANAGER_HPP_