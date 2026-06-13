//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_SCENEMAIN_H
#define SDLSPACESHOOTER_SCENEMAIN_H

#include <list>

#include "Scene.h"
#include "Object.h"

class Game;

class SceneMain: public Scene {
public:
    SceneMain();
    ~SceneMain();

    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;
    void keyboardControl(float deltaTime);
    void shootPlayer();
    void updatePlayerProjectiles(float deltaTime);
    void renderPlayerProjectiles();
private:
    Game &game;
    Player player;
    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayer;
};



#endif //SDLSPACESHOOTER_SCENEMAIN_H