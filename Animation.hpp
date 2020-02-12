#ifndef SUPER_MISSILE_COMMAND_ANIMATION_HPP
#define SUPER_MISSILE_COMMAND_ANIMATION_HPP


class Animation {
private:
    int    CurrentFrame;
    int     FrameInc;
    int     FrameRate; //Milliseconds
    long    OldTime;
    bool isComplete = false;

public:
    int    MaxFrames;
    bool    Oscillate;
    bool animateOnce = false;

public:
    Animation();

    void Update();

public:
    void SetFrameRate(int Rate);

    void SetCurrentFrame(int Frame);

    int GetCurrentFrame();

    bool IsComplete() { return isComplete; }
};


#endif //SUPER_MISSILE_COMMAND_ANIMATION_HPP
