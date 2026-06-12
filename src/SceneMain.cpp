//
// Created by Andy on 2026/6/11.
//

#include "SceneMain.h"
#include "Game.h"
#include <SDL_image.h>

SceneMain::SceneMain() : game(Game::getInstance()) {

}

SceneMain::~SceneMain() {

}

void SceneMain::init() {
    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    player.width /= 4;
    player.height /= 4;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;
}

void SceneMain::handleEvent(SDL_Event *event) {

}

void SceneMain::render() {
    SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y), player.width, player.height};
    SDL_RenderCopy(game.getRenderer(), player.texture, NULL, &playerRect);
}

void SceneMain::clean() {
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::update() {

}