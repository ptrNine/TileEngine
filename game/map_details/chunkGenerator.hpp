#ifndef _CHUNK_GENERATOR_HPP_
#define _CHUNK_GENERATOR_HPP_

#include "chunk.hpp"

///////////////////////// CHUNK GENERATOR SINGLETON ///////////////////////////////
//
//	Создает чанки согласно картам объектов, ландшафту и прочим штукам,
//	которые пока не реализованы. Пока это просто радном-джен тайтлов
//	Используется из объектов СЧанк для их генерации и только, т.к. больше нигде не нужен
//	и в дальнейшем используется лишь сгенерированный объект СЧанк,
//	в котором уже достаточно информации для игрового процесса
//	(включая зоны структур для ориентирования НПС)
//
////////////////////////////////////////////////////////////////////
class CChunkGenerator {
public:
    void generateChunk(CChunk* chunk, const bfs::path& map, const sf::Vector2i& chunkNumber) const;

private:
    CChunkGenerator()  = default;
    ~CChunkGenerator() = default;

public:
    CChunkGenerator(const CChunkGenerator&) = delete;
    CChunkGenerator& operator=(const CChunkGenerator&) = delete;

public:
    static CChunkGenerator& inst() {
        static CChunkGenerator instance;
        return instance;
    }
};


#endif // !_CHUNK_GENERATOR_HPP_
