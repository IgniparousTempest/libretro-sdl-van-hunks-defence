#ifndef SUPER_MISSILE_COMMAND_CANNON_BALL_HPP
#define SUPER_MISSILE_COMMAND_CANNON_BALL_HPP


#include "../assets.hpp"
#include "../game_object.hpp"
#include "cannon_ball_trail.hpp"
#include "../PlayerId.hpp"

class CannonBall : public GameObject {
public:
    CannonBall(Assets *assets, enum PlayerId playerId, float x, float y, float destX, float destY, float speed);
    ~CannonBall();
    void Update(double delta) override;
    void Render(SDL_Renderer* renderer) override;
    FPoint Destination() { return {destX, destY}; }
    bool HasReachedDestination() const { return traveled > distance; }
    float Radius() { return w() / 2.0f; }
    enum PlayerId PlayerId() { return playerId; }
    void MakeExplosion(bool shouldMakeExplosion) { this->shouldMakeExplosion = shouldMakeExplosion; }
    bool ShouldMakeExplosion() { return shouldMakeExplosion; }

private:
    CannonBallTrail* trail;
    enum PlayerId playerId;
    bool shouldMakeExplosion = true;
    float destX;
    float destY;
    float speed;
    float angle;
    float scale = 1;
    float distance;
    float traveled = 0;
};


#endif //SUPER_MISSILE_COMMAND_CANNON_BALL_HPP
