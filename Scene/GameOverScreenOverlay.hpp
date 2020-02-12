#ifndef SUPER_MISSILE_COMMAND_GAMEOVERSCREENOVERLAY_HPP
#define SUPER_MISSILE_COMMAND_GAMEOVERSCREENOVERLAY_HPP


#include "../assets.hpp"
#include "../Timer.hpp"
#include "../game_object.hpp"
#include "../GameObjects/Flag.hpp"

class Game;

class GameOverScreenOverlay {
public:
    GameOverScreenOverlay(Game* parent, Assets* assets, int screenWidth, int screenHeight);
    ~GameOverScreenOverlay();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

    bool IsComplete();
    std::vector<PlayerId> ReadyPlayers();

    void PressedStart(PlayerId id);

private:
    bool isComplete = false;
    Game* parent;
    Assets* assets;
    const int screenWidth;
    const int screenHeight;
    const float durationCountDown = 5499; // 5.5 seconds (so that the 5 second display lasts a second)
    Timer timerCountdown;
    std::map<PlayerId, bool> readyPlayers;
    Flag* flagVanHunks;
    Flag* flagVOC;

    void CallbackCountDownComplete();
};


#endif //SUPER_MISSILE_COMMAND_GAMEOVERSCREENOVERLAY_HPP
