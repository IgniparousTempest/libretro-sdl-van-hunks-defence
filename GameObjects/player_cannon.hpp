#ifndef SUPER_MISSILE_COMMAND_PLAYER_CANNON_HPP
#define SUPER_MISSILE_COMMAND_PLAYER_CANNON_HPP


#include "../game_object.hpp"
#include "../assets.hpp"

class PlayerCannon : public GameObject {
public:
    PlayerCannon(Assets* assets, int shipX, int shipY);
    void recalculateAngle(float crosshairX, float crosshairY);
    void Render(SDL_Renderer* renderer) override;
    float getAngle() { return angle; }

private:
    float angle = 0;
};


#endif //SUPER_MISSILE_COMMAND_PLAYER_CANNON_HPP
