#ifndef _CHUNK_AREA_HPP_
#define _CHUNK_AREA_HPP_

#include "../../base/types.hpp"
#include "../../base/new_types/sharedString.hpp"

#include <map>
#include <SFML/System/Vector2.hpp>



class CChunkManager;
class CChunk;
class CEngineKernel;
class CChunkArea {
protected:
	friend CEngineKernel;


	U32						_renderRadius;		// 1 - min, chunk matrix 3x3
	U32						_matrixScale;		// _renderRadius * 2 + 1
	sf::Vector2i			_onlineRectStart;	// online rectangle dimmensions
	sf::Vector2i			_onlineRectEnd;		// online rectangle dimmensions
	shared_str				_mapName;

	CChunkManager*			_chunkCreator;


	struct SChunkData {
		CChunk* _chunk;
		bool	_isOperated;
	};
	std::map<U64, SChunkData>	_chunkMatrix;


public:
	CChunkArea(const shared_str& map_name);
	~CChunkArea();
	void update(const sf::Vector2f& viewPosition); // position of view in world coordinates

protected:
	CChunk * getChunk(S32 x, S32 y);

public:
	static void getChunkPos(sf::Vector2i* chunkPos, const sf::Vector2f& pos);
	static void getTilePos(sf::Vector2i* chunkPos, sf::Vector2u* tilePos, const sf::Vector2f& pos);
};
#endif // !_CHUNK_AREA_HPP_
