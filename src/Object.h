//
// Created by Andy on 2026/6/12.
//

#ifndef SDLSPACESHOOTER_OBJECT_H
#define SDLSPACESHOOTER_OBJECT_H

#include <SDL.h>

enum class ItemType {
    Health,
    Shield,
    Time
};

struct Player {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 300;
    int health = 3;
    int maxHealth = 6;
    Uint32 coolDown = 200;
    Uint32 lastShootTime = 0;
};

struct ProjectilePlayer {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 600;
    int damage = 1;
};

struct Enemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int health = 1;
    Uint32 coolDown = 1500;
    Uint32 lastShootTime = 0;
};

struct ProjectileEnemy {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 500;
    int damage = 1;
};

struct Explosion {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int currentFrame = 0;
    int totalFrames = 0;
    Uint32 startTime = 0;
    Uint32 FPS = 10;
};

struct Item {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int bounce = 3;
    ItemType type = ItemType::Health;
};

struct Background {
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 30;
    float offset = 0.0;
};

#endif //SDLSPACESHOOTER_OBJECT_H