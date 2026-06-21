//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_SCENE_H
#define SDLSPACESHOOTER_SCENE_H

#include <SDL.h>

class Game;

class Scene {
public:
    Scene();
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvent(SDL_Event *event) = 0;
protected:
    Game &game;
};


#endif //SDLSPACESHOOTER_SCENE_H