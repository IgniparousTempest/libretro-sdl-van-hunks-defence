#include <iostream>
#include "cannon_ball.hpp"

CannonBall::CannonBall(Assets *assets, enum PlayerId playerId, float x, float y, float destX, float destY, float speed) :
        GameObject(assets->get("cannon_ball"), x, y), playerId(playerId), speed(speed) {
    this->destX = destX - w() / 2.0f;
    this->destY = destY + h() / 2.0f;
    distance = std::sqrt(std::pow(x - this->destX, 2) + std::pow(y - this->destY, 2));

    float x1 = this->destX;
    float x2 = this->x() - w() / 2.0f;
    float y1 = this->destY;
    float y2 = this->y() - h() / 2.0f;
    angle = atan2(y1 - y2, x1 - x2) * 180.0f / M_PI;

    trail = new CannonBallTrail(assets, this->x(), this->y());
}

CannonBall::~CannonBall() {
    delete trail;
}

void CannonBall::Update(double delta) {
    ShiftPosition(
            speed * cos(angle * M_PI / 180.0) * delta,
            speed * sin(angle * M_PI / 180.0) * delta
            );
    traveled += speed * delta;
    float half = distance / 2.0f;
    if (traveled < half) {
        scale = 1.0f + (traveled / half);
    } else {
        scale = 1.0f + (1.0f - (traveled - half) / half);
    }

    if (HasReachedDestination())
        Destroy();
}

void CannonBall::Render(SDL_Renderer* renderer) {
    trail->Render(renderer, centreX(), centreY(), traveled, angle);
    SDL_FRect destRect;
    destRect.w = w() * scale;
    destRect.h = h() * scale;
    destRect.x = x() + (w() - destRect.w) / 2;
    destRect.y = y() + (h() - destRect.h) / 2;
    SDL_RenderCopyF(renderer, texture, nullptr, &destRect);
}
