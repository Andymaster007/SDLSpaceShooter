//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_GAME_H
#define SDLSPACESHOOTER_GAME_H

#include "Scene.h"


class Game {
public:
    static Game& getInstance() {
        static Game instance;
        return instance;
    }
    ~Game();
    void init();
    void clean();
    void changeScene(Scene *scene);
    void run();
    void handleEvent(SDL_Event *event);
    void update();
    void render();
    SDL_Window *getWindow();
    SDL_Renderer *getRenderer();
    int getWindowWidth();
    int getWindowHeight();

private:
    Game();
    Game(const Game &) = delete;
    Game& operator=(const Game&) = delete;
    bool isRunning = true;
    Scene *currentScene = nullptr;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int windowWidth = 600;
    int windowHeight = 800;
};



#endif //SDLSPACESHOOTER_GAME_H