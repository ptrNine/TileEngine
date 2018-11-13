#include "chunkManager.hpp"
#include "chunk.hpp"
#include "chunkGenerator.hpp"


CChunkManager::CChunkManager(const shared_str& map) {
    Log("Creating chunk manager...");
    _mapPath = zog::readConfig().readPath("maps") / map.c_str();
    _mapChunksInfoPath = _mapPath / "chunks.dat";


    // loading chunk info file
    R_ASSERTF(zog::readConfig().isFileExist(_mapChunksInfoPath), "Can't find chunks information file in %s", _mapChunksInfoPath.string().c_str());

    bytes_t file;
    zog::readConfig().readFile(file, _mapChunksInfoPath);

    auto filesize = file.size() / sizeof(U64);
    Log("Loading %i chunks info...", filesize);

    U64* ptr = nullptr;
    if (filesize > 0)
        ptr = reinterpret_cast<U64*>(&file.at(0));

    _avaibleChunks.reserve(filesize);
    for (size_t i = 0; i < filesize; ++i) {

#ifdef DEBUG
        auto tmp = CChunkManager::CU64DoubleS32(*ptr);
        S32 x_ = tmp._x;
        S32 y_ = tmp._y;

        DLog("chunk registered at [%i, %i]", x_, y_);
#endif // DEBUG


        _avaibleChunks.push_back(*ptr);
        ptr++;
    }
    Log("Loaded! Sorting...");
    std::sort(_avaibleChunks.begin(), _avaibleChunks.end());
}

CChunkManager::~CChunkManager() {

}

CChunk* CChunkManager::createChunk(const sf::Vector2i& chunkNumber) const {
    U64 key = CU64DoubleS32(chunkNumber.x, chunkNumber.y).asU64();
    if (std::find(_avaibleChunks.begin(), _avaibleChunks.end(), key) != _avaibleChunks.end()) {
        Log("!!!CChunkManager:: Finded: [%i, %i]", chunkNumber.x, chunkNumber.y);
        return new CChunk(_mapPath, chunkNumber);
    }
    else {

        Log("!!!CChunkManager:: NOT Finded: [%i, %i]", chunkNumber.x, chunkNumber.y);
        return generateChunk(chunkNumber);
    }
}

void CChunkManager::destroyChunk(CChunk* chunk) const {
    delete chunk;
}

CChunk* CChunkManager::generateChunk(const sf::Vector2i& chunkNumber) const {
    Log("CChunkManager:: Generating chunk [%i, %i]", chunkNumber.x, chunkNumber.y);

    // создаем пустой чанк
    auto* chunk = new CChunk();

    // генерацией занимается чанк генератор
    CChunkGenerator::inst().generateChunk(chunk, _mapPath, chunkNumber);

    // Создаем и открываем файл чанка для записи
    shared_str chunkName;
    chunkName.sprintf("%i_%i.chnk", chunkNumber.x, chunkNumber.y);
    CFileWritter fw(_mapPath / chunkName.c_str());

    // Начинаем писать чанк
    fw.Write(chunk->_terrainTable);
    // Заканчиваем писать чанк

    // Вносим в базу сгенереных чанков
    U64 key = CU64DoubleS32(chunkNumber.x, chunkNumber.y).asU64();
    _avaibleChunks.push_back(key);
    CFileWritter(_mapChunksInfoPath, true).Write(key);


    Log("CChunkManager:: Chunk generate! [%i, %i]", chunkNumber.x, chunkNumber.y);
    return chunk;
}
