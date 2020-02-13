#include "Crosshair.hpp"

Crosshair::Crosshair(Assets *assets, enum PlayerId playerId, float x, float y, int xMax, int yMax) :
        GameObject(assets->get("crosshair" + std::to_string(PlayerIdExtensions::Value(playerId))), x, y), xMax(xMax), yMax(yMax) {
}

void Crosshair::moveLeft(float deltaTime) {
    ShiftX(-speed * deltaTime);
    if (x() < 0)
        SetX(0);
}

void Crosshair::moveRight(float deltaTime) {
    ShiftX(speed * deltaTime);
    if (x() > xMax - w())
        SetX(xMax - w());
}

void Crosshair::moveUp(float deltaTime) {
    ShiftY(-speed * deltaTime);
    if (y() < 0)
        SetY(0);
}

void Crosshair::moveDown(float deltaTime) {
    ShiftY(speed * deltaTime);
    if (y() > yMax - h())
        SetY(yMax - h());
}
