#include "Crosshair.hpp"

Crosshair::Crosshair(Assets *assets, enum PlayerId playerId, float x, float y, int xMax, int yMax) : xMax(xMax), yMax(yMax) {
    std::string assetName = "crosshair" + std::to_string(PlayerIdExtensions::Value(playerId));
    texture = assets->get(assetName);
    auto dimensions = assets->dimensions(assetName); //TODO: C++17: auto [ w, h ] =
    auto w = std::get<0>(dimensions);
    auto h = std::get<1>(dimensions);
    rect.x = x - w / 2.0f;
    rect.y = y - h / 2.0f;
    rect.w = w;
    rect.h = h;
}

void Crosshair::moveLeft(float deltaTime) {
    rect.x -= speed * deltaTime;
    if (rect.x < 0)
        rect.x = 0;
}

void Crosshair::moveRight(float deltaTime) {
    rect.x += speed * deltaTime;
    if (rect.x > xMax - rect.w)
        rect.x = xMax - rect.w;
}

void Crosshair::moveUp(float deltaTime) {
    rect.y -= speed * deltaTime;
    if (rect.y < 0)
        rect.y = 0;
}

void Crosshair::moveDown(float deltaTime) {
    rect.y += speed * deltaTime;
    if (rect.y > yMax - rect.h)
        rect.y = yMax - rect.h;
}
