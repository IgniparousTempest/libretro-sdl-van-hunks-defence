#ifndef SUPER_MISSILE_COMMAND_ENEMYCANNONBALLFACTORY_HPP
#define SUPER_MISSILE_COMMAND_ENEMYCANNONBALLFACTORY_HPP


#include <random>
#include <queue>
#include "../assets.hpp"
#include "../game_object.hpp"
#include "cannon_ball.hpp"

class EnemyCannonBallFactory : public GameObject {
public:
    EnemyCannonBallFactory(Assets* assets, int startX, int endX, const std::vector<GameObject*> &targets, float speed);
    void LoadCannons(int number, float timeWindow);
    void Update(double deltaTime, std::vector<CannonBall*>* cannonBalls);
    bool AllFired() { return pendingCannonBalls.empty(); }

private:
    Assets* assets;
    int startX;
    int endX;
    float speed;
    std::vector<FRect> targets;
    std::mt19937 rng;
    float elapsedTime = 0;
    /// The time offsets until each cannon ball fires. This is a min heap in C++ (it is very ugly).
    std::priority_queue<float, std::vector<float>, std::greater<>> pendingCannonBalls;

    CannonBall* Fire();
};


#endif //SUPER_MISSILE_COMMAND_ENEMYCANNONBALLFACTORY_HPP
