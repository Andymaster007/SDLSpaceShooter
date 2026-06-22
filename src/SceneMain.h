//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_SCENEMAIN_H
#define SDLSPACESHOOTER_SCENEMAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>
#include <map>
#include <string>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Game;

class SceneMain: public Scene {
public:
    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;
private:
    Player player;
    Mix_Music *music;
    SDL_Texture *uiHealth;
    TTF_Font *scoreFont;

    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayer;
    Enemy enemyTemplate;
    std::list<Enemy*> enemies;
    ProjectileEnemy projectileEnemyTemplate;
    std::list<ProjectileEnemy*> projectileEnemies;
    Explosion explosionTemplate;
    std::list<Explosion*> explosions;
    Item itemHealthTemplate;
    std::list<Item*> items;
    std::map<std::string,Mix_Chunk*> soundEffects;

    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    int scalingFactor = 4;
    bool isDead = false;
    Uint32 gameStart = 0;
    float gameEnd = 0.0f;

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
    void updatePlayer();
    void updateExplosions();
    void renderExplosions();
    void dropItem(Enemy *enemy);
    void updateItems(float deltaTime);
    void playerGetItem(Item *item);
    void renderItems();
    void renderUI();
    void changeSceneDelay(float deltaTime);
};

#endif //SDLSPACESHOOTER_SCENEMAIN_H