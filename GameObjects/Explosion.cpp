#include "Explosion.hpp"

Explosion::Explosion(Assets *assets, enum PlayerId playerId, float x, float y) : playerId(playerId) {
    texture = assets->get("explosion");
    auto dimensions = assets->dimensions("explosion"); //TODO: C++17: auto [ w, h ] =
    auto h = std::get<1>(dimensions);
    SetPosition(x - h / 2.0f, y - h / 2.0f);
    SetDimensions(h, h);
    anim.animateOnce = true;
    anim.MaxFrames = 3;
    anim.SetFrameRate(400);
    assets->PlaySoundExplosion();
}

void Explosion::Update(double deltaTime) {
    anim.Update();
    if (anim.IsComplete())
        Destroy();
}

void Explosion::Render(SDL_Renderer *renderer) {
    SDL_Rect src;
    src.x = (int)h() * anim.GetCurrentFrame();
    src.y = 0;
    src.w = (int)h();
    src.h = (int)h();
    auto dst = Rect();
    SDL_RenderCopy(renderer, texture, &src, &dst);
}
