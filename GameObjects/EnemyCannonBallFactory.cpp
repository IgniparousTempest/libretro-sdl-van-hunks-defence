#include <iostream>
#include "EnemyCannonBallFactory.hpp"
#include "cannon_ball.hpp"

EnemyCannonBallFactory::EnemyCannonBallFactory(Assets *assets, int startX, int endX, const std::vector<GameObject*> &targets, float speed) : assets(assets), startX(startX), endX(endX), speed(speed), rng((std::random_device())()) {
    std::cout << "Targeting " << targets.size() << " ships." << std::endl;
    this->targets.resize(targets.size());
    int i = 0;
    float w, h;
    for (auto &t : targets) {
        w = t->w();
        h = t->h();
        this->targets[i].x = t->x() + w * 0.15f;
        this->targets[i].y = t->y() + h * 0.15f;
        this->targets[i].w = w * 0.7f;
        this->targets[i].h = h * (0.5f - 0.15f);  // Only target the top half of the ship.
        i++;
    }
}

void EnemyCannonBallFactory::LoadCannons(int number, float timeWindow) {
    elapsedTime = 0;
    pendingCannonBalls = std::priority_queue<float, std::vector<float>, std::greater<>>();
    std::uniform_real_distribution<> dist(0.0f, timeWindow);
    for (int i = 0; i < number; ++i)
        pendingCannonBalls.push(dist(rng));
}

void EnemyCannonBallFactory::Update(double deltaTime, std::vector<CannonBall*>* cannonBalls) {
    elapsedTime += deltaTime / 1000.0f;

    while (!pendingCannonBalls.empty() && elapsedTime >= pendingCannonBalls.top()) {
        cannonBalls->push_back(Fire());
        std::cout << "Enemy is firing a ball at offset " << pendingCannonBalls.top() << "," << elapsedTime << std::endl;
        pendingCannonBalls.pop();
    }
}

CannonBall* EnemyCannonBallFactory::Fire() {
    std::uniform_int_distribution<> distIndex(0, targets.size() - 1);
    auto target = targets[distIndex(rng)];
    std::uniform_real_distribution<> distStartX(startX, endX);
    std::uniform_real_distribution<> distTargetX(target.x, target.x + target.w);
    return new CannonBall(assets, PlayerId::computer, distStartX(rng), 0, distTargetX(rng), target.y + target.h / 2.0f, speed);
}
