#ifndef _TILE_MANAGER_HPP_
#define _TILE_MANAGER_HPP_

#include "../../base/types.hpp"

#include <map>
#include <SFML/Graphics/RectangleShape.hpp>


namespace sf { class Texture; }



///////////////////////// TILE MAP CLASS ///////////////////////////////
//
//	Provide acces to tile by position
//
///////////////////////////////////////////////////////////////////////////////
class CTileMap {
protected:
	U32				_tileSize;								// tile size in pixels
	sf::Texture*	_texture;

public:
	CTileMap(sf::Texture* texture, U32 tileSize);

	// Getting tile coordinates in shape. RectangleShape texture sets to _texture
	// shape			- sfml rectangle shape that should be writed
	// tile_position	- tile position (double array number) in texture
	void getTile(sf::RectangleShape& shape, const sf::Vector2u& tile_position) const;  
};





// Todo: сделать байтовое выравнивание

///////////////////////// TILE STRUCT ///////////////////////////////
//
//	Tile which is write and load from file
//
//	tilemap_position_x	- position number at X
//	tilemap_position_y	- position number at Y
//	material_uid		- material UID
//	rotation			- tile transform: 0 - +0 1 - +90 2 - + 180 3 - +270 4 - x-mirroring 5 - y-mirroring 6 - mirroring
//
///////////////////////////////////////////////////////////////////////////////


struct STile {
	U32		tilemap_uid;
	U8		tilemap_position_x;
	U8		tilemap_position_y;
	U8		material_uid;
	U8		rotation;			// 0 - +0 1 - +90 2 - + 180 3 - +270 4 - x-mirroring 5 - y-mirroring 6 - mirroring
};



// Todo: сделать пререндер террайна и статичных объектов. они редко меняются, нет смысла каждый раз дергать тайлменеджер. тем более можно обновлять один конкретный тайл
///////////////////////// TILE MANAGER SINGLETON ///////////////////////////////
//
//	Load all tilemaps in game format
//	Provide acces to SFML rectangle shape of tile
//
///////////////////////////////////////////////////////////////////////////////
class CTileManager {
private:
	std::map<U32, CTileMap> _pathMap;

public:
	//	write in rectangle shape real tile texture
	//
	//	real_tile	- sfml rectangle shape that should be writed
	//	tile		- engine tile
	void getRealTile(sf::RectangleShape& real_tile, const STile& tile) const;		


// singleton realization
private:
	CTileManager();
	~CTileManager() = default;

public:
	CTileManager(const CTileManager&) = delete;
	CTileManager& operator=(const CTileManager&) = delete;

public:
	static CTileManager& inst() {
		static CTileManager instance;
		return instance;
	}

};





#endif // !_TILE_MANAGER_HPP_
