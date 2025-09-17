#pragma once
#include <queue>
#include <unordered_map>
#include "../Core/Enums.h"
#include "../Graphics/Text.h"

using namespace std;

class Game;
class Menu;
class Entity;
class UI;

class Scene {
public:
    SceneName curr_scn = SceneName::Title;
    vector<Entity*> entities;
    uint score = 0;
    Text score_txt;

	Scene() = default;
	~Scene() {
        for (auto& e : entities) delete e;
        entities.clear();

        for (auto& m : menus) delete m;
        menus.clear();
    }
	void Init(Game* g);

    void ChangeScene(SceneName new_scn);

    //Engine stuff
    void GetInput();
    void Update();
    void Draw();
    void DrawGUI();
    void Resize();


    //Menu handling
    void CreateMenu(const MenuName menu);
    void OpenMenu(const MenuName menu, const bool o = true);
    bool MenuOpen(const MenuName menu);
    Menu* FindMenu(const MenuName menu);

    //Entities
    inline void AddEntity(Entity* e) { entities.push_back(e); }
    void RemoveEntity(Entity* e);

private:
    vector<Menu*> menus;
    Rect selected_icon;

    inline static Game* game = nullptr;
};