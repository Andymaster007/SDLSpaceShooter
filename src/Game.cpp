//
// Created by Andy on 2026/6/10.
//

#include "Game.h"

#include <iostream>
#include <ostream>

#include "SceneMain.h"
#include <SDL_image.h>

Game::Game() {

}

Game::~Game() {
    clean();
}

void Game::init() {
    expectedFrameTime = 1000 / FPS;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL: %s\n", SDL_GetError());
        isRunning = false;
    }

    window = SDL_CreateWindow("SDL Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't create window: %s\n", SDL_GetError());
        isRunning = false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't create renderer: %s\n", SDL_GetError());
    }
    currentScene = new SceneMain();
    currentScene->init();

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_image: %s\n", IMG_GetError());
        isRunning = false;
    }

}

void Game::run() {
    while (isRunning) {
        auto frameStart = SDL_GetTicks();
        SDL_Event event;
        handleEvent(&event);
        update();
        render();
        auto frameEnd = SDL_GetTicks();
        auto frameTime = frameEnd - frameStart;
        if (frameTime < expectedFrameTime) {
            SDL_Delay(expectedFrameTime - frameTime);
            deltaTime = expectedFrameTime / 1000.0f;
        }
        else {
            deltaTime = frameTime / 1000.0f;
        }
    }
}

void Game::changeScene(Scene *scene) {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::clean() {
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;
    }
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

void Game::update() {
    currentScene->update(deltaTime);
}

void Game::render() {
    SDL_RenderClear(renderer);
    currentScene->render();
    SDL_RenderPresent(renderer);
}

void Game::handleEvent(SDL_Event *event) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            isRunning = false;
        }
        currentScene->handleEvent(event);
    }
}

SDL_Window *Game::getWindow() {
    return window;
}

SDL_Renderer *Game::getRenderer() {
    return renderer;
}

int Game::getWindowWidth() {
    return windowWidth;
}

int Game::getWindowHeight() {
    return windowHeight;
}
