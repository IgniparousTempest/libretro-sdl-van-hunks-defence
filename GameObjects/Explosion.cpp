#include "Explosion.hpp"

Explosion::Explosion(Assets *assets, enum PlayerId playerId, float x, float y) : playerId(playerId) {
    texture = assets->get("explosion");
    auto [ w, h ] = assets->dimensions("explosion");
    rect.x = x - h / 2.0f;
    rect.y = y - h / 2.0f;
    rect.w = h;
    rect.h = h;
    anim.animateOnce = true;
    anim.MaxFrames = 3;
    anim.SetFrameRate(400);
    assets->PlaySoundExplosion();
}

void Explosion::Update(double deltaTime) {
    anim.Update();
    if (anim.IsComplete())
        destroyed = true;
}

void Explosion::Render(SDL_Renderer *renderer) {
    SDL_Rect src;
    src.x = (int)h() * anim.GetCurrentFrame();
    src.y = 0;
    src.w = (int)h();
    src.h = (int)h();
    SDL_RenderCopyF(renderer, texture, &src, &rect);
}
