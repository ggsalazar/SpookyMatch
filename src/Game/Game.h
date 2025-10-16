#pragma once
#include <nlohmann/json.hpp>
#include "../Engine/Enums.h"
#include "../Engine/Graphics/Sprite.h"

using namespace std;
using json = nlohmann::json;

class Engine;
class Icon;
class Menu;
class Text;

class Game {
public:
    Scene curr_scn = Scene::Title;
    GameMode gm_mode = GameMode::NONE;
    vector<Icon*> icons;
    Icon* chosen_icons[2] = {nullptr};
    uint time_remaining = 0, max_time_remaining = 0, moves_remaining = 0, max_moves_remaining = 0, score = 0, high_score = 0;
    uchar combo = 0, max_combo = 0, move_buffer = 0, move_buffer_max = 45, icon_gap = 35;
    bool paused = false;
    Text* score_txt = nullptr;
    Text* high_score_txts[3];
    Text* combo_txt = nullptr;
    Text* remaining_txt = nullptr;

    //Matching stuff
    Text* match_txt;
    bool match_made = false, swap_back = false;
    uint match_timer_max = 180, match_timer = 180; //3 seconds
    vector<Icon*> matched_icons;

    json high_scores;

    Game() = default;
	~Game();
	void Init(Engine* e);

    void ChangeScene(Scene new_scn);

    //Engine stuff
    void GetInput();
    void Update();
    void Draw();
    void DrawGUI();
    void Resize();

    //Menu handling
    void OpenMenu(const MenuName menu, const bool o = true);
    Menu* FindMenu(const MenuName menu);

    //Gameplay
    void CheckSwap(Icon* icon);
    void RemoveIcons();

private:
    vector<Menu*> menus;
    Sprite cursor;
    Sprite game_board;

    inline static Engine* engine = nullptr;
};
