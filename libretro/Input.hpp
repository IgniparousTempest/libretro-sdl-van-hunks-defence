#ifndef LIBRETRO_VAN_HUNKS_DEFENCE_INPUT_HPP
#define LIBRETRO_VAN_HUNKS_DEFENCE_INPUT_HPP


struct Input {
    bool fire;
    bool back;
    bool right;
    bool down;
    bool left;
    bool up;
    bool start;

    bool fire_pressed;
    bool back_pressed;
    bool right_pressed;
    bool down_pressed;
    bool left_pressed;
    bool up_pressed;
    bool start_pressed;

    bool fire_released;
    bool back_released;
    bool right_released;
    bool down_released;
    bool left_released;
    bool up_released;
    bool start_released;
};


#endif //LIBRETRO_VAN_HUNKS_DEFENCE_INPUT_HPP
