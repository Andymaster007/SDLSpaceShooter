//
// Created by Andy on 2026/6/12.
//

#ifndef SDLSPACESHOOTER_OBJECT_H
#define SDLSPACESHOOTER_OBJECT_H

#include <SDL.h>

struct Player {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    float speed = 300.0f;
};





#endif //SDLSPACESHOOTER_OBJECT_H