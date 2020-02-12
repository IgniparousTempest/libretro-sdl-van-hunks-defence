#include "PlayerId.hpp"


bool operator<(PlayerId left, PlayerId right) {
    return PlayerIdExtensions::Value(left) < PlayerIdExtensions::Value(right);
}