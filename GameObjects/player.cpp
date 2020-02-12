#include "player.hpp"
#include "../Auxiliary.hpp"
#include "cannon_ball.hpp"
#include <iostream>

Player::Player(Assets* assets, enum PlayerId playerId, int x, int bottomY, int screenWidth, int screenHeight) : assets(assets), playerId(playerId) {
    // Boat
    std::string textureName = "ship" + std::to_string(PlayerIdExtensions::Value(playerId));
    texture = assets->get(textureName);
    auto dimensions = assets->dimensions(textureName); //TODO: C++17: auto [ width, height ] =
    auto width = std::get<0>(dimensions);
    auto height = std::get<1>(dimensions);
    dimensions = assets->dimensions("cannon_ball"); //TODO: C++17: auto [ ballWidth, ballHeight ] =
    auto ballHeight = std::get<1>(dimensions);
    rect.x = x - width / 2.0f;
    rect.y = bottomY - height - ballHeight;
    rect.w = width;
    rect.h = height;

    // Cannon
    cannon = new PlayerCannon(assets, rect.x, rect.y);

    // Crosshair
    crosshair = new Crosshair(assets, playerId, x, rect.y - rect.h * 2, screenWidth, rect.y);

    recalculateCannonAngle();
}

Player::~Player() {
    delete cannon;
    delete crosshair;
}

void Player::Render(SDL_Renderer *renderer) {
    if (!alive)
        return;

    GameObject::Render(renderer);
}

void Player::RenderTopLayers(SDL_Renderer* renderer) {
    if (!alive)
        return;

    // Cannon
    cannon->Render(renderer);

    // Crosshair
    crosshair->Render(renderer);

    // UI
    for (auto &ball : ammoUi)
        ball->Render(renderer);
}

void Player::recalculateCannonAngle() {
    cannon->recalculateAngle(crosshair->x() + crosshair->w() / 2.0f, crosshair->y() + crosshair->w() / 2.0f);
}

void Player::moveLeft(float deltaTime) {
    if (!alive)
        return;
    crosshair->moveLeft(deltaTime);
    recalculateCannonAngle();
}

void Player::moveRight(float deltaTime) {
    if (!alive)
        return;
    crosshair->moveRight(deltaTime);
    recalculateCannonAngle();
}

void Player::moveUp(float deltaTime) {
    if (!alive)
        return;
    crosshair->moveUp(deltaTime);
    recalculateCannonAngle();
}

void Player::moveDown(float deltaTime) {
    if (!alive)
        return;
    crosshair->moveDown(deltaTime);
    recalculateCannonAngle();
}

void Player::RestockAmmo(int ammo) {
    if (!alive)
        return;
    this->ammo = ammo;
    for (auto &ball : ammoUi)
        delete ball;
    ammoUi = {};
    auto dimensions = assets->dimensions("cannon_ball"); //TODO: C++17: auto [ width, height ] =
    auto width = std::get<0>(dimensions);
    for (int i = 0; i < ammo; ++i) {
        ammoUi.push_back(new GameObject(assets->get("cannon_ball"), x() + width * i, y() + h(), 0, 0));
    }
}

void Player::Hit() {
    alive = false;
}

bool Player::RemoveBall() {
    if (!alive || ammo-- <= 0)
        return false;
    delete ammoUi.back();
    ammoUi.pop_back();
    return true;
}

CannonBall* Player::fire() {
    if (!RemoveBall())
        return nullptr;
    assets->PlaySoundCannonFire();
    auto ball = new CannonBall(assets, playerId, cannon->centreX(), cannon->centreY(), crosshair->centreX(), crosshair->centreY(), 0.3);
    return ball;
}

bool Player::HasAmmo() {
    return ammo > 0;
}
