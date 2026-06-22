//
// Created by Andy on 2026/6/22.
//

#include "SceneEnd.h"
#include "Game.h"
#include <string>

void SceneEnd::init() {
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
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE && !userName.empty()) {
                deleteLastUTF8Character(userName);
            }
        }
        if (event->type == SDL_TEXTINPUT) {
            userName += event->text.text;
        }
    }
}

void SceneEnd::clean() {

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