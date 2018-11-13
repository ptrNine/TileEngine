cmake_minimum_required(VERSION 3.1)

project(TileEngine)



set(SFML_DIR "../libs/SFML")
find_package(SFML 2.5 REQUIRED COMPONENTS system window graphics network audio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1" )


set(SOURCES
        base/allocators/array_allocator.cpp
        base/AnimatedRect.cpp
        base/cfgSystem.cpp
        base/fps_counter.cpp
        base/io/keyboard.cpp
        base/io/mouse.cpp
        base/io/tests/keyboard_tests.cpp
        base/logger.cpp
        base/math/game_vector.cpp
        base/math/math.cpp
        base/math/matrix.cpp
        base/new_types/sharedString.cpp
        fontManager.cpp
        game/collisions/collisionLine.cpp
        game/collisions/collisionManager.cpp
        game/collisions/collisionPoint.cpp
        game/collisions/collisionRectangle.cpp
        game/map_details/chunk.cpp
        game/map_details/chunkArea.cpp
        game/map_details/chunkGenerator.cpp
        game/map_details/chunkManager.cpp
        game/map_details/tileManager.cpp
        game/objects/bullet.cpp
        game/objects/bulletManager.cpp
        game/objects/collisionBoxObject.cpp
        game/objects/physicObject.cpp
        game/objects/prop.cpp
        game/objects/rectangleObject.cpp
        game/objects/updatableObject.cpp
        GameBase.cpp
        gui/button.cpp
        gui/button_array.cpp
        gui/component.cpp
        gui/context_menu.cpp
        gui/cursorIcons.cpp
        gui/edit.cpp
        gui/keyboardUpdater.cpp
        gui/label.cpp
        gui/mouseUpdater.cpp
        gui/panel.cpp
        gui/scrollBar.cpp
        gui/select.cpp
        gui/tests/mouse_tests.cpp
        gui/tests/panel_tests.cpp
        gui/window.cpp
        main.cpp
        )

set(HEADERS
        base/allocators/array_allocator.hpp
        base/AnimatedRect.hpp
        base/Asserts.hpp
        base/cfgSystem.hpp
        base/compileTime.hpp
        base/engineBaseTypes.hpp
        base/engineDefines.hpp
        base/fps_counter.hpp
        base/io/keyboard.hpp
        base/io/mouse.hpp
        base/io/tests/keyboard_tests.hpp
        base/logger.hpp
        base/math/game_vector.hpp
        base/math/math.hpp
        base/math/matrix.hpp
        base/new_types/fixedPoint.hpp
        base/new_types/sharedString.hpp
        base/timer.hpp
        base/types.hpp
        fontManager.hpp
        game/collisions/collisionLine.hpp
        game/collisions/collisionManager.hpp
        game/collisions/collisionPoint.hpp
        game/collisions/collisionRectangle.hpp
        game/map_details/chunk.hpp
        game/map_details/chunkArea.hpp
        game/map_details/chunkGenerator.hpp
        game/map_details/chunkManager.hpp
        game/map_details/tileManager.hpp
        game/objects/bullet.hpp
        game/objects/bulletManager.hpp
        game/objects/collisionBoxObject.hpp
        game/objects/physicObject.hpp
        game/objects/prop.hpp
        game/objects/rectangleObject.hpp
        game/objects/updatableObject.hpp
        GameBase.hpp
        gui/button.hpp
        gui/button_array.hpp
        gui/component.hpp
        gui/context_menu.hpp
        gui/cursorIcons.hpp
        gui/edit.hpp
        gui/keyboardUpdater.hpp
        gui/label.hpp
        gui/mouseUpdater.hpp
        gui/panel.h
        gui/scrollBar.hpp
        gui/select.hpp
        gui/tests/mouse_tests.hpp
        gui/tests/panel_tests.hpp
        gui/window.hpp
        textureManager.hpp
        vectorMatrix.hpp
        )


include_directories(${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system stdc++fs )
