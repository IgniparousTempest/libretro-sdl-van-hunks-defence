#include "cannon_ball_trail.hpp"

CannonBallTrail::CannonBallTrail(Assets *assets, float x, float y) :
        GameObject(assets->get("cannon_ball_trail"), x, y) {
}

void CannonBallTrail::Render(SDL_Renderer *renderer, float ballCentreX, float ballCentreY, float distance, float angle) {
    float length = std::fmin(distance, h());
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = w();
    src.h = length;
    SDL_Rect dst;
    dst.x = ballCentreX - w() / 2.0f;
    dst.y = ballCentreY;
    dst.w = w();
    dst.h = length;
    SDL_Point centre;
    centre.x = w() / 2.0f;
    centre.y = 0;
    SDL_RenderCopyEx(renderer, texture, &src, &dst, angle + 90, &centre, SDL_FLIP_NONE);
}
