//
// Created by Andy on 2026/6/22.
//

#ifndef SDLSPACESHOOTER_SCENEEND_H
#define SDLSPACESHOOTER_SCENEEND_H

#include "Scene.h"
#include <string>

class SceneEnd: public Scene {
public:
    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event *event) override;
    void init() override;
    void clean() override;
private:
    bool isTyping = true;
    std::string userName;
    float underlineBlinkTimer = 0.0f;

    void renderScoreboard();
    void renderNameInput();
    void deleteLastUTF8Character(std::string &str);
};


#endif //SDLSPACESHOOTER_SCENEEND_H