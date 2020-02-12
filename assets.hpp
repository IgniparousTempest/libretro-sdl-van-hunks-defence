#ifndef SUPER_MISSILE_COMMAND_ASSETS_HPP
#define SUPER_MISSILE_COMMAND_ASSETS_HPP

#include <map>
#include <string>
#include <SDL_mixer.h>
#include "Auxiliary.hpp"
#include "SDL_FontCache/SDL_FontCache.h"
#include "PlayerId.hpp"

class Assets {
public:
    Assets(SDL_Renderer* renderer) : rng((std::random_device())()) {
        font = FC_CreateFont();
        fontLarge = FC_CreateFont();
        fontPlayer1 = FC_CreateFont();
        fontPlayer2 = FC_CreateFont();
        fontPlayer3 = FC_CreateFont();
        fontPlayer4 = FC_CreateFont();
        fontCredits = FC_CreateFont();
        std::string fontPath = "./assets/TreasureMapDeadhand-yLA3.ttf";
        FC_LoadFont(font, renderer, fontPath.c_str(), 30, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
        FC_LoadFont(fontLarge, renderer, fontPath.c_str(), 50, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
        FC_LoadFont(fontPlayer1, renderer, fontPath.c_str(), 30, FC_MakeColor(207,94,83,255), TTF_STYLE_NORMAL);
        FC_LoadFont(fontPlayer2, renderer, fontPath.c_str(), 30, FC_MakeColor(224,180,35,255), TTF_STYLE_NORMAL);
        FC_LoadFont(fontPlayer3, renderer, fontPath.c_str(), 30, FC_MakeColor(112,180,76,255), TTF_STYLE_NORMAL);
        FC_LoadFont(fontPlayer4, renderer, fontPath.c_str(), 30, FC_MakeColor(107,147,184,255), TTF_STYLE_NORMAL);
        FC_LoadFont(fontCredits, renderer, "./assets/Calibri Regular.ttf", 16, FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);

        std::vector<std::string> textureFiles = {
                "crosshair1",
                "crosshair2",
                "crosshair3",
                "crosshair4",
                "ship0",
                "ship1",
                "ship2",
                "ship3",
                "ship4",
                "ship_destroyed_player",
                "ship_destroyed_treasure",
                "cannon",
                "cannon_ball",
                "cannon_ball_trail",
                "explosion",
                "tile_water",
                "tile_water_animated",
                "sand_top_left",
                "sand_top",
                "sand_top_right",
                "sand_centre_left",
                "sand_centre",
                "sand_centre_right",
                "sand_bottom_left",
                "sand_bottom",
                "sand_bottom_right",
                "sand_inner_top_left",
                "sand_inner_top_right",
                "sand_inner_bottom_left",
                "sand_inner_bottom_right",
                "sand_single_horizontal",
                "sand_single_vertical",
                "flag_van_hunks_fluttering",
                "flag_voc_fluttering",
                "flag_voc_fluttering2",
        };
        std::string texturesFolder = "./assets/";
        for (auto &name : textureFiles)
            assets[name] = Auxiliary::loadTexture(renderer, (texturesFolder + name + ".png").c_str());

        std::string soundsFolder = "./assets/";
        std::vector<std::string> soundFiles = {
                "cannon1",
                "cannon2",
                "explosion1",
                "explosion2",
        };
        for (auto &name : soundFiles)
            sounds[name] = Mix_LoadWAV((soundsFolder + name + ".wav").c_str());
    }

    ~Assets() {
        // Free up textures
        for (auto const& entry : assets)
            SDL_DestroyTexture(entry.second);

        // Free up sounds
        for (auto const& entry : sounds)
            Mix_FreeChunk(entry.second);

        // Free up fonts
        FC_FreeFont(font);
        FC_FreeFont(fontLarge);
        FC_FreeFont(fontPlayer1);
        FC_FreeFont(fontPlayer2);
        FC_FreeFont(fontPlayer3);
        FC_FreeFont(fontPlayer4);
        FC_FreeFont(fontCredits);
    }

    SDL_Texture* get(const std::string& key) {
        // Dynamically load texture
//        if (assets.find(key) == assets.end())
//            assets[key] = Auxiliary::loadTexture(renderer, (assetsDirectory + key + ".png").c_str());
        return assets.at(key);
    }

    std::tuple<int, int> dimensions(const std::string& key) {
        int w, h;
        SDL_QueryTexture(this->get(key), nullptr, nullptr, &w, &h);
        return {w, h};
    }

    FC_Font* Font() { return font; }

    FC_Font* FontLarge() { return fontLarge; }

    FC_Font* Font(enum PlayerId playerId) {
        switch (playerId) {
            case PlayerId::player1:
                return fontPlayer1;
            case PlayerId::player2:
                return fontPlayer2;
            case PlayerId::player3:
                return fontPlayer3;
            case PlayerId::player4:
                return fontPlayer4;
            default:
                throw std::invalid_argument("Invalid playerId");
        }
    }

    FC_Font* FontCredits() { return fontCredits; }

    void PlaySoundExplosion() {
        PlayFromList({"explosion1", "explosion2"});
    }

    void PlaySoundCannonFire() {
        PlayFromList({"cannon1", "cannon2"});
    }

private:
    std::mt19937 rng;
    FC_Font* font;
    FC_Font* fontLarge;
    FC_Font* fontPlayer1;
    FC_Font* fontPlayer2;
    FC_Font* fontPlayer3;
    FC_Font* fontPlayer4;
    FC_Font* fontCredits;
    std::map<std::string, SDL_Texture*> assets;
    std::map<std::string, Mix_Chunk*> sounds;

    void PlayFromList(const std::vector<std::string> &options) {
        std::string key = *Auxiliary::Choose(options.begin(), options.end(), rng);
        Mix_PlayChannel(-1, sounds[key], 0);
    }
};

#endif //SUPER_MISSILE_COMMAND_ASSETS_HPP
