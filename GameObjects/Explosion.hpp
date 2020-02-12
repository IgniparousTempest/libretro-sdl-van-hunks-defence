#ifndef SUPER_MISSILE_COMMAND_EXPLOSION_HPP
#define SUPER_MISSILE_COMMAND_EXPLOSION_HPP


#include "../game_object.hpp"
#include "../assets.hpp"
#include "../Animation.hpp"
#include "../PlayerId.hpp"

class Explosion : public GameObject {
public:
    Explosion(Assets* assets, enum PlayerId playerId, float x, float y);
    void Update(double deltaTime) override;
    void Render(SDL_Renderer* renderer) override;
    float Radius() {
        switch (anim.GetCurrentFrame()) {
            case 0:
                return 74.0f / 2.0f;
            case 1:
                return 59.0f / 2.0f;
            case 2:
                return 40.0f / 2.0f;
            default:
                throw std::out_of_range("Animation in bad state");
        }
    }
    enum PlayerId PlayerId() { return playerId; }

private:
    enum PlayerId playerId;
    Animation anim;
};


#endif //SUPER_MISSILE_COMMAND_EXPLOSION_HPP
