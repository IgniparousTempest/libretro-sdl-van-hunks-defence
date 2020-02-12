#include <SDL_timer.h>
#include "Animation.hpp"

Animation::Animation() {
    CurrentFrame    = 0;
    MaxFrames       = 0;
    FrameInc        = 1;

    FrameRate       = 100; //Milliseconds
    OldTime         = SDL_GetTicks();

    Oscillate       = false;
}

void Animation::Update() {
    if(OldTime + FrameRate > SDL_GetTicks()) {
        return;
    }

    OldTime = SDL_GetTicks();

    CurrentFrame += FrameInc;

    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= MaxFrames) {
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= MaxFrames) {
            if (animateOnce) {
                CurrentFrame = MaxFrames - 1;
                isComplete = true;
            }
            else
                CurrentFrame = 0;
        }
    }
}

void Animation::SetFrameRate(int Rate) {
    FrameRate = Rate;
}

void Animation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame >= MaxFrames) return;

    CurrentFrame = Frame;
}

int Animation::GetCurrentFrame() {
    return CurrentFrame;
}