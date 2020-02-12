#include <iostream>
#include "cannon_ball.hpp"

CannonBall::CannonBall(Assets *assets, enum PlayerId playerId, float x, float y, float destX, float destY, float speed) : playerId(playerId), speed(speed) {
    texture = assets->get("cannon_ball");
    auto [w, h] = assets->dimensions("cannon_ball");
    rect.x = x - w / 2;
    rect.y = y - h / 2;
    rect.w = w;
    rect.h = h;
    this->destX = destX - rect.w / 2.0f;
    this->destY = destY + rect.h / 2.0f;
    distance = std::sqrt(std::pow(x - this->destX, 2) + std::pow(y - this->destY, 2));

    float x1 = this->destX;
    float x2 = rect.x - rect.w / 2.0f;
    float y1 = this->destY;
    float y2 = rect.y - rect.h / 2.0f;
    angle = atan2(y1 - y2, x1 - x2) * 180.0f / M_PI;

    trail = new CannonBallTrail(assets, rect.x, rect.y);
}

CannonBall::~CannonBall() {
    delete trail;
}

void CannonBall::Update(double delta) {
    rect.x += speed * cos(angle * M_PI / 180.0) * delta;
    rect.y += speed * sin(angle * M_PI / 180.0) * delta;
    traveled += speed * delta;
    float half = distance / 2.0f;
    if (traveled < half) {
        scale = 1.0f + (traveled / half);
    } else {
        scale = 1.0f + (1.0f - (traveled - half) / half);
    }

    if (HasReachedDestination())
        destroyed = true;
}

void CannonBall::Render(SDL_Renderer* renderer) {
    trail->Render(renderer, centreX(), centreY(), traveled, angle);
    SDL_FRect destRect;
    destRect.w = rect.w * scale;
    destRect.h = rect.h * scale;
    destRect.x = rect.x + (rect.w - destRect.w) / 2;
    destRect.y = rect.y + (rect.h - destRect.h) / 2;
    SDL_RenderCopyF(renderer, texture, nullptr, &destRect);
}
