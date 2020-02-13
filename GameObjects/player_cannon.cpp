#include "player_cannon.hpp"

PlayerCannon::PlayerCannon(Assets* assets, int shipX, int shipY) :
        GameObject(assets->get("cannon"), shipX + 87, shipY + 33) {
}

void PlayerCannon::recalculateAngle(float crosshairX, float crosshairY) {
    float x1 = crosshairX;
    float x2 = centreX();
    float y1 = crosshairY;
    float y2 = centreY();
    angle = atan2(y1 - y2, x1 - x2) * 180.0f / M_PI;
}

void PlayerCannon::Render(SDL_Renderer *renderer) {
    auto rect = Rect();
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
}
