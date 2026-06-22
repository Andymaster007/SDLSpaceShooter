//
// Created by Andy on 2026/6/21.
//

#include "SceneTitle.h"
#include "Game.h"
#include "SceneMain.h"
#include <string>


void SceneTitle::init() {
    music = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (music == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't load background music %s", SDL_GetError());
    }
    Mix_PlayMusic(music, -1);
}

void SceneTitle::update(float deltaTime) {
    timer += deltaTime;
    if (timer >= 1.2f) {
        timer -= 1.2f;
    }
}

void SceneTitle::render() {
    std::string title = "Space Shooter";
    game.renderTextCentered(title, 0.4, true);
    if (timer < 0.6f) {
        std::string instruction = "Press SPACE to Start";
        game.renderTextCentered(instruction, 0.8, false);
    }
}

void SceneTitle::handleEvent(SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
            auto sceneMain = new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}

void SceneTitle::clean() {
    if (music != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }
}