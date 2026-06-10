#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

int main(int, char**) {
    std::cout << "Hello World!" << std::endl;

    //SDL初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    //创建窗口
    SDL_Window *window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);

    //创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //SDL_Image初始化
    if (IMG_Init(IMG_INIT_PNG |IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        std::cout << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    //加载图片
    SDL_Texture *texture = IMG_LoadTexture(renderer, "assets/image/Health UI Black.png");

    //SDL_Mixer初始化
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    //加载音频
    Mix_Music *music = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    Mix_PlayMusic(music, -1);

    //SDL_TTF初始化
    if (TTF_Init() != 0) {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    //加载字体
    TTF_Font *font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 24);

    //创建文本
    SDL_Color color = {255,255,255,255};
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, "Hello, SDL! 你好，SDL!", color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, surface);

    //渲染循环
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quitting" << std::endl;
                break;
            }
        }
        //清理窗口
        SDL_RenderClear(renderer);

        //画正方形
        SDL_Rect rect = {100,100,100,100};
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer, &rect);

        //画图片
        SDL_Rect dstrect = {125,125,50,50};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        //画文本
        SDL_Rect textRect = {300, 300, surface->w, surface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        //重置渲染颜色
        SDL_SetRenderDrawColor(renderer,0,0,0,255);

        //更新窗口
        SDL_RenderPresent(renderer);
    }
    //释放文本纹理
    SDL_DestroyTexture(textTexture);
    //释放文本模板
    SDL_FreeSurface(surface);
    //释放字体
    TTF_CloseFont(font);
    //退出SDL_TTF
    TTF_Quit();
    //释放音频
    Mix_FreeMusic(music);
    //关闭音频设备
    Mix_CloseAudio();
    //退出SDL_Mixer
    Mix_Quit();
    //释放图片
    SDL_DestroyTexture(texture);
    //退出SDL_Image
    IMG_Quit();
    //释放渲染器
    SDL_DestroyRenderer(renderer);
    //释放窗口
    SDL_DestroyWindow(window);
    //退出SQL
    SDL_Quit();
    return 0;
}