#pragma once
#include <queue>
#include <unordered_map>
#include "../Engine/Enums.h"
#include "../Engine/Math/Geometry.h"

using namespace std;

class Engine;
class Menu;
class Entity;
class Icon;
class UI;
class Text;

class Game {
public:
    Scene curr_scn = Scene::Title;
    vector<Entity*> entities;
    vector<Icon*> icons;
    Icon* swapped_icons[2];
    uint score = 0;
    bool paused = false;
    Text* score_txt;
    Icon* chosen_icon = nullptr;
    Icon* swapped_icon = nullptr;

    Game() = default;
	~Game() {
        for (auto& e : entities) delete e;
        entities.clear();

        for (auto& i : icons) delete i;
        icons.clear();

        for (auto& m : menus) delete m;
        menus.clear();
    }
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

    //Entities
    inline void AddEntity(Entity* e) { entities.push_back(e); }
    void RemoveEntity(Entity* e);

    //Gameplay
    void CheckSwap(Icon* icon);

private:
    vector<Menu*> menus;

    inline static Engine* engine = nullptr;
};