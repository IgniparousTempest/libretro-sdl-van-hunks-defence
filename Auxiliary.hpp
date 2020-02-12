#ifndef SUPER_MISSILE_COMMAND_AUXILIARY_HPP
#define SUPER_MISSILE_COMMAND_AUXILIARY_HPP

#include <SDL_render.h>
#include <string>
#include <SDL_image.h>
#include <random>

class Auxiliary {
public:
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path)
    {
        //The final texture
        SDL_Texture* newTexture = nullptr;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == nullptr )
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        }
        else
        {
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if( newTexture == nullptr )
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }

        return newTexture;
    }

    /**
     * Chooses a random element from a container.
     * @tparam Iter
     * @tparam RandomGenerator
     * @param start
     * @param end
     * @param g
     * @return
     */
    template<typename Iter, typename RandomGenerator>
    static Iter Choose(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    }
};

#endif //SUPER_MISSILE_COMMAND_AUXILIARY_HPP
