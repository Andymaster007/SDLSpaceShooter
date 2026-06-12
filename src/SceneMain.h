//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_SCENEMAIN_H
#define SDLSPACESHOOTER_SCENEMAIN_H

#include "Scene.h"
#include "Object.h"

class Game;

class SceneMain: public Scene {
public:
    SceneMain();
    ~SceneMain();

    void update() override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;
    void keyboardControl();
private:
    Game &game;
    Player player;
};



#endif //SDLSPACESHOOTER_SCENEMAIN_H