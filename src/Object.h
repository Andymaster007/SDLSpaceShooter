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
    int speed = 250;
    int health = 3;
    Uint32 coolDown = 300;
    Uint32 lastShootTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 500;
    int damage = 1;
};

struct Enemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 150;
    int health = 1;
    Uint32 coolDown = 1000;
    Uint32 lastShootTime = 0;
};

struct ProjectileEnemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 300;
    int damage = 1;
};

#endif //SDLSPACESHOOTER_OBJECT_H