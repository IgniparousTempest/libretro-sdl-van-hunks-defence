#include "GameOverScreenOverlay.hpp"
#include "../Game.hpp"

GameOverScreenOverlay::GameOverScreenOverlay(Game *parent, Assets *assets, int screenWidth, int screenHeight) : parent(parent), assets(assets), screenWidth(screenWidth), screenHeight(screenHeight) {
    flagVanHunks = new Flag(assets->get("flag_van_hunks_fluttering"), 136, 214, -15, SDL_FLIP_HORIZONTAL, 16);
    flagVOC = new Flag(assets->get("flag_voc_fluttering"), 811, 214, 15, SDL_FLIP_NONE, 16);
    timerCountdown.SetCallback(std::bind(&GameOverScreenOverlay::CallbackCountDownComplete, this));
}

GameOverScreenOverlay::~GameOverScreenOverlay() {
    delete flagVanHunks;
    delete flagVOC;
}

void GameOverScreenOverlay::Update(float deltaTime) {
    flagVanHunks->Update(deltaTime);
    flagVOC->Update(deltaTime);
    timerCountdown.Tick(deltaTime);
}

void GameOverScreenOverlay::Render(SDL_Renderer *renderer) {
    auto text = "Van Hunks Defence";
    float width = FC_GetWidth(assets->FontLarge(), text);
    FC_Draw(assets->FontLarge(), renderer, screenWidth / 2.0f - width / 2.0f, screenHeight * 0.29f, text);

    if (timerCountdown.IsActive()) {
        int time = (int) std::round(timerCountdown.RemainingSeconds());
        text = std::to_string(time).c_str();
        width = FC_GetWidth(assets->FontLarge(), text);
        float height = FC_GetHeight(assets->FontLarge(), text);
        FC_Draw(assets->FontLarge(), renderer, screenWidth / 2.0f - width / 2.0f, screenHeight / 2.0f - height / 2.0f, text);
    }

    if (!readyPlayers.empty()) {
        if (readyPlayers.size() == 1)
            text = "1 Player ready";
        else
            text = (std::to_string(readyPlayers.size()) + " Players ready").c_str();
        width = FC_GetWidth(assets->Font(), text);
        FC_Draw(assets->Font(), renderer, screenWidth / 2.0f - width / 2.0f, screenHeight * 0.6f, text);
    }

    text = "Press Start to Begin";
    width = FC_GetWidth(assets->FontLarge(), text);
    FC_Draw(assets->FontLarge(), renderer, screenWidth / 2.0f - width / 2.0f, screenHeight * 0.7f, text);

    text = "Game by Courtney Pitcher. 2020. bit.ly/386L9M9";
    width = FC_GetWidth(assets->FontCredits(), text);
    float height = FC_GetHeight(assets->FontCredits(), text);
    FC_Draw(assets->FontCredits(), renderer, screenWidth - width, screenHeight - height, text);

    flagVanHunks->Render(renderer);
    flagVOC->Render(renderer);
}

bool GameOverScreenOverlay::IsComplete() {
    return isComplete;
}

std::vector<PlayerId> GameOverScreenOverlay::ReadyPlayers() {
    std::vector<PlayerId> players;
    //TODO: c++17 s/auto const &entry/auto const &[,]/
    for  (auto const &entry : readyPlayers)
        if (entry.second)
            players.push_back(entry.first);
    return players;
}

void GameOverScreenOverlay::PressedStart(PlayerId id) {
    if (readyPlayers[id])
        return;
    timerCountdown.Reset(durationCountDown);
    readyPlayers[id] = true;
}

void GameOverScreenOverlay::CallbackCountDownComplete() {
    isComplete = true;
}
