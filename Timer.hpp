#ifndef SUPER_MISSILE_COMMAND_TIMER_HPP
#define SUPER_MISSILE_COMMAND_TIMER_HPP

#include <functional>
#include <utility>

class Timer {
public:
    void Reset(float duration_ms) {
        time = duration_ms;
        active = true;
    }

    void SetCallback(std::function<void(void)> callback) {
        this->callback = std::move(callback);
    }

    void Tick(float deltaTime) {
        if (!active)
            return;

        time -= deltaTime;
        if (time < 0) {
            active = false;
            if (callback != nullptr)
                callback();
        }
    }

    bool IsActive() { return active; }

    float RemainingMilliseconds() { return time; }

    float RemainingSeconds() { return time / 1000.0f; }

private:
    bool active = false;
    float time = 0;
    std::function<void(void)> callback = nullptr;
};

#endif //SUPER_MISSILE_COMMAND_TIMER_HPP
