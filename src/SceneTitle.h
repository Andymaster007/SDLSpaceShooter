//
// Created by Andy on 2026/6/21.
//

#ifndef SDLSPACESHOOTER_SCENETITLE_H
#define SDLSPACESHOOTER_SCENETITLE_H

#include "Scene.h"
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Game;

class SceneTitle : public Scene {
public:
    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;
private:
    Mix_Music *music;
    float timer = 0.0f;
};

#endif //SDLSPACESHOOTER_SCENETITLE_H