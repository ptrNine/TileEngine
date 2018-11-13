#include "chunk.hpp"


// Creating chunk. If chunk already exist load them from file, else generate chunk from patterns
CChunk::CChunk(const bfs::path& path, const sf::Vector2i& chunkNumber) {
    DLog("!!!!!!!!!!!!Loading chunk in [%i, %i]", chunkNumber.x, chunkNumber.y);

    _chunkNumber = chunkNumber;
    Log("Creating chunk [%i, %i]", _chunkNumber.x, _chunkNumber.y);

    //вычисляем путь до файла
    shared_str map_name;
    map_name.sprintf("%i_%i.chnk", chunkNumber.x, chunkNumber.y);
    auto map_path = path / map_name.c_str();

    // loading this shit
    CFileReader file(map_path);

    // Начинаем читать файл

    file.Read(_terrainTable);

    // Заканчиваем читать файл
}

CChunk::~CChunk() {
    printf("destroying chunk in [%i, %i]\n", _chunkNumber.x, _chunkNumber.y);
}

void CChunk::loadTestChunk() {
    Log("load test chunk");
    auto tilemap = zog::readConfig().r_string("tilemap_generator", "texture");

    std::vector<STile> tileset; //доступные тайлы

    // считываем символьные обозначения
    U32 cur = 'a';

    while (true) {
        std::string aaa = " ";
        aaa[0] = cur;
        if (zog::readConfig().is_value_exist("tilemap_generator", aaa.c_str())) {
            auto size = zog::readConfig().r_array<U8>("tilemap_generator", aaa.c_str());

            auto newtile = STile();
            newtile.rotation = 0;
            newtile.material_uid = 0;
            newtile.tilemap_position_x = size[0];
            newtile.tilemap_position_y = size[1];
            newtile.tilemap_uid = hashes::fnv1(tilemap.c_str(), tilemap.length());
            tileset.push_back(newtile);

            cur++;
        }
        else {
            break;
        }
    }

    //////////read and parse asci map file
    auto map_path = zog::readConfig().readPath("maps");
    map_path /= "test_chunk";
    map_path /= "test.chnk_asci";

    std::string file;
    zog::readConfig().readFile(file, map_path);

    U32 asciBegingCode = 'a';
    size_t asciLastCode = asciBegingCode + tileset.size();

    auto terrainPtr = _terrainTable[0];
    for (auto i : file) {
        if (i >= asciBegingCode && i < asciLastCode) {
            *terrainPtr = tileset[i - 'a'];
            terrainPtr++;
        }
    }
    Log("Test chunk loaded");
}




void CChunk::renderDummy(std::vector<sf::RectangleShape>& toDraw) const {
    sf::RectangleShape rect;
    toDraw.reserve(CHUNK_SIZE * CHUNK_SIZE);
    for (size_t i = 0; i < CHUNK_SIZE; ++i) {
        for (size_t j = 0; j < CHUNK_SIZE; ++j) {
            rect.setPosition(j * 32, i * 32);
            rect.setSize(sf::Vector2f(32, 32));
            CTileManager::inst().getRealTile(rect, _terrainTable[i][j]);
            toDraw.push_back(rect);
        }
    }
}