#ifndef SUPER_MISSILE_COMMAND_BURNINGTREASURESHIP_HPP
#define SUPER_MISSILE_COMMAND_BURNINGTREASURESHIP_HPP

#include "BurningShip.hpp"
#include "../assets.hpp"

class BurningTreasureShip : public BurningShip {
public:
    BurningTreasureShip(Assets* assets, float x, float y) : BurningShip(assets->get("ship_destroyed_treasure"), x, y) {};
};

#endif //SUPER_MISSILE_COMMAND_BURNINGTREASURESHIP_HPP
