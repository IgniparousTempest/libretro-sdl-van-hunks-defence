#include <cstdio>
#include "SDL.h"
#include "SDL_mixer.h"
#include "keyboard_handler.hpp"
#include "GameObjects/Score.hpp"
#include "Game.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Current displayed texture
SDL_Texture* gTexture = nullptr;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "Van Hunks Defence", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == nullptr )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == nullptr )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void close()
{
    //Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = nullptr;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        auto keyboardHandler = KeyboardHandler();
        auto assets = Assets(gRenderer);
        auto game = Game(&assets, SCREEN_WIDTH, SCREEN_HEIGHT);
        Uint64 currentTime = SDL_GetPerformanceCounter();

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        //While application is running
        while( !quit )
        {
            Uint64 newTime = SDL_GetPerformanceCounter();
            auto frameTime = (double) ((newTime - currentTime) * 1000.0 / (double) SDL_GetPerformanceFrequency());
            currentTime = newTime;

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                    keyboardHandler.handle(e);
                }
            }

            // Handle key presses
            if (keyboardHandler.isDown((SDL_Keycode) SDLK_LEFT)) {
                game.PressLeft(PlayerId::player1, frameTime);
            }
            if (keyboardHandler.isDown((SDL_Keycode) SDLK_RIGHT)) {
                game.PressRight(PlayerId::player1, frameTime);
            }
            if (keyboardHandler.isDown((SDL_Keycode) SDLK_UP)) {
                game.PressUp(PlayerId::player1, frameTime);
            }
            if (keyboardHandler.isDown((SDL_Keycode) SDLK_DOWN)) {
                game.PressDown(PlayerId::player1, frameTime);
            }
            if (keyboardHandler.triggerPressed((SDL_Keycode) SDLK_SPACE)) {
                game.PressFire(PlayerId::player1);
            }
            if (keyboardHandler.triggerPressed((SDL_Keycode) SDLK_RETURN)) {
                game.PressStart(PlayerId::player1);
            }

            // Updates
            game.Update(frameTime);

            //Clear screen
            SDL_RenderClear( gRenderer );

            //Render game to screen
            game.Render(gRenderer);

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}