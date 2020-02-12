#ifndef SUPER_MISSILE_COMMAND_SCORESCREENOVERLAY_HPP
#define SUPER_MISSILE_COMMAND_SCORESCREENOVERLAY_HPP


#include <SDL_rect.h>
#include <SDL_render.h>
#include <iostream>
#include "../assets.hpp"
#include "../Timer.hpp"
#include "../game_object.hpp"

class Game;

class ScoreScreenOverlay {
public:
    ScoreScreenOverlay(Game* parent, Assets* assets, int screenWidth, int screenHeight);
    ~ScoreScreenOverlay();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    bool IsComplete();

private:
    Game* parent;
    Assets* assets;
    const int screenWidth;
    const int screenHeight;
    std::map<enum PlayerId, int> ammoScore;
    std::map<enum PlayerId, std::vector<GameObject*>> ammoSprites;
    int shipScore = 0;
    std::vector<GameObject*> shipSprites;
    bool isComplete = false;
    Timer timer;
    const float cannonBallDelay = 200;
    const float treasureShipDelay = 300;
    const float endDelay = 1000;

    bool PlayersHaveAmmo();
    int TreasureShipsAlive();

    void CallbackDrawCannonBall();
    void CallbackDrawTreasureShip();
    void CallbackEnd();
};


#endif //SUPER_MISSILE_COMMAND_SCORESCREENOVERLAY_HPP
