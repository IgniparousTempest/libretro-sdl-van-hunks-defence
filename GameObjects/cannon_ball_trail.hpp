#ifndef SUPER_MISSILE_COMMAND_CANNON_BALL_TRAIL_HPP
#define SUPER_MISSILE_COMMAND_CANNON_BALL_TRAIL_HPP


#include "../game_object.hpp"
#include "../assets.hpp"

class CannonBallTrail : public GameObject {
public:
    CannonBallTrail(Assets *assets, float x, float y);
    void Render(SDL_Renderer* renderer, float ballCentreX, float ballCentreY, float distance, float angle);
};


#endif //SUPER_MISSILE_COMMAND_CANNON_BALL_TRAIL_HPP
