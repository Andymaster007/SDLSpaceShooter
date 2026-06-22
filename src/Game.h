//
// Created by Andy on 2026/6/10.
//

#ifndef SDLSPACESHOOTER_GAME_H
#define SDLSPACESHOOTER_GAME_H

#include "Scene.h"
#include "Object.h"
#include <SDL_ttf.h>
#include <string>
#include <map>

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
    SDL_Point renderTextCentered(const std::string& text, float posY, bool isTitle);
    void renderTextPosition(const std::string& text, int posX, int posY, bool isLeft = true);
    int getScore();
    void setScoreAdd(int score);
    void setScore(int score);
    std::multimap<int, std::string, std::greater<int>>& getLeaderboard();
    void insertLeaderboard(int score, std::string name);
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
    int FPS = 60;
    Uint32 expectedFrameTime;
    float deltaTime;
    Background nearStars;
    Background farStars;
    TTF_Font *titleFont;
    TTF_Font *textFont;
    int playerScore = 0;
    std::multimap <int, std::string, std::greater<int>> leaderboard;

    void updateBackground(float deltaTime);
    void renderBackground();
};

#endif //SDLSPACESHOOTER_GAME_H