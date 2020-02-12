#ifndef SUPER_MISSILE_COMMAND_CROSSHAIR_HPP
#define SUPER_MISSILE_COMMAND_CROSSHAIR_HPP


#include "../game_object.hpp"
#include "../assets.hpp"
#include "../PlayerId.hpp"

class Crosshair : public GameObject {
public:
    Crosshair(Assets* assets, enum PlayerId playerId, float x, float y, int xMax, int yMax);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

private:
    int xMax;
    int yMax;
    /// Number of pixels to move crosshairs a second.
    const float speed = 0.5;
};


#endif //SUPER_MISSILE_COMMAND_CROSSHAIR_HPP
