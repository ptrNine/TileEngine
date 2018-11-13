#include "tileManager.hpp"

#include "../../base/new_types/sharedString.hpp"
#include "../../base/cfgSystem.hpp"
#include "../../textureManager.hpp"

CTileMap::CTileMap(sf::Texture* texture, U32 tileSize) {
    _texture = texture;
    _tileSize = tileSize;
}

void CTileMap::getTile(sf::RectangleShape& shape, const sf::Vector2u& tile_position) const {
    shape.setTexture(_texture);
    U32 posX = tile_position.x * _tileSize;
    U32 posY = tile_position.y * _tileSize;
    shape.setTextureRect(sf::IntRect(posX, posY, _tileSize, _tileSize));
}


// TODO: нужны тесты для тайлменеджера
CTileManager::CTileManager() {
    // loading all names in section
    std::vector<shared_str> valList;
    shared_str section_name = "tilemap_define";
    zog::readConfig().get_values_list(section_name, valList);

    // loading tilemap sections
    for (const auto& i : valList) {
        auto texture = CTextureManager::inst().load(zog::readConfig().r_string(i, "path").c_str());
        auto tileSize = zog::readConfig().r_u32(i, "size");
        auto tilemap = CTileMap(texture, tileSize);

        // tilemap section name using as input data
        _pathMap.emplace(std::make_pair(hashes::fnv1(i.c_str(), i.length()), tilemap));
    }
}


// TODO если тайл не найден нужно рисовать тайл-заглушку
void CTileManager::getRealTile(sf::RectangleShape& real_tile, const STile& tile) const {
    _pathMap.at(tile.tilemap_uid).getTile(real_tile, sf::Vector2u(tile.tilemap_position_x, tile.tilemap_position_y));
}
