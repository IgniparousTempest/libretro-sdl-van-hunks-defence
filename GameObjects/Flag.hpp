#ifndef SUPER_MISSILE_COMMAND_FLAG_HPP
#define SUPER_MISSILE_COMMAND_FLAG_HPP

#include <iostream>
#include "../game_object.hpp"
#include "../Animation.hpp"

class Flag : public GameObject {
public:
    Flag(SDL_Texture* texture, float x, float y, float angle, SDL_RendererFlip flip, int frames) : angle(angle), flip(flip), frames(frames) {
        this->texture = texture;

        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        rect.x = x - (w / frames) / 2.0f;
        rect.y = y - h / 2.0f;
        rect.w = w / frames;
        rect.h = h;

        src.x = 0;
        src.y = 0;
        src.w = w / frames;
        src.h = h;

        anim.MaxFrames = frames;
        anim.SetFrameRate(100);
    }

    void Update(double deltaTime) override {
        anim.Update();
    }

    void Render(SDL_Renderer* renderer) override {
        src.x = src.w * anim.GetCurrentFrame();
//        src.w -= 1;

        SDL_RenderCopyExF(renderer, texture, &src, &rect, angle, nullptr, flip);
//        src.w += 1;
    }

private:
    const int frames;
    float angle;
    SDL_RendererFlip flip;
    Animation anim;
    SDL_Rect src;
};

#endif //SUPER_MISSILE_COMMAND_FLAG_HPP
