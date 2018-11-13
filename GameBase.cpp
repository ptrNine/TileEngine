#include "GameBase.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "base/cfgSystem.hpp"

#include "game/map_details/chunk.hpp"
#include "game/map_details/chunkArea.hpp"
#include "game/objects/bulletManager.hpp"
#include "game/collisions/collisionPoint.hpp"
#include "game/collisions/collisionManager.hpp"

CEngineKernel::CEngineKernel(sf::RenderWindow* window) {
    _window = window;
    _onlineChunks = nullptr;
    _cameraXFactor = zog::readConfig().r_u32("camera_defines", "standard_x_camera_factor");

    _placeToTile = _window->getView().getSize().x / (_cameraXFactor * 1.0);

    _drawCollisions = false;
}

CEngineKernel::~CEngineKernel() {
    delete _onlineChunks;
}

void CEngineKernel::load_map(const shared_str& map_name) {
    _onlineChunks = new CChunkArea(map_name);
}

void CEngineKernel::update(const vector2d& viewpoint, double timestep) const {
    CEngineState::inst().timestep = timestep;
    // обновление чанков
    _onlineChunks->update(viewpoint.asSfmlVecF());

    // обновление игровых объектов
    for (auto& i : _objects)
        i->update(timestep);

    // обновление пуль
    CBulletManager::inst().update(timestep);

    CCollisionManager::inst().update();
}

// да братан это БЛЯДСКИЙ рендер
void CEngineKernel::draw(const vector2d& viewpoint) const {
    ///////////////////// Рендер мапы
    // Находим половину экрана в метрах
    vector2d window_size = _window->getView().getSize();
    window_size.mul(0.5);
    window_size.div(_placeToTile);

    // Находим углы экрана в метрах
    auto leftUp = viewpoint;
    auto rightDown = viewpoint;
    leftUp.sub(window_size);
    rightDown.add(window_size);

    // Находим начальную позицию рендера со сдвигом
    vector2d renderPos = leftUp;
    auto tmp = renderPos;
    tmp.cutFrac();

    if (renderPos.getX() > 0) {
        renderPos.subX(tmp);
        renderPos.invX();
    }
    else {
        tmp.subX(1.0 + renderPos.getX());
        renderPos.setX(tmp);
    }

    if (renderPos.getY() > 0) {
        renderPos.subY(tmp);
        renderPos.invY();
    }
    else {
        tmp.subY(1.0 + renderPos.getY());
        renderPos.setY(tmp);
    }
    renderPos.mul(_placeToTile);

    // получаем позиции чанков и тайлов относительно чанков в углах
    sf::Vector2i luChunkPos;
    sf::Vector2i rtChunkPos;
    sf::Vector2u luTilePos;
    sf::Vector2u rtTilePos;

    CChunkArea::getTilePos(&luChunkPos, &luTilePos, leftUp.asSfmlVecF());
    CChunkArea::getTilePos(&rtChunkPos, &rtTilePos, rightDown.asSfmlVecF());


    U32 chunksSizeX = rtChunkPos.x - luChunkPos.x + 1;
    U32 chunksSizeY = rtChunkPos.y - luChunkPos.y + 1;

    // проходимся по чанкам
    for (auto i = luChunkPos.y; i <= rtChunkPos.y; ++i) {
        // определяем стартовую и конечную точки по y
        U32 yStart = 0;
        U32 yEnd = CHUNK_SIZE;
        if (i == luChunkPos.y)	yStart = luTilePos.y;
        if (i == rtChunkPos.y)	yEnd = rtTilePos.y + 1;

        // сохраняем текущую позицию рендера по Х
        auto renderPosSave2 = renderPos;

        for (auto j = luChunkPos.x; j <= rtChunkPos.x; ++j) {
            // определяем стартовую и конечную точки по x
            U32 xStart = 0;
            U32 xEnd = CHUNK_SIZE;
            if (j == luChunkPos.x)	xStart = luTilePos.x;
            if (j == rtChunkPos.x)	xEnd = rtTilePos.x + 1;
            // Todo разобраться с nullptr
            // берем чанк. Он может быть nullptr
            auto chunk = _onlineChunks->getChunk(j, i);

            // если не нуллптр - рисуем
            if (chunk != nullptr) {
                // сэйвим позицию
                vector2d renderPosSave = renderPos;
                // цикл по тайлам чанка
                for (auto y = yStart; y < yEnd; ++y) {
                    for (auto x = xStart; x < xEnd; ++x) {
                        // рисуем тайл
                        sf::RectangleShape draw_rect;
                        draw_rect.setPosition(renderPos.getX(), renderPos.getY());
                        draw_rect.setSize(sf::Vector2f(_placeToTile, _placeToTile));
                        chunk->getRealTile(draw_rect, x, y);
                        _window->draw(draw_rect);

                        // сдвиг по иксу
                        renderPos.addX(_placeToTile);
                    }
                    // сброс х
                    renderPos.setX(renderPosSave);
                    // сдвиг по иксу
                    renderPos.addY(_placeToTile);
                }
                // загружаем позицию
                renderPos = renderPosSave;
            }

            // сдвигаем на ширину видимой области чанка по X
            renderPos.addX((xEnd - xStart) * _placeToTile);
        }

        //возвращаем старую позицию
        renderPos.set(renderPosSave2);

        // сдвигаем на высоту видимой области чанка по Y
        renderPos.addY((yEnd - yStart) * _placeToTile);
    }

    ///////////////////// Рендер игровых объектов //////////////////////

    _renderedObjects = 0;
    for (auto& i : _objects) {
        if (i->draw(_window, viewpoint, _placeToTile))
            _renderedObjects++;
    }
    // рендер пуль
    _renderedObjects += CBulletManager::inst().draw(_window, viewpoint, _placeToTile);

    // рисуем коллижнбоксы
    if (_drawCollisions) {
        for (auto cbox : CCollisionManager::inst()._collisionBoxes) {
            cbox->draw(_window, viewpoint, _placeToTile);
        }
    }
}


void CEngineKernel::addObject(CPhysicObject* object) {
    _objects.push_back(object);
}

void CEngineKernel::destroyObject(CPhysicObject* object) {
    auto obj = std::find(_objects.begin(), _objects.end(), object);
    if (obj != _objects.end())
        _objects.erase(obj);
    else
        ASSERTF(obj != _objects.end(), "deleting nullptr object");
}


void CEngineKernel::setCameraXFactor(U32 factor) {
    _cameraXFactor = factor;
    _placeToTile = _window->getView().getSize().x / (_cameraXFactor * 1.0);
}
