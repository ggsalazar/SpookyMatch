#pragma once
#include <chrono>
#include "Graphics/Camera.h"
#include "Graphics/Font.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer.h" //SDL_render, Sprite (SDL_image)
#include "../Game/Game.h"

using namespace std;
using namespace chrono;
using hr_clock = high_resolution_clock;
using durationf = duration<float>;

class Engine {
private:
    //Variables
    uint fps = 0;
    uint game_frames = 0;
    float target_frame_time;
    hr_clock::time_point last_time;
    durationf delta;
    float accumulated_time = 0.f;
    float music_volume = 100;
    float sfx_volume = 100;

public:
    //Game UTH details
    const Vec2u min_res = { 400 };
    Vec2u resolution;
    Window window;
    Renderer renderer;
    bool running = true;
    float delta_time = 0.f;
    Game game;

    //Camera
    Camera camera;

    //Miscellaneous
    unordered_map<int, Font> fonts;
    Sprite cursor;

    Engine(const char* title, const uint init_fps);
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
    void SetMusicVolume(float n_v);
    inline float GetMusicVolume() const { return music_volume; }
    void SetSFXVolume(float n_v);
    inline float GetSFXVolume() const { return sfx_volume; }

    void SetResolution(uchar res_scalar);
    void SetResolution(Vec2u n_r);
    void SetRes();
};