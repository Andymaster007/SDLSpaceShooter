//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_SCENEMAIN_H
#define SDLSPACESHOOTER_SCENEMAIN_H

#include <list>
#include "Scene.h"
#include "Object.h"
#include <random>

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
    void spawnEnemy();
    void updateEnemies(float deltaTime);
    void renderEnemies();
    void shootEnemy(Enemy *enemy);
    SDL_FPoint getDirection(Enemy *enemy);
    void updateEnemyProjectiles(float deltaTime);
    void renderEnemyProjectiles();
    void enemyExplode(Enemy *enemy);
    void updatePlayer(float deltaTime);
private:
    Game &game;
    Player player;
    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayer;
    Enemy enemyTemplate;
    std::list<Enemy*> enemies;
    ProjectileEnemy projectileEnemyTemplate;
    std::list<ProjectileEnemy*> projectileEnemies;
    Explosion explosionTemplate;
    std::list<Explosion*> explosions;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    int scalingFactor = 4;
    bool isDead = false;
};



#endif //SDLSPACESHOOTER_SCENEMAIN_H