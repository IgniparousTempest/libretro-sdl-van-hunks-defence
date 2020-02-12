#ifndef SUPER_MISSILE_COMMAND_GAME_OBJECT_HPP
#define SUPER_MISSILE_COMMAND_GAME_OBJECT_HPP

#include <SDL_render.h>

class GameObject {
public:
    GameObject() {}
    GameObject(SDL_Texture* texture, float x, float y) : texture(texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        rect.x = x - w / 2.0;
        rect.y = y - h / 2.0;
        rect.w = w;
        rect.h = h;
    }
    GameObject(SDL_Texture* texture, float x, float y, float centreX, float centreY) : texture(texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        rect.x = x - centreX;
        rect.y = y - centreY;
        rect.w = w;
        rect.h = h;
    }
    GameObject(SDL_Texture* texture, float x, float y, float centreX, float centreY, float width) : texture(texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        rect.x = x - centreX;
        rect.y = y - centreY;
        rect.w = width;
        rect.h = h * width / w;
    }
    virtual void Update(double delta) {};

    virtual void Render(SDL_Renderer* renderer) {
        SDL_RenderCopyF(renderer, texture, nullptr, &rect);
    };

    float centreX() { return rect.x + rect.w / 2.0f; }
    float centreY() { return rect.y + rect.h / 2.0f; }

    float x() { return rect.x; }
    float y() { return rect.y; }
    float w() { return rect.w; }
    float h() { return rect.h; }

    SDL_FRect Rect() { return rect; }

    void Destroy() { destroyed = true; }
    virtual bool isDestroyed() { return destroyed; }

protected:
    SDL_Texture* texture;
    SDL_FRect rect;
    bool destroyed = false;
};

#endif //SUPER_MISSILE_COMMAND_GAME_OBJECT_HPP
