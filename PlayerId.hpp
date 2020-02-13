#ifndef SUPER_MISSILE_COMMAND_PLAYERID_HPP
#define SUPER_MISSILE_COMMAND_PLAYERID_HPP

#include <vector>
#include <algorithm>
#include <stdexcept>

enum class PlayerId { computer, player1, player2, player3, player4 };

class PlayerIdExtensions {
public:
    static const std::vector<enum PlayerId> HumanPlayers() { return {PlayerId::player1, PlayerId::player2, PlayerId::player3, PlayerId::player4}; }

    static const int Value(enum PlayerId value) {
        switch (value) {
            case PlayerId::computer:
                return -1;
            case PlayerId::player1:
                return 1;
            case PlayerId::player2:
                return 2;
            case PlayerId::player3:
                return 3;
            case PlayerId::player4:
                return 4;
        }
        throw std::invalid_argument("Invalid player number");
    }

    /**
     * Inverts a vector of PlayerId.
     * @param list A vector of players to be inverted.
     * @return A vector containing all the human players not in @param list.
     */
    static const std::vector<enum PlayerId> InvertHumans(const std::vector<enum PlayerId> &list) {
        auto players = HumanPlayers();
        players.erase(
                std::remove_if(
                        players.begin(),
                        players.end(),
                        [list](const enum PlayerId &o) { return std::find(list.begin(), list.end(), o) != list.end(); }),
                players.end());
        return players;
    }
};

bool operator<(PlayerId left, PlayerId right);

#endif //SUPER_MISSILE_COMMAND_PLAYERID_HPP
