#ifndef _CHUNK_HPP_
#define _CHUNK_HPP_
#include <vector>
#include "tileManager.hpp"

#include "../../base/cfgSystem.hpp"

#define CHUNK_SIZE 64

///////////////////////// CHUNK CLASS ///////////////////////////////
//
//	Consist of 64x64 terrain tiles, collision boxes and objects
//
////////////////////////////////////////////////////////////////////
class CChunkManager;
class CChunkGenerator;
class CChunk {
protected:
	friend CChunkManager;
	friend CChunkGenerator;
	STile				_terrainTable[CHUNK_SIZE][CHUNK_SIZE];
	sf::Vector2i		_chunkNumber;		// chunk number pos in global coordinates
	bool				_needToLoad;		// need to load from file

public:
	CChunk(const bfs::path& path, const sf::Vector2i& chunkNumber);					// creating chunk
	CChunk() = default;
	~CChunk();



public:
	const sf::Vector2i& getChunkNumber() { return _chunkNumber; }
	S32					getChunkNumberX() { return _chunkNumber.x; }
	S32					getChunkNumberY() { return _chunkNumber.y; }

	//	write in rectangle shape real tile texture
	//
	//	real_tile	- sfml rectangle shape that should be writed
	//	x, y		- position of tile
	void				getRealTile(sf::RectangleShape& real_tile, U32 x, U32 y) { CTileManager::inst().getRealTile(real_tile, _terrainTable[y][x]); }


	// TESTS
public:
	void loadTestChunk();
	void saveTestChunk();
	void renderDummy(std::vector<sf::RectangleShape>& toDraw) const;
};

#endif // !_CHUNK_HPP_
