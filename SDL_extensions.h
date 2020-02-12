#ifndef SUPER_MISSILE_COMMAND_SDL_EXTENSIONS_H
#define SUPER_MISSILE_COMMAND_SDL_EXTENSIONS_H

#include <begin_code.h>
#include <SDL_types.h>
#include <SDL_rect.h>

SDL_FORCE_INLINE SDL_bool SDL_FPointInFRect(const SDL_FPoint *p, const SDL_FRect *r)
{
    return ( (p->x >= r->x) && (p->x < (r->x + r->w)) &&
             (p->y >= r->y) && (p->y < (r->y + r->h)) ) ? SDL_TRUE : SDL_FALSE;
}

#endif //SUPER_MISSILE_COMMAND_SDL_EXTENSIONS_H
