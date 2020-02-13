cmake_minimum_required(VERSION 3.7.2)
project(van_hunks_defence)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2TTF REQUIRED)

set(SourcesList GameObjects/player.cpp GameObjects/player.hpp game_object.hpp Auxiliary.hpp assets.hpp GameObjects/background.cpp GameObjects/background.hpp PerlinNoise.hpp GameObjects/cannon_ball.cpp GameObjects/cannon_ball.hpp GameObjects/player_cannon.cpp GameObjects/player_cannon.hpp keyboard_handler.hpp GameObjects/Crosshair.cpp GameObjects/Crosshair.hpp Animation.cpp Animation.hpp GameObjects/Explosion.cpp GameObjects/Explosion.hpp GameObjects/cannon_ball_trail.cpp GameObjects/cannon_ball_trail.hpp GameObjects/EnemyCannonBallFactory.cpp GameObjects/EnemyCannonBallFactory.hpp PlayerId.hpp GameObjects/BurningShip.cpp GameObjects/BurningShip.hpp GameObjects/BurningPlayerShip.hpp GameObjects/BurningTreasureShip.hpp SDL_FontCache/SDL_FontCache.c SDL_FontCache/SDL_FontCache.h GameObjects/Score.cpp GameObjects/Score.hpp Game.cpp Game.hpp Scene/ScoreScreenOverlay.cpp Scene/ScoreScreenOverlay.hpp Timer.hpp Scene/GameOverScreenOverlay.cpp Scene/GameOverScreenOverlay.hpp PlayerId.cpp GameObjects/Flag.hpp geometry.cpp geometry.hpp)

# SDL Executable
add_executable(van_hunks_defence main.cpp ${SourcesList})
include_directories(van_hunks_defence ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(van_hunks_defence ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARIES})

# Libretro library
project(vanhunksdefence_libretro)
add_library(vanhunksdefence_libretro SHARED libretro/libretro.cpp libretro/libretro.hpp libretro/Input.hpp ${SourcesList})
include_directories(vanhunksdefence_libretro ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(vanhunksdefence_libretro ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${SDL2_MIXER_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")  # Removes the 'lib' prefix on Linux

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
