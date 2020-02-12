#include <iostream>
#include <algorithm>
#include "Game.hpp"
#include "SDL_extensions.h"
#include "GameObjects/BurningTreasureShip.hpp"
#include "GameObjects/BurningPlayerShip.hpp"

const char* Game::NAME = "Van Hunks Defence";
const char* Game::VERSION = "1.0.0";

Game::Game(Assets* assets, int screenWidth, int screenHeight) : assets(assets), screenWidth(screenWidth), screenHeight(screenHeight), dt(1.0 / TARGET_FPS) {
    std::cout << "Created Game Object." << std::endl;
    gameOverScreenOverlay = new GameOverScreenOverlay(this, assets, screenWidth, screenHeight);
    background = new Background(assets, screenWidth, screenHeight);
}

void Game::StartNewGame(std::vector<PlayerId> readyPlayers) {
    std::cout << "Starting a new game." << std::endl;

    // Reset burnt out ships
    for (auto &ship : burningShips)
        delete ship;
    burningShips = {};

    level = 0;
    SpawnShips(readyPlayers);

    // Reset Scores
    for (auto &[key, score] : scores)
        delete score;
    scores = {};
    for(auto const& playerId : PlayerIdExtensions::HumanPlayers())
        scores.emplace(playerId, new Score(assets, players[playerId]->centreX(), 0));

    StartNextLevel();
}

void Game::StartNextLevel() {
    level++;
    std::cout << "Starting level " << level << "." << std::endl;

    // Remove remnants of previous level
    for (auto &ball : cannonBalls)
        delete ball;
    cannonBalls = {};
    for (auto &exp : explosions)
        delete exp;
    explosions = {};
    
    delete background;
    background = new Background(assets, screenWidth, screenHeight);

    // Calculate ammo for the round
    int livingPlayers = PlayersAlive();
    for(auto const& player : PlayerIdExtensions::HumanPlayers())
        players[player]->RestockAmmo(12 / livingPlayers);

    // Get targets for enemy balls
    std::vector<GameObject*> targets;
    for(auto const& player : PlayerIdExtensions::HumanPlayers())
        if (players[player]->IsAlive())
            targets.push_back(players[player]);
    for (auto &ship : treasureShips)
        targets.push_back(ship);

    delete enemyCannonBallFactory;
    enemyCannonBallFactory = new EnemyCannonBallFactory(assets, 0, screenWidth, targets, 0.05f + 0.01f * (level - 1));
    enemyCannonBallFactory->LoadCannons(10, 5);
}

void Game::Update(double frameTime) {
    if (state == GameState::normal) {
        // Pre Update
        enemyCannonBallFactory->Update(frameTime, &cannonBalls);
        for (auto &ship : burningShips)
            ship->Update(frameTime);

        // Sub-Frame Updates
        while (frameTime >= DBL_EPSILON) {
            double deltaTime = std::min(frameTime, dt);

            for (auto &obj : cannonBalls)
                obj->Update(deltaTime);
            for (auto &obj : explosions)
                obj->Update(deltaTime);

            CheckCollisions();

            frameTime -= deltaTime;
        }

        // Post Update

        // Cannon balls that hit a ship
        for (auto &obj : cannonBalls)
            if (obj->isDestroyed() && obj->PlayerId() == PlayerId::computer && obj->HasReachedDestination()) {
                auto dest = obj->Destination();
                for (auto &playerId : PlayerIdExtensions::HumanPlayers()) {
                    auto player = players[playerId];
                    auto rect = player->Rect();
                    if (SDL_FPointInFRect(&dest, &rect)) {
                        std::cout << "Enemy cannon ball hit player " << PlayerIdExtensions::Value(playerId) << "."
                                  << std::endl;
                        BurnPlayerShip(player);
                        break;
                    }
                }
                for (auto &ship : treasureShips) {
                    auto rect = ship->Rect();
                    if (SDL_FPointInFRect(&dest, &rect)) {
                        std::cout << "Enemy cannon ball hit a treasure ship." << std::endl;
                        burningShips.push_back(new BurningTreasureShip(assets, ship->centreX(), ship->centreY()));
                        ship->Destroy();
                        break;
                    }
                }
            }
        DeleteDestroyedGameObjects(&treasureShips);
        DeleteDestroyedGameObjects((std::vector<GameObject *> *) &explosions);
        DeleteDestroyedCannonBalls();

        // Is game over
        if (IsLevelOver()) {
            state = GameState::scoring;
            scoreScreenOverlay = new ScoreScreenOverlay(this, assets, screenWidth, screenHeight);
        }
    }
    else if (state == GameState::scoring) {
        scoreScreenOverlay->Update(frameTime);
        if (scoreScreenOverlay->IsComplete()) {
            delete scoreScreenOverlay;
            if (PlayersAlive() > 0) {
                state = GameState::normal;
                StartNextLevel();
            }
            else {
                state = GameState::over;
                gameOverScreenOverlay = new GameOverScreenOverlay(this, assets, screenWidth, screenHeight);
            }
        }
    }
    else if (state == GameState::over) {
        gameOverScreenOverlay->Update(frameTime);
        if (gameOverScreenOverlay->IsComplete()) {
            state = GameState::normal;
            StartNewGame(gameOverScreenOverlay->ReadyPlayers());
            delete gameOverScreenOverlay;
        }
    }
}

void Game::Render(SDL_Renderer *renderer) {
    background->Render(renderer);
    for(auto const& [id, player] : players)
        player->Render(renderer);
    for (auto &obj : treasureShips)
        obj->Render(renderer);
    for (auto &obj : burningShips)
        obj->Render(renderer);
    for (auto &obj : cannonBalls)
        obj->Render(renderer);
    for (auto &obj : explosions)
        obj->Render(renderer);
    for(auto const& [id, player] : players)
        player->RenderTopLayers(renderer);
    for(auto const& [id, score] : scores)
        score->Render(renderer);

    if (state == GameState::scoring)
        scoreScreenOverlay->Render(renderer);

    if (state == GameState::over)
        gameOverScreenOverlay->Render(renderer);
}

void Game::SpawnShips(std::vector<enum PlayerId> readyPlayers) {
    // Clean up previous game;
    for (auto &[key, ship] : players)
        delete ship;
    for (auto &ship : treasureShips)
        delete ship;
    players = {};
    treasureShips = {};

    // Set parameters
    auto [ w, h ] = assets->dimensions("ship1");
    auto offset = w / 2;
    auto x1 = w / 2 + offset;
    auto x4 = screenWidth - w / 2 - offset;
    auto x2 = x1 + (x4 - x1) / 3;
    auto x3 = x1 + 2 * (x4 - x1) / 3;

    std::sort(readyPlayers.begin(), readyPlayers.end());
    auto otherPlayers = PlayerIdExtensions::InvertHumans(readyPlayers);
    std::vector<enum PlayerId> playerOrder;
    playerOrder.reserve( readyPlayers.size() + otherPlayers.size() );
    playerOrder.insert( playerOrder.end(), readyPlayers.begin(), readyPlayers.end() );
    playerOrder.insert( playerOrder.end(), otherPlayers.begin(), otherPlayers.end() );

    // Place players
    int numberOfPlayers = readyPlayers.size();
    if (numberOfPlayers <= 2) {
        players.emplace(playerOrder[2], new Player(assets, playerOrder[2], x1, screenHeight, screenWidth, screenHeight));
        players.emplace(playerOrder[0], new Player(assets, playerOrder[0], x2, screenHeight, screenWidth, screenHeight));
        players.emplace(playerOrder[1], new Player(assets, playerOrder[1], x3, screenHeight, screenWidth, screenHeight));
        players.emplace(playerOrder[3], new Player(assets, playerOrder[3], x4, screenHeight, screenWidth, screenHeight));
    }
    else {
        players.emplace(playerOrder[0], new Player(assets, playerOrder[0], x1, screenHeight, screenWidth, screenHeight));
        players.emplace(playerOrder[1], new Player(assets, playerOrder[1], x2, screenHeight, screenWidth, screenHeight));
        players.emplace(playerOrder[2], new Player(assets, playerOrder[2], x3, screenHeight, screenWidth, screenHeight));
        players.emplace(playerOrder[3], new Player(assets, playerOrder[3], x4, screenHeight, screenWidth, screenHeight));
    }
    // Burn ships not in the game
    for (auto &playerId : otherPlayers)
        BurnPlayerShip(players[playerId]);

    // Place treasure ships
    auto y = players[playerOrder[0]]->centreY();
    auto xT0 = x1 + (x2 - x1) / 2.0f;
    auto xT1 = x2 + (x3 - x2) / 2.0f;
    auto xT2 = x3 + (x4 - x3) / 2.0f;
    treasureShips.push_back(new GameObject(assets->get("ship0"), xT0, y));
    treasureShips.push_back(new GameObject(assets->get("ship0"), xT1, y));
    treasureShips.push_back(new GameObject(assets->get("ship0"), xT2, y));
}

void Game::BurnPlayerShip(Player *player) {
    burningShips.push_back(new BurningPlayerShip(assets, player->centreX(), player->centreY()));
    player->Hit();
}

void Game::CheckCollisions() {
    for (auto &explosion : explosions) {
        if (explosion->PlayerId() == PlayerId::computer)
            continue;
        for (auto &ball : cannonBalls) {
            if (ball->PlayerId() != PlayerId::computer || ball->isDestroyed())
                continue;
            float dist = std::sqrt(std::pow(explosion->centreX() - ball->centreX(), 2) + std::pow(explosion->centreY() - ball->centreY(), 2));
            if (dist < ball->Radius() + explosion->Radius()) {
                std::cout << "Player " << PlayerIdExtensions::Value(explosion->PlayerId()) << " destroyed enemy cannon ball" << std::endl;
                ball->Destroy();
                ball->MakeExplosion(false);
                scores[explosion->PlayerId()]->Add(25 * PointScaleFactor());
            }
        }
    }
}

void Game::DeleteDestroyedGameObjects(std::vector<GameObject *> *objects) {
    auto end = std::stable_partition(
            objects->begin(),
            objects->end(),
            [](GameObject* p) { return !p->isDestroyed(); }
    );
    for (auto i = end; i < objects->end(); i++) {
        delete *i;
    }
    objects->erase(end, objects->end());
}

void Game::DeleteDestroyedCannonBalls() {
    auto end = std::stable_partition(
            cannonBalls.begin(),
            cannonBalls.end(),
            [](GameObject* p) { return !p->isDestroyed(); }
    );
    for (auto i = end; i < cannonBalls.end(); i++) {
        if ((*i)->ShouldMakeExplosion())
            explosions.push_back(new Explosion(assets, (*i)->PlayerId(), (*i)->x(), (*i)->y()));
        delete *i;
    }
    cannonBalls.erase(end, cannonBalls.end());
}

void Game::PressLeft(enum PlayerId playerId, float deltaTime) {
    players[playerId]->moveLeft(deltaTime);
}

void Game::PressRight(enum PlayerId playerId, float deltaTime) {
    players[playerId]->moveRight(deltaTime);
}

void Game::PressUp(enum PlayerId playerId, float deltaTime) {
    players[playerId]->moveUp(deltaTime);
}

void Game::PressDown(enum PlayerId playerId, float deltaTime) {
    players[playerId]->moveDown(deltaTime);
}

void Game::PressFire(enum PlayerId playerId) {
    if (state != GameState::normal)
        return;
    auto ball = players[playerId]->fire();
    if (ball != nullptr) {
        cannonBalls.push_back(ball);
        std::cout << "Player " << PlayerIdExtensions::Value(playerId) << " fired a shot." << std::endl;
    }
}

void Game::PressStart(enum PlayerId playerId) {
    if (state == GameState::over) {
        gameOverScreenOverlay->PressedStart(playerId);
        std::cout << "Player " << PlayerIdExtensions::Value(playerId) << " pressed start." << std::endl;
    }
}

bool Game::IsLevelOver() {
    int enemyCannonBallsCount = 0;
    for (auto &ball : cannonBalls)
        if (ball->PlayerId() == PlayerId::computer)
            enemyCannonBallsCount++;
    return enemyCannonBallFactory->AllFired() && explosions.empty() && enemyCannonBallsCount == 0;
}

int Game::PointScaleFactor() {
    return std::clamp((level - 1) / 2 + 1, 0, 6);
}

std::map<enum PlayerId, Score *> Game::Scores() { return scores; }

std::vector<GameObject *> Game::TreasureShips() { return treasureShips; }

std::map<enum PlayerId, Player *> Game::Players() { return players; }

int Game::PlayersAlive() {
    int livingPlayers = 0;
    for(auto const& player : PlayerIdExtensions::HumanPlayers())
        if (players[player]->IsAlive())
            livingPlayers++;
    return livingPlayers;
}
