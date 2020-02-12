#ifndef SUPER_MISSILE_COMMAND_BURNINGPLAYERSHIP_HPP
#define SUPER_MISSILE_COMMAND_BURNINGPLAYERSHIP_HPP

#include "BurningShip.hpp"
#include "../assets.hpp"

class BurningPlayerShip : public BurningShip {
public:
    BurningPlayerShip(Assets* assets, float x, float y) : BurningShip(assets->get("ship_destroyed_player"), x, y) {};
};

#endif //SUPER_MISSILE_COMMAND_BURNINGPLAYERSHIP_HPP
