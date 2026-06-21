//
// Created by Andy on 2026/6/10.
//

#include <iostream>
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_image: %s\n", IMG_GetError());
        isRunning = false;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
        isRunning = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't open audio: %s\n", Mix_GetError());
        isRunning = false;
    }
    Mix_AllocateChannels(32);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    if (nearStars.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load near star texture %s", SDL_GetError());
        isRunning = false;
    }
    SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
    nearStars.width /= 2;
    nearStars.height /= 2;
    farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    if (farStars.texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load far star texture %s", SDL_GetError());
        isRunning = false;
    }
    SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
    farStars.speed = 20;
    farStars.width /= 2;
    farStars.height /= 2;

    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize TTF: %s\n", SDL_GetError());
        isRunning = false;
    }

    titleFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);
    if (titleFont == nullptr || textFont == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't load font: %s\n", SDL_GetError());
        isRunning = false;
    }

    currentScene = new SceneTitle();
    currentScene->init();
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
    if (nearStars.texture != nullptr) {
        SDL_DestroyTexture(nearStars.texture);
        nearStars.texture = nullptr;
    }
    if (farStars.texture != nullptr) {
        SDL_DestroyTexture(farStars.texture);
        farStars.texture = nullptr;
    }
    if (textFont != nullptr) {
        TTF_CloseFont(textFont);
        textFont = nullptr;
    }
    if (titleFont != nullptr) {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

void Game::update() {
    updateBackground(deltaTime);
    currentScene->update(deltaTime);
}

void Game::render() {
    SDL_RenderClear(renderer);
    renderBackground();
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

void Game::updateBackground(float deltaTime) {
    nearStars.offset += nearStars.speed * deltaTime;
    if (nearStars.offset > 0.0) {
        nearStars.offset -= nearStars.height;
    }

    farStars.offset += farStars.speed * deltaTime;
    if (farStars.offset > 0.0) {
        farStars.offset -= farStars.height;
    }
}

void Game::renderBackground() {
    for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height) {
        for (int posX = 0; posX < getWindowWidth(); posX += farStars.width) {
            SDL_Rect dstrect = {posX, posY, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, NULL, &dstrect);
        }
    }

    for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height) {
        for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width) {
            SDL_Rect dstrect = {posX, posY, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, NULL, &dstrect);
        }
    }
}

void Game::renderTextCentered(std::string text, float posY, bool isTitle) {
    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface;
    if (isTitle) {
        surface = TTF_RenderUTF8_Blended(titleFont, text.c_str(), color);
    }
    else {
        surface = TTF_RenderUTF8_Blended(textFont, text.c_str(), color);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int y = static_cast<int>((getWindowHeight() - surface->h) * posY);
    SDL_Rect rect = {getWindowWidth() / 2 - surface->w / 2, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}