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
    int speed = 300;
    Uint32 coolDown = 500;
    Uint32 lastShootTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 600;
};

struct Enemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
};



#endif //SDLSPACESHOOTER_OBJECT_H