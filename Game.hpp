#ifndef SUPER_MISSILE_COMMAND_GAME_HPP
#define SUPER_MISSILE_COMMAND_GAME_HPP


#include <vector>
#include "assets.hpp"
#include "GameObjects/BurningShip.hpp"
#include "game_object.hpp"
#include "GameObjects/Score.hpp"
#include "GameObjects/player.hpp"
#include "GameObjects/Explosion.hpp"
#include "GameObjects/background.hpp"
#include "GameObjects/EnemyCannonBallFactory.hpp"
#include "Scene/ScoreScreenOverlay.hpp"
#include "Scene/GameOverScreenOverlay.hpp"

enum class GameState {normal, scoring, over};

class Game {
public:
    static const int TARGET_FPS = 60;
    static const char* NAME;
    static const char* VERSION;

    Game(Assets* assets, int screenWidth, int screenHeight);
    void StartNewGame(std::vector<PlayerId> readyPlayers);
    void StartNextLevel();
    void Update(double frameTime);
    void Render(SDL_Renderer* renderer);
    bool IsLevelOver();
    /// Gets the factor to scale the points by.
    int PointScaleFactor();
    int PlayersAlive();

    void PressLeft(enum PlayerId playerId, float deltaTime);
    void PressRight(enum PlayerId playerId, float deltaTime);
    void PressUp(enum PlayerId playerId, float deltaTime);
    void PressDown(enum PlayerId playerId, float deltaTime);
    void PressFire(enum PlayerId playerId);
    void PressStart(enum PlayerId playerId);

    std::map<enum PlayerId, Score*> Scores();
    std::vector<GameObject*> TreasureShips();
    std::map<enum PlayerId, Player*> Players();

private:
    enum GameState state = GameState::over;
    int level = 0;
    Assets* assets;
    const int screenWidth;
    const int screenHeight;
    const double dt;

    GameOverScreenOverlay* gameOverScreenOverlay = nullptr;
    ScoreScreenOverlay* scoreScreenOverlay = nullptr;
    Background* background = nullptr;
    EnemyCannonBallFactory* enemyCannonBallFactory = nullptr;
    std::map<enum PlayerId, Score*> scores;
    std::vector<BurningShip*> burningShips;
    std::vector<GameObject*> treasureShips;
    std::map<enum PlayerId, Player*> players;
    std::vector<CannonBall*> cannonBalls;
    std::vector<Explosion*> explosions;

    void SpawnShips(std::vector<enum PlayerId> readyPlayers);
    void BurnPlayerShip(Player* player);
    /// Checks if an enemy ball hit a player explosion
    void CheckCollisions();
    static void DeleteDestroyedGameObjects(std::vector<GameObject*>* objects);
    void DeleteDestroyedCannonBalls();
};


#endif //SUPER_MISSILE_COMMAND_GAME_HPP
