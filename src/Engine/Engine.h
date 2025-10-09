#pragma once
#include <chrono>
#include <SDL3_ttf/SDL_ttf.h>
#include "Graphics/Camera.h" //Geometry (Vec2 (iostream))
#include "Graphics/Window.h" //SDL, SDL_main, SDL_video
#include "Graphics/Renderer.h" //SDL_render
#include "../Game/Game.h" //Enums, Sprite

using namespace std;
using namespace std::chrono;
using hr_clock = high_resolution_clock;
using durationf = duration<float>;

class Engine {
private:
    //Variables
    uchar fps = 0, game_frames = 0;
    float target_frame_time, delta_time = .0f;
    hr_clock::time_point last_time;
    durationf delta;
    float accumulated_time = 0.f;

public:
    //Game UTH details
    const Vec2u min_res = { 400 };
    Vec2u resolution;
    Window window;
    Renderer renderer;
    Game game;
    Camera camera;
    bool running = true;

    Engine(const char* title, const uchar init_fps);
    ~Engine() {
        TTF_Quit();
        SDL_Quit(); //Pretty sure this has to be called last
    }

    //Engine
    void Run();
    void ProcessInput();
    void Update();
    void Render();

    //Frame stuff
    inline uint GetFPS() const { return fps; }
    inline int GetGameFrames() const { return game_frames; }

    //Settings
    void SetResolution(uchar res_scalar);
    void SetResolution(Vec2u n_r);
    void SetRes();
};
