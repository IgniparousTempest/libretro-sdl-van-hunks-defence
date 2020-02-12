#include "ScoreScreenOverlay.hpp"
#include "../Game.hpp"

ScoreScreenOverlay::ScoreScreenOverlay(Game* parent, Assets* assets, int screenWidth, int screenHeight) : parent(parent), assets(assets), screenWidth(screenWidth), screenHeight(screenHeight) {
    for (auto &playerId : PlayerIdExtensions::HumanPlayers())
        ammoScore[playerId] = 0;
    for (auto &playerId : PlayerIdExtensions::HumanPlayers())
        ammoSprites[playerId] = {};
    if (PlayersHaveAmmo()) {
        timer.Reset(cannonBallDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackDrawCannonBall, this));
    }
    else if (TreasureShipsAlive() > 0) {
        timer.Reset(treasureShipDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackDrawTreasureShip, this));
    }
    else {
        timer.Reset(endDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackEnd, this));
    }
}

ScoreScreenOverlay::~ScoreScreenOverlay() {
    for (auto &playerId : PlayerIdExtensions::HumanPlayers())
        for (auto &ball : ammoSprites[playerId])
            delete ball;
    for (auto &ship : shipSprites)
        delete ship;
}

void ScoreScreenOverlay::Update(float deltaTime) {
    timer.Tick(deltaTime);
}

void ScoreScreenOverlay::Render(SDL_Renderer *renderer) {
    auto text = "Bonus Points!";
    float width = FC_GetWidth(assets->Font(), text);
    FC_Draw(assets->Font(), renderer, screenWidth / 2.0f - width / 2.0f, screenHeight * 0.29f, text);

    float scoreX = screenWidth / 2.0f - screenWidth * 0.025f;
    float yOffset = 0;
    for (auto &playerId : PlayerIdExtensions::HumanPlayers()) {
        text = std::to_string(ammoScore[playerId]).c_str();
        width = FC_GetWidth(assets->Font(playerId), text);
        FC_Draw(assets->Font(playerId), renderer, scoreX - width, screenHeight * (0.43f + yOffset), text);
        yOffset += 0.03f;
    }

    text = std::to_string(shipScore).c_str();
    width = FC_GetWidth(assets->Font(), text);
    FC_Draw(assets->Font(), renderer, scoreX - width, screenHeight * 0.56f, text);

    // Draw cannon balls
    for (auto &playerId : PlayerIdExtensions::HumanPlayers())
        for (auto &ball : ammoSprites[playerId])
            ball->Render(renderer);

    // Draw ships
    for (auto &ship : shipSprites)
        ship->Render(renderer);
}

bool ScoreScreenOverlay::PlayersHaveAmmo() {
    bool isAmmo = false;
    auto players = parent->Players();
    for (auto &playerId : PlayerIdExtensions::HumanPlayers())
        if (players[playerId]->IsAlive() && players[playerId]->HasAmmo()) {
            isAmmo = true;
            break;
        }
    return isAmmo;
}

int ScoreScreenOverlay::TreasureShipsAlive() {
    int count = 0;
    auto treasureShips = parent->TreasureShips();
    for (auto &ship : treasureShips)
        if (!ship->isDestroyed())
            count++;
    return count;
}

void ScoreScreenOverlay::CallbackDrawCannonBall() {
    auto players = parent->Players();
    auto scores = parent->Scores();
    const float scoreX = screenWidth / 2.0f + screenWidth * 0.025f;
    const float xOffset = std::get<1>(assets->dimensions("cannon_ball")) * 1.2f;
    float yOffset = 0;
    for (auto &playerId : PlayerIdExtensions::HumanPlayers()) {
        yOffset += 0.03f;
        if (!players[playerId]->IsAlive())
            continue;
        if (players[playerId]->RemoveBall()) {
            ammoSprites[playerId].push_back(new GameObject(assets->get("cannon_ball"), scoreX + xOffset * ammoSprites[playerId].size(), screenHeight * (0.43f + yOffset)));
            ammoScore[playerId] += 5 * parent->PointScaleFactor();
            scores[playerId]->Add(5 * parent->PointScaleFactor());
        }
    }

    if (PlayersHaveAmmo()) {
        timer.Reset(cannonBallDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackDrawCannonBall, this));
    }
    else {
        timer.Reset(treasureShipDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackDrawTreasureShip, this));
    }
}

void ScoreScreenOverlay::CallbackDrawTreasureShip() {
    auto players = parent->Players();
    auto scores = parent->Scores();
    int ships = TreasureShipsAlive();
    const float scoreX = screenWidth / 2.0f + screenWidth * 0.025f;
    auto [w, h] = assets->dimensions("ship0");
    const float scaledWidth = w * 0.5f;
    const float xOffset = scaledWidth * 1.2f;
    const float scaledHalfHeight = (h * 0.5f) / 2.0f;
    shipSprites.push_back(new GameObject(assets->get("ship0"), scoreX + xOffset * shipSprites.size(), screenHeight * 0.57f + scaledHalfHeight, 0, scaledHalfHeight, scaledWidth));
    shipScore += 50 * parent->PointScaleFactor();
    for (auto &playerId : PlayerIdExtensions::HumanPlayers())
        if (players[playerId]->IsAlive())
            scores[playerId]->Add(50 * parent->PointScaleFactor());
    if (shipSprites.size() < ships) {
        timer.Reset(treasureShipDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackDrawTreasureShip, this));
    }
    else {
        timer.Reset(endDelay);
        timer.SetCallback(std::bind(&ScoreScreenOverlay::CallbackEnd, this));
    }
}

bool ScoreScreenOverlay::IsComplete() {
    return isComplete;
}

void ScoreScreenOverlay::CallbackEnd() {
    isComplete = true;
}
