#include "cannon_ball_trail.hpp"

CannonBallTrail::CannonBallTrail(Assets *assets, float x, float y) {
    texture = assets->get("cannon_ball_trail");
    auto [w, h] = assets->dimensions("cannon_ball_trail");
    rect.x = x - w / 2;
    rect.y = y - h / 2;
    rect.w = w;
    rect.h = h;
}

void CannonBallTrail::Render(SDL_Renderer *renderer, float ballCentreX, float ballCentreY, float distance, float angle) {
    float length = std::fmin(distance, rect.h);
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = rect.w;
    src.h = length;
    SDL_FRect dst;
    dst.x = ballCentreX - rect.w / 2.0f;
    dst.y = ballCentreY;
    dst.w = rect.w;
    dst.h = length;
    SDL_FPoint centre;
    centre.x = rect.w / 2.0f;
    centre.y = 0;
    SDL_RenderCopyExF(renderer, texture, &src, &dst, angle + 90, &centre, SDL_FLIP_NONE);
}
