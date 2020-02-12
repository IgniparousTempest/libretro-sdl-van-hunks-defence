#include <iostream>
#include <limits>
#include <stack>
#include "background.hpp"
#include "../PerlinNoise.hpp"

Background::Background(Assets *assets, int screenWidth, int screenHeight) {
    this->assets = assets;
    auto [ tileWidth, tileHeight ] = assets->dimensions("tile_water");
    width = ceil(screenWidth / static_cast<double>(tileWidth)) * scale;
    height = ceil(screenHeight / static_cast<double>(tileHeight)) * scale;
    this->tileWidth = static_cast<float>(screenWidth) / static_cast<float>(width);
    this->tileHeight = static_cast<float>(screenHeight) / static_cast<float>(height);

    anim.MaxFrames = 15;
    anim.SetFrameRate(200);

    generateIslands();
}

void Background::generateIslands() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, std::numeric_limits<uint32_t>::max());

    double frequency = 8.0;
    int octaves = 8;
    double threshold = 0.5;
    auto noise = siv::PerlinNoise(dist(rng));
    auto map = std::vector<std::vector<bool>>();
    for (int y = 0; y < height - scale; ++y) {
        map.emplace_back();
        for (int x = 0; x < width; ++x)
            map.at(y).push_back(noise.octaveNoise0_1(x / frequency, y / frequency, octaves) > threshold);
    }
    // Make bottom all sea
    for (int y = 0; y < scale; ++y) {
        map.emplace_back();
        for (int x = 0; x < width; ++x)
            map.at(height - scale + y).push_back(false);
    }
    smoothIslands(&map);

    tiles = std::vector<std::vector<SDL_Texture*>>();
    for (int y = 0; y < height; ++y) {
        tiles.emplace_back();
        for (int x = 0; x < width; ++x) {
            if (!map.at(y).at(x)) {
                tiles.at(y).push_back(nullptr);
                continue;
            }
            if (isTopLeftCorner(map, x, y))
                tiles.at(y).push_back(assets->get("sand_top_left"));
            else if (isTop(map, x, y))
                tiles.at(y).push_back(assets->get("sand_top"));
            else if (isTopRightCorner(map, x, y))
                tiles.at(y).push_back(assets->get("sand_top_right"));
            else if (isCentreLeft(map, x, y))
                tiles.at(y).push_back(assets->get("sand_centre_left"));
            else if (isCentreRight(map, x, y))
                tiles.at(y).push_back(assets->get("sand_centre_right"));
            else if (isBottomLeftCorner(map, x, y))
                tiles.at(y).push_back(assets->get("sand_bottom_left"));
            else if (isBottom(map, x, y))
                tiles.at(y).push_back(assets->get("sand_bottom"));
            else if (isBottomRightCorner(map, x, y))
                tiles.at(y).push_back(assets->get("sand_bottom_right"));
            else if (isInnerTopLeft(map, x, y))
                tiles.at(y).push_back(assets->get("sand_inner_top_left"));
            else if (isInnerTopRight(map, x, y))
                tiles.at(y).push_back(assets->get("sand_inner_top_right"));
            else if (isInnerBottomLeft(map, x, y))
                tiles.at(y).push_back(assets->get("sand_inner_bottom_left"));
            else if (isInnerBottomRight(map, x, y))
                tiles.at(y).push_back(assets->get("sand_inner_bottom_right"));
            else if (isSingleHorizontal(map, x, y))
                tiles.at(y).push_back(assets->get("sand_single_horizontal"));
            else if (isSingleVertical(map, x, y))
                tiles.at(y).push_back(assets->get("sand_single_vertical"));
            else
                tiles.at(y).push_back(assets->get("sand_centre"));
        }
    }
}

void Background::smoothIslands(std::vector<std::vector<bool>>* map) {
    int water, possible;
    std::stack<std::pair<int, int>> candidates = {};
    for (int y = 0; y < map->size(); ++y) {
        for (int x = 0; x < map->at(y).size(); ++x) {
            // Ignore water tiles
            if (!map->at(y)[x])
                continue;
            candidates.push({x, y});
            while (!candidates.empty()) {
                auto p = candidates.top();
                candidates.pop();
                waterTiles(*map, p.first, p.second, &water, &possible);
                if (map->at(p.second)[p.first] && water >= possible - 1) {
                    map->at(p.second)[p.first] = false;
                    // Add
                    if (p.first > 0) candidates.push({p.first - 1, p.second});
                    if (p.first < map->at(p.second).size() - 2) candidates.push({p.first + 1, p.second});
                    if (p.second > 0) candidates.push({p.first, p.second - 1});
                    if (p.second < map->size() - 2) candidates.push({p.first, p.second + 1});
                }
            }
        }
    }
}

void Background::waterTiles(const std::vector<std::vector<bool>> &map, int x, int y, int *water, int *possible) {
    *water = 0;
    *possible = 0;
    bool value;
    std::vector<std::pair<int, int>> positions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (auto &pos : positions) {
        try {
            value = map.at(y + pos.first).at(x + pos.second);
            *possible += 1;
            if (!value)
                *water += 1;
        } catch (const std::out_of_range& e) {
        }
    }
}

void Background::Render(SDL_Renderer *renderer) {
    anim.Update();
    auto [ wWater, hWater ] = assets->dimensions("tile_water_animated");
    auto water = assets->get("tile_water_animated");
    SDL_Rect src;
    src.x = hWater * anim.GetCurrentFrame();
    src.y = 0;
    src.w = hWater;
    src.h = hWater;

    SDL_FRect dest;
    dest.w = tileWidth;
    dest.h = tileHeight;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            dest.x = tileWidth * static_cast<float>(x);
            dest.y = tileHeight * static_cast<float>(y);
            SDL_RenderCopyF(renderer, water, &src, &dest);
            SDL_RenderCopyF(renderer, tiles.at(y).at(x), nullptr, &dest);
        }
    }
}

bool Background::getOrTrue(std::vector<std::vector<bool>> map, int x, int y) {
    try {
        return map.at(y).at(x);
    } catch (const std::out_of_range& e) {
        return true;
    }
}

bool Background::isTopLeftCorner(const std::vector<std::vector<bool>> &map, int x, int y) {
    return !getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && !getOrTrue(map, x - 1, y);
}

bool Background::isTop(const std::vector<std::vector<bool>> &map, int x, int y) {
    return !getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isTopRightCorner(const std::vector<std::vector<bool>> &map, int x, int y) {
    return !getOrTrue(map, x, y - 1) && !getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isCentreLeft(const std::vector<std::vector<bool>> &map, int x, int y) {
    return getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && !getOrTrue(map, x - 1, y);
}

bool Background::isCentreRight(const std::vector<std::vector<bool>> &map, int x, int y) {
    return getOrTrue(map, x, y - 1) && !getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isBottomLeftCorner(const std::vector<std::vector<bool>>& map, int x, int y) {
    return getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && !getOrTrue(map, x, y + 1) && !getOrTrue(map, x - 1, y);
}

bool Background::isBottom(const std::vector<std::vector<bool>> &map, int x, int y) {
    return getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && !getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isBottomRightCorner(const std::vector<std::vector<bool>> &map, int x, int y) {
    return getOrTrue(map, x, y - 1) && !getOrTrue(map, x + 1, y) && !getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isNSWELand(const std::vector<std::vector<bool>> &map, int x, int y) {
    return getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isInnerTopLeft(const std::vector<std::vector<bool>> &map, int x, int y) {
    return isNSWELand(map, x, y) && !getOrTrue(map, x - 1, y - 1) && getOrTrue(map, x + 1, y - 1) && getOrTrue(map, x - 1, y + 1) && getOrTrue(map, x + 1, y + 1);
}

bool Background::isInnerTopRight(const std::vector<std::vector<bool>> &map, int x, int y) {
    return isNSWELand(map, x, y) && getOrTrue(map, x - 1, y - 1) && !getOrTrue(map, x + 1, y - 1) && getOrTrue(map, x - 1, y + 1) && getOrTrue(map, x + 1, y + 1);
}

bool Background::isInnerBottomLeft(const std::vector<std::vector<bool>> &map, int x, int y) {
    return isNSWELand(map, x, y) && getOrTrue(map, x - 1, y - 1) && getOrTrue(map, x + 1, y - 1) && !getOrTrue(map, x - 1, y + 1) && getOrTrue(map, x + 1, y + 1);
}

bool Background::isInnerBottomRight(const std::vector<std::vector<bool>> &map, int x, int y) {
    return isNSWELand(map, x, y) && getOrTrue(map, x - 1, y - 1) && getOrTrue(map, x + 1, y - 1) && getOrTrue(map, x - 1, y + 1) && !getOrTrue(map, x + 1, y + 1);
}

bool Background::isSingleHorizontal(const std::vector<std::vector<bool>> &map, int x, int y) {
    return !getOrTrue(map, x, y - 1) && getOrTrue(map, x + 1, y) && !getOrTrue(map, x, y + 1) && getOrTrue(map, x - 1, y);
}

bool Background::isSingleVertical(const std::vector<std::vector<bool>> &map, int x, int y) {
    return getOrTrue(map, x, y - 1) && !getOrTrue(map, x + 1, y) && getOrTrue(map, x, y + 1) && !getOrTrue(map, x - 1, y);
}
