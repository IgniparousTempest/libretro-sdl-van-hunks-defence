#include "player_cannon.hpp"

PlayerCannon::PlayerCannon(Assets* assets, int shipX, int shipY) {
    texture = assets->get("cannon");
    auto [ w, h ] = assets->dimensions("cannon");
    rect.x = shipX + 87 - w / 2.0f;
    rect.y = shipY + 33 - h / 2.0f;
    rect.w = w;
    rect.h = h;
}

void PlayerCannon::recalculateAngle(float crosshairX, float crosshairY) {
    float x1 = crosshairX;
    float x2 = rect.x + rect.w / 2.0f;
    float y1 = crosshairY;
    float y2 = rect.y + rect.h / 2.0f;
    angle = atan2(y1 - y2, x1 - x2) * 180.0f / M_PI;
}

void PlayerCannon::Render(SDL_Renderer *renderer) {
    SDL_RenderCopyExF(renderer, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
}
