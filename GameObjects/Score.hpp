#ifndef SUPER_MISSILE_COMMAND_SCORE_HPP
#define SUPER_MISSILE_COMMAND_SCORE_HPP


#include "../assets.hpp"

class Score {
public:
    Score(Assets* assets, float centreX, float topY);
    void Add(int increase);

    void Render(SDL_Renderer* renderer);

private:
    Assets* assets;
    float centreX;
    float topY;
    int score = 0;
};


#endif //SUPER_MISSILE_COMMAND_SCORE_HPP
