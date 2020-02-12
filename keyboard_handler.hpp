#ifndef SUPER_MISSILE_COMMAND_KEYBOARD_HANDLER_HPP
#define SUPER_MISSILE_COMMAND_KEYBOARD_HANDLER_HPP

#include <SDL_events.h>
#include <map>
#include <iostream>

class KeyboardHandler {
public:
    void handle(SDL_Event e) {
        bool value;
        if (e.type == SDL_KEYDOWN)
            value = true;
        else if (e.type == SDL_KEYUP)
            value = false;
        else
            return;
        initialise(e.key.keysym.sym);
        initialisePressed(e.key.keysym.sym);
        // Compare to previous state
        statePressed[e.key.keysym.sym] = value && !state[e.key.keysym.sym];
        state[e.key.keysym.sym] = value;
    }

    bool triggerPressed(SDL_Keycode key) {
        initialisePressed(key);
        auto value = statePressed[key];
        if (value)
            statePressed[key] = false;
        return value;
    }

    bool isDown(SDL_Keycode key) {
        initialise(key);
        return state[key];
    }

    bool isUp(SDL_Keycode key) {
        initialise(key);
        return !state[key];
    }

private:
    std::map<SDL_Keycode, bool> state = {};
    std::map<SDL_Keycode, bool> statePressed = {};

    void initialise(SDL_Keycode key) {
        auto it = state.find(key);
        if (it == state.end())
            state[key] = false;
    }

    void initialisePressed(SDL_Keycode key) {
        auto it = statePressed.find(key);
        if (it == statePressed.end())
            statePressed[key] = false;
    }
};


#endif //SUPER_MISSILE_COMMAND_KEYBOARD_HANDLER_HPP
