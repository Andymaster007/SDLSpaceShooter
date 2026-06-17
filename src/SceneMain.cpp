//
// Created by Andy on 2026/6/11.
//

#include "SceneMain.h"
#include "Game.h"
#include <SDL_image.h>
#include <cmath>
#include <iostream>
#include <ostream>

static constexpr float PI = 3.14159265358979323846f;

SceneMain::SceneMain() : game(Game::getInstance()) {

}

SceneMain::~SceneMain() {

}

void SceneMain::init() {
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player texture %s", SDL_GetError());
    }
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= scalingFactor;
    player.height /= scalingFactor;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    if (projectilePlayerTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load player projectile texture %s", SDL_GetError());
    }
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    projectilePlayerTemplate.width /= scalingFactor;
    projectilePlayerTemplate.height /= scalingFactor;

    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= scalingFactor;
    enemyTemplate.height /= scalingFactor;

    projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-1.png");
    if (projectileEnemyTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy projectile texture %s", SDL_GetError());
    }
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    projectileEnemyTemplate.width /= scalingFactor;
    projectileEnemyTemplate.height /= scalingFactor;

    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    if (explosionTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load explosion texture %s", SDL_GetError());
    }
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrames = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.width = explosionTemplate.height;

    itemHealthTemplate.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    if (itemHealthTemplate.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load life texture %s", SDL_GetError());
    }
    SDL_QueryTexture(itemHealthTemplate.texture, NULL, NULL, &itemHealthTemplate.width, &itemHealthTemplate.height);
    itemHealthTemplate.width /= scalingFactor;
    itemHealthTemplate.height /= scalingFactor;
}

void SceneMain::handleEvent(SDL_Event *event) {

}

void SceneMain::render() {
    renderPlayerProjectiles();
    renderEnemyProjectiles();
    renderEnemies();
    renderItems();
    renderExplosions();
    if (!isDead) {
        SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
        SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
    }
}

void SceneMain::clean() {
    for (auto projectile : projectilePlayer) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectilePlayer.clear();

    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    if (projectilePlayerTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
        projectilePlayerTemplate.texture = nullptr;
    }

    for (auto enemy: enemies) {
        if (enemy != nullptr) {
            delete enemy;
        }
    }
    enemies.clear();

    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
        enemyTemplate.texture = nullptr;
    }

    for (auto projectile : projectileEnemies) {
        if (projectile != nullptr) {
            delete projectile;
        }
    }
    projectileEnemies.clear();

    if (projectileEnemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
        projectileEnemyTemplate.texture = nullptr;
    }

    for (auto explosion: explosions) {
        if (explosion != nullptr) {
            delete explosion;
        }
    }
    explosions.clear();

    if (explosionTemplate.texture != nullptr) {
        SDL_DestroyTexture(explosionTemplate.texture);
        explosionTemplate.texture = nullptr;
    }

    for (auto item: items) {
        if (item != nullptr) {
            delete item;
        }
    }
    items.clear();

    if (itemHealthTemplate.texture != nullptr) {
        SDL_DestroyTexture(itemHealthTemplate.texture);
        itemHealthTemplate.texture = nullptr;
    }
}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawnEnemy();
    updateEnemies(deltaTime);
    updatePlayer();
    updateExplosions();
    updateItems(deltaTime);
}

void SceneMain::keyboardControl(float deltaTime) {
    if (isDead) {
        return;
    }
    auto keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_W]) {
        player.position.y -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        player.position.y += deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        player.position.x -= deltaTime * player.speed;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        player.position.x += deltaTime * player.speed;
    }

    if (player.position.x < 0) {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width) {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0) {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height) {
        player.position.y = game.getWindowHeight() - player.height;
    }

    if (keyboardState[SDL_SCANCODE_J]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - player.lastShootTime >player.coolDown) {
            player.lastShootTime = currentTime;
            shootPlayer();
        }
    }
}

void SceneMain::shootPlayer() {
    auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
    projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y = player.position.y;
    projectilePlayer.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime) {
    for (auto it = projectilePlayer.begin(); it != projectilePlayer.end();) {
        auto projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        if (projectile->position.y + projectile->height < 0) {
            delete projectile;
            it = projectilePlayer.erase(it);
        }
        else {
            bool hit = false;
            for (auto enemy: enemies) {
                SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
                SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
                if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
                    enemy->health -= projectile->damage;
                    delete projectile;
                    it = projectilePlayer.erase(it);
                    hit = true;
                    break;
                }
            }
            if (!hit) {
                ++it;
            }
        }
    }
}

void SceneMain::renderPlayerProjectiles() {
    for (auto projectile: projectilePlayer) {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        SDL_RenderCopy(game.getRenderer(), projectile->texture, NULL, &projectileRect);
    }
}

void SceneMain::spawnEnemy() {
    if (isDead) {
        return;
    }
    if (dis(gen) < 1 / 60.0f) {
        Enemy *enemy = new Enemy(enemyTemplate);
        enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
        enemy->position.y = -enemy->height;
        enemies.push_back(enemy);
    }
}

void SceneMain::updateEnemies(float deltaTime) {
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        enemy->position.y += enemy->speed * deltaTime;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            it = enemies.erase(it);
        }
        else {
            if (currentTime - enemy->lastShootTime > enemy->coolDown && !isDead) {
                shootEnemy(enemy);
                enemy->lastShootTime = currentTime;
            }
            if (enemy->health <= 0) {
                enemyExplode(enemy);
                it = enemies.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemies() {
    for (auto enemy: enemies) {
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy *enemy) {
    auto projectile = new ProjectileEnemy(projectileEnemyTemplate);
    projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
    projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;
    projectile->direction = getDirection(enemy);
    projectileEnemies.push_back(projectile);
}

SDL_FPoint SceneMain::getDirection(Enemy *enemy) {
    auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
    auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);
    if (x != 0 || y != 0) {
        auto length = std::sqrt(x * x + y * y);
        x /= length;
        y /= length;
    }
    return SDL_FPoint{x, y};
}

void SceneMain::updateEnemyProjectiles(float deltaTime) {
    for (auto it = projectileEnemies.begin(); it != projectileEnemies.end();) {
        auto projectile = *it;
        projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
        projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;
        if (projectile->position.y > game.getWindowHeight() || projectile->position.y + projectile->height < 0 || projectile->position.x + projectile->width < 0 || projectile->position.x > game.getWindowWidth()) {
            delete projectile;
            it = projectileEnemies.erase(it);
        }
        else {
            if (!isDead) {
                SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
                SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
                if (SDL_HasIntersection(&projectileRect, &playerRect)) {
                    player.health -= projectile->damage;
                    delete projectile;
                    it = projectileEnemies.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
}

void SceneMain::renderEnemyProjectiles() {
    for (auto projectile: projectileEnemies) {
        SDL_Rect projectileRect = {static_cast<int>(projectile->position.x), static_cast<int>(projectile->position.y), projectile->width, projectile->height};
        float angle = std::atan(projectile->direction.y / projectile->direction.x) * 180 / PI - 90;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
    }
}

void SceneMain::enemyExplode(Enemy *enemy) {
    auto currentTime = SDL_GetTicks();
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2 + 15;
    explosion->startTime = currentTime;
    explosions.push_back(explosion);
    if (dis(gen) < 0.5f) {
        dropItem(enemy);
    }
    delete enemy;
}

void SceneMain::updatePlayer() {
    if (isDead) {
        return;
    }
    if (player.health <= 0) {
        isDead = true;
        auto currentTime = SDL_GetTicks();
        auto explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2 + 3;
        explosion->startTime = currentTime;
        explosions.push_back(explosion);
    }
    else {
        for (auto enemy: enemies) {
            SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y), enemy->width, enemy->height};
            SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
            if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                player.health -= 1;
                enemy->health = 0;
            }
        }
    }
}

void SceneMain::updateExplosions() {
    auto currentTime = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();) {
        auto explosion = *it;
        explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
        if (explosion->currentFrame >= explosion->totalFrames) {
            delete explosion;
            it = explosions.erase(it);
        }
        else {
                ++it;
        }
    }
}

void SceneMain::renderExplosions() {
    for (auto explosion: explosions) {
        SDL_Rect explosionSrcrect = {explosion->currentFrame * explosion->width, 0, explosion->width, explosion->height};
        SDL_Rect explosionDstrect = {static_cast<int>(explosion->position.x), static_cast<int>(explosion->position.y) , explosion->width, explosion->height};
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &explosionSrcrect, &explosionDstrect);
    }
}

void SceneMain::dropItem(Enemy *enemy) {
    auto item = new Item(itemHealthTemplate);
    item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
    item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2 + 15;
    float angle = dis(gen) * 2 * PI;
    item->direction.x = std::cos(angle);
    item->direction.y = std::sin(angle);
    items.push_back(item);
}

void SceneMain::updateItems(float deltaTime) {
    for (auto it = items.begin(); it != items.end();) {
        auto item = *it;
        item->position.x += item->direction.x * item->speed * deltaTime;
        item->position.y += item->direction.y * item->speed * deltaTime;
        if (item->position.x < 0 && item->bounce > 0) {
            item->direction.x = -item->direction.x;
            item->bounce--;
        }
        if (item->position.x + item->width > game.getWindowWidth() && item->bounce > 0) {
            item->direction.x = -item->direction.x;
            item->bounce--;
        }
        if (item->position.y < 0 && item->bounce > 0) {
            item->direction.y = -item->direction.y;
            item->bounce--;
        }
        if (item->position.y + item->height > game.getWindowHeight() && item->bounce > 0) {
            item->direction.y = -item->direction.y;
            item->bounce--;
        }

        if (item->position.x + item->width < 0 || item->position.x > game.getWindowWidth() || item->position.y + item->height < 0 || item->position.y > game.getWindowHeight()) {
            delete item;
            it = items.erase(it);
        }
        else {
            SDL_Rect itemRect = {static_cast<int>(item->position.x), static_cast<int>(item->position.y), item->width, item->height};
            SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
            if (SDL_HasIntersection(&playerRect, &itemRect)) {
                playerGetItem(item);
                delete item;
                it = items.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}

void SceneMain::playerGetItem(Item *item) {
    if (item->type == ItemType::Health && player.health < player.maxHealth) {
        player.health += 1;
        std::cout << player.health << std::endl;
    }
}

void SceneMain::renderItems() {
    for (auto item: items) {
        SDL_Rect itemRect = {static_cast<int>(item->position.x), static_cast<int>(item->position.y), item->width, item->height};
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &itemRect);
    }
}
