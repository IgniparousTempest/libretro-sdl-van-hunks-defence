#ifndef SUPER_MISSILE_COMMAND_GAME_OBJECT_HPP
#define SUPER_MISSILE_COMMAND_GAME_OBJECT_HPP

#include <SDL_render.h>
#include "geometry.hpp"

class GameObject {
public:
    GameObject() {}
    GameObject(SDL_Texture* texture, float x, float y) : texture(texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SetPosition(x - w / 2.0f, y - h / 2.0f);
        SetDimensions(w, h);
    }
    GameObject(SDL_Texture* texture, float x, float y, float centreX, float centreY) : texture(texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SetPosition(x - centreX, y - centreY);
        SetDimensions(w, h);
    }
    GameObject(SDL_Texture* texture, float x, float y, float centreX, float centreY, float width) : texture(texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SetPosition(x - centreX, y - centreY);
        SetDimensions(width, h * width / w);
    }
    virtual void Update(double delta) {};

    virtual void Render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    };

    float centreX() { return rect.x + rect.w / 2.0f; }
    float centreY() { return rect.y + rect.h / 2.0f; }

    float x() { return position.x; }
    float y() { return position.y; }
    int w() { return rect.w; }
    int h() { return rect.h; }

    SDL_Rect Rect() { return rect; }

    virtual void Destroy() { destroyed = true; }
    virtual bool isDestroyed() { return destroyed; }

protected:
    SDL_Texture* texture;

    void SetPosition(float x, float y) {
        position = {.x = x, .y = y};
        rect.x = std::roundf(x);
        rect.y = std::roundf(y);
    }

    void SetX(float x) {
        SetPosition(x, position.y);
    }

    void SetY(float y) {
        SetPosition(position.x, y);
    }

    void ShiftPosition(float x, float y) {
        SetPosition(position.x + x, position.y + y);
    }

    void ShiftX(float x) {
        SetPosition(position.x + x, position.y);
    }

    void ShiftY(float y) {
        SetPosition(position.x, position.y + y);
    }

    void SetDimensions(int w, int h) {
        rect.w = w;
        rect.h = h;
    }

private:
    FPoint position;
    SDL_Rect rect;
    bool destroyed = false;
};

#endif //SUPER_MISSILE_COMMAND_GAME_OBJECT_HPP
