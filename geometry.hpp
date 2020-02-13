#ifndef VANHUNKSDEFENCE_LIBRETRO_GEOMETRY_HPP
#define VANHUNKSDEFENCE_LIBRETRO_GEOMETRY_HPP

struct FPoint {
    float x, y;
};

struct FRect {
    float x, y, w, h;
};

bool FPointInFRect(FPoint* point, FRect* rect);

#endif //VANHUNKSDEFENCE_LIBRETRO_GEOMETRY_HPP
