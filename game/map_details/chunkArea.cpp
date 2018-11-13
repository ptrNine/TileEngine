#include "chunkArea.hpp"
#include "../../base/cfgSystem.hpp"
#include "chunk.hpp"
#include "chunkManager.hpp"

CChunkArea::CChunkArea(const shared_str& map_name) {
	Log("Create CChunkArea");
	_mapName = map_name;
	_chunkCreator = new CChunkManager(map_name);
	_renderRadius = zog::readConfig().r_u32("online_params", "online_radius");
	_matrixScale = _renderRadius * 2 + 1;
}

CChunkArea::~CChunkArea() {
	Log("destroing CChunkArea");
	delete _chunkCreator;
}

void CChunkArea::update(const sf::Vector2f& viewPosition) {
	//DLog("update chunks by [%f, %f] view", viewPosition.x, viewPosition.y);
	// определяем, в каком чанке находится вьюпоинт
	// Todo: преобразование float to int
	sf::Vector2i chunkNumber;
	getChunkPos(&chunkNumber, viewPosition);

	_onlineRectStart = sf::Vector2i(chunkNumber.x - _renderRadius, chunkNumber.y - _renderRadius);
	_onlineRectEnd = sf::Vector2i(chunkNumber.x + _renderRadius + 1, chunkNumber.y + _renderRadius + 1);
	
	// помечаем все чанки как необрабатываемые
	for (auto& i : _chunkMatrix)
		i.second._isOperated = false;

	// цикл по требуемым чанкам
	for (auto i = _onlineRectStart.y; i < _onlineRectEnd.y; ++i) {
		for (auto j = _onlineRectStart.x; j < _onlineRectEnd.x; ++j) {
			// вычисляем ключ, это х и у записанные подряд
			U64 key = CChunkManager::CU64DoubleS32(j, i).asU64();

			// ищем чанк с данной позицией в таблице
			auto findingChunk = _chunkMatrix.find(key);
			if (findingChunk != _chunkMatrix.end()) {	// если чанк найден, помечаем его
				findingChunk->second._isOperated = true;
			}
			else {		// если чанк не нашли, создаем его (класс чанка загружает его из файла) и помечаем
				Log("UPDATE");
				// Todo: требуется реализация загрузки чанка в другом потоке
				auto newChunk = SChunkData();
				newChunk._chunk = _chunkCreator->createChunk(sf::Vector2i(j, i)); // просто создаем чанк в памяти. нас не интересует, сгенерен он или нет
				newChunk._isOperated = true;
				_chunkMatrix.emplace(std::make_pair(key, newChunk));
			}
		}
	}
	
	// удаляем чанки, выпавшие за область
	std::vector<U64> toErase;
	for (auto& i : _chunkMatrix) {
		if (!i.second._isOperated) {
			_chunkCreator->destroyChunk(i.second._chunk);
			toErase.push_back(i.first);
		}
	}

	for (auto& i : toErase)
		_chunkMatrix.erase(i);

	//printf("online rect: [%i, %i] to [%i, %i]. viewpos = %f, %f\n", _onlineRectStart.x, _onlineRectStart.y, _onlineRectEnd.x, _onlineRectEnd.y, viewPosition.x, viewPosition.y);

}

void CChunkArea::getChunkPos(sf::Vector2i* chunkPos, const sf::Vector2f& pos) {
	chunkPos->x = int(round((pos.x - CHUNK_SIZE * 0.5) / CHUNK_SIZE + std::numeric_limits<double>::epsilon()));
	chunkPos->y = int(round((pos.y - CHUNK_SIZE * 0.5) / CHUNK_SIZE + std::numeric_limits<double>::epsilon()));
}

void CChunkArea::getTilePos(sf::Vector2i* chunkPos, sf::Vector2u* tilePos, const sf::Vector2f& pos) {
	getChunkPos(chunkPos, pos);

	sf::Vector2f fabsPos(fabs(pos.x), fabs(pos.y));
	if (pos.x >= 0)
		tilePos->x = int(fabsPos.x) % CHUNK_SIZE;
	else
		tilePos->x = int(CHUNK_SIZE - (int(fabsPos.x) % CHUNK_SIZE) - 1) ;

	if (pos.y >= 0)
		tilePos->y = int(fabsPos.y) % CHUNK_SIZE;
	else
		tilePos->y = int(CHUNK_SIZE - (int(fabsPos.y) % CHUNK_SIZE) - 1);
}

CChunk* CChunkArea::getChunk(S32 x, S32 y) { 
	auto chunk = _chunkMatrix.find(CChunkManager::CU64DoubleS32(x, y).asU64());
	return chunk != _chunkMatrix.end() ? chunk->second._chunk : nullptr;
}