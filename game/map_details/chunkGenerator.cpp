#include "chunkGenerator.hpp"


void CChunkGenerator::generateChunk(CChunk* chunk, const bfs::path& map, const sf::Vector2i& chunkNumber) const {
	DLog("CChunkGenerator:: Generating chunk [%i, %i]", chunkNumber.x, chunkNumber.y);
	chunk->_chunkNumber.x = chunkNumber.x;
	chunk->_chunkNumber.y = chunkNumber.y;
	chunk->loadTestChunk();
	DLog("CChunkGenerator:: Chunk generate! [%i, %i]", chunkNumber.x, chunkNumber.y);
}
