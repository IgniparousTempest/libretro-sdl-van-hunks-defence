#ifndef SUPER_MISSILE_COMMAND_PLAYER_HPP
#define SUPER_MISSILE_COMMAND_PLAYER_HPP


#include <vector>
#include "../game_object.hpp"
#include "../assets.hpp"
#include "player_cannon.hpp"
#include "cannon_ball.hpp"
#include "Crosshair.hpp"

class Player : public GameObject {
public:
    Player(Assets* assets, enum PlayerId playerId, int x, int y, int screenWidth, int screenHeight);
    ~Player();
    void Render(SDL_Renderer* renderer) override;
    void RenderTopLayers(SDL_Renderer* renderer);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void RestockAmmo(int ammo);
    /// Called when the ship is hit by an enemy cannon ball
    void Hit();
    bool IsAlive() { return alive; }
    /// Removes a ball from the player's inventory and updates the UI.
    bool RemoveBall();
    CannonBall* fire();
    bool HasAmmo();

private:
    enum PlayerId playerId;
    bool alive = true;
    int ammo = 0;
    Assets* assets;

    Crosshair* crosshair;
    PlayerCannon* cannon;
    std::vector<GameObject*> ammoUi = {};

    void recalculateCannonAngle();
};


#endif //SUPER_MISSILE_COMMAND_PLAYER_HPP
