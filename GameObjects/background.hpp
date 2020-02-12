#ifndef SUPER_MISSILE_COMMAND_BACKGROUND_HPP
#define SUPER_MISSILE_COMMAND_BACKGROUND_HPP


#include <vector>
#include "../assets.hpp"
#include "../game_object.hpp"
#include "../Animation.hpp"

class Background : public GameObject {
public:
    Background(Assets* assets, int screenWidth, int screenHeight);
    void Render(SDL_Renderer* renderer) override;

private:
    const int scale = 2;
    std::vector<std::vector<SDL_Texture*>> tiles;
    Assets *assets;
    int width;
    int height;
    float tileWidth;
    float tileHeight;
    Animation anim;

    void generateIslands();
    static void waterTiles(const std::vector<std::vector<bool>>& map, int x, int y, int* water, int* possible);
    static void smoothIslands(std::vector<std::vector<bool>>* map);
    static bool getOrTrue(std::vector<std::vector<bool>> map, int x, int y);
    static bool isTopLeftCorner(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isTop(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isTopRightCorner(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isCentreLeft(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isCentreRight(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isBottomLeftCorner(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isBottom(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isBottomRightCorner(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isNSWELand(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isInnerTopLeft(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isInnerTopRight(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isInnerBottomLeft(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isInnerBottomRight(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isSingleHorizontal(const std::vector<std::vector<bool>>& map, int x, int y);
    static bool isSingleVertical(const std::vector<std::vector<bool>>& map, int x, int y);
};


#endif //SUPER_MISSILE_COMMAND_BACKGROUND_HPP
