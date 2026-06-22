//
// Created by Andy on 2026/6/22.
//

#include "SceneEnd.h"
#include "SceneMain.h"
#include "Game.h"
#include <string>

void SceneEnd::init() {
    music = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (music == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load background music %s", SDL_GetError());
    }
    Mix_PlayMusic(music, -1);

    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
        if (!SDL_IsTextInputActive()) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL TextInput not active %s", SDL_GetError());
        }
    }
}

void SceneEnd::update(float deltaTime) {
    underlineBlinkTimer += deltaTime;
    if (underlineBlinkTimer >= 1.0f) {
        underlineBlinkTimer -= 1.0f;
    }
    newGameTimer += deltaTime;
    if (newGameTimer >= 1.2f) {
        newGameTimer -= 1.2f;
    }
}

void SceneEnd::render() {
    if (isTyping) {
        renderNameInput();
    }
    else {
        renderScoreboard();
    }
}

void SceneEnd::handleEvent(SDL_Event *event) {
    if (isTyping) {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                isTyping = false;
                SDL_StopTextInput();
                if (userName.empty()) {
                    userName = "unnamed";
                }
                game.insertLeaderboard(game.getScore(), userName);
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE && !userName.empty()) {
                deleteLastUTF8Character(userName);
            }
        }
        if (event->type == SDL_TEXTINPUT) {
            userName += event->text.text;
        }
    }
    else {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
                auto SceneMain = new ::SceneMain;
                game.changeScene(SceneMain);
            }
        }
    }
}

void SceneEnd::clean() {
    if (music != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void SceneEnd::renderNameInput() {
    int score = game.getScore();
    std::string scoreText = "Score: " + std::to_string(score);
    std::string gameOverText = "Game Over";
    std::string instruction1Text = "Please enter your name";
    std::string instruction2Text = "and press return:";
    game.renderTextCentered(scoreText, 0.15, false);
    game.renderTextCentered(gameOverText, 0.4, true);
    game.renderTextCentered(instruction1Text, 0.6, false);
    game.renderTextCentered(instruction2Text, 0.65, false);
    if (!userName.empty()) {
        SDL_Point point = game.renderTextCentered(userName, 0.8, false);
        if (underlineBlinkTimer < 0.5f) {
            game.renderTextPosition("_", point.x, point.y);
        }
    }
    else {
        if (underlineBlinkTimer < 0.5f) {
            game.renderTextCentered("_", 0.8, false);
        }
    }
}

void SceneEnd::renderScoreboard() {
    game.renderTextCentered("Leaderboard", 0.1, true);
    int index = 1;
    float posY = 0.2 * game.getWindowHeight();
    float posX = 0.15 * game.getWindowWidth();
    for (auto item: game.getLeaderboard()) {
        std::string score = std::to_string(item.first);
        std::string name = std::to_string(index) + ". " + item.second;
        game.renderTextPosition(name, posX, posY);
        game.renderTextPosition(score, posX, posY, false);
        posY += static_cast<int>(50 * game.getScale());
        index++;
    }
    if (newGameTimer < 0.6f) {
        game.renderTextCentered("Press SPACE to Start Again", 0.8, false);
    }
}

void SceneEnd::deleteLastUTF8Character(std::string &str) {
    if (!str.empty()) {
        auto lastChar = str.back();
        if ((lastChar & 0b10000000) == 0b10000000) {
            str.pop_back();
            while ((str.back() & 0b11000000) != 0b11000000) {
                str.pop_back();
            }
        }
        str.pop_back();
    }
}