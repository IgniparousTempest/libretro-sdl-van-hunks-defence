#include "Score.hpp"

Score::Score(Assets* assets, float centreX, float topY) : assets(assets), centreX(centreX), topY(topY) {

}

void Score::Add(int increase) {
    score += increase;
}

void Score::Render(SDL_Renderer *renderer) {
    auto text = std::to_string(score).c_str();
    float width = FC_GetWidth(assets->Font(), text);
    FC_Draw(assets->Font(), renderer, centreX - width / 2.0f, 0, text);
}

