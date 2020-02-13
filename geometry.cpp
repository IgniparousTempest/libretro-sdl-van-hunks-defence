#include "geometry.hpp"

bool FPointInFRect(FPoint *point, FRect *rect) {
    return point->x >= rect->x && point->y >= rect->y && point->x < rect->x + rect->w && point->y < rect->y + rect->h;
}
