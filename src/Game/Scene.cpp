#include "Scene.h"
#include "Menu.h"
#include "Game.h"
#include "../Core/Input.h"
#include "../Core/Math.h"
#include "../Entities/Icon.h"

void Scene::Init(Game* g) {
	//Initialize all the things
	game = g;
	Menu::SetGame(game);
	Entity::SetGame(game);

	score_txt.Init(&game->default_fonts[30*Text::res_scale]);
	score_txt.SetStr("Score: ");
	score_txt.MoveTo({10, 4});

	selected_icon.w = selected_icon.h = 33;
}

void Scene::ChangeScene(SceneName new_scn) {
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& m : menus)
		m->Open(false);
	menus.clear();
	entities.clear();
	score = 0;
	selected_icon.x = selected_icon.y = -32;

	switch (curr_scn) {
		case SceneName::Title:
			menus.push_back(new Menu(MenuName::Main));
			FindMenu(MenuName::Main)->Open();
			menus.push_back(new Menu(MenuName::Options));
		break;

		case SceneName::Game:
			//First add the pause menu
			menus.push_back(new Menu(MenuName::Options_I));

			//Next we gotta add in our icons
			Sprite::Info icon_info = {}; icon_info.origin = { .5 };
			for (uchar row = 0; row <= 9; ++row) {
				for (uchar col = 0; col <= 9; ++col) {
					//CONSIDER PUTTING ALL OF THE ICONS INTO ONE SHEET
					switch (rand() % 3) {
						case 0:
							icon_info.sheet = "Ghost";
						break;
						case 1:
							icon_info.sheet = "Pumpkin";
						break;
						case 2:
							icon_info.sheet = "Witch";
						break;
					}
					icon_info.pos = { 56 + col * 32, 56 + row * 32 };

					entities.push_back(new Icon(icon_info));
				}
			}
		break;
	}
}

void Scene::GetInput() {
	//Input for the entities
	for (auto& e : entities)
		e->GetInput();

	//Input for the menus
	for (uchar i = 0; i < menus.size(); ++i)
		menus[i]->GetInput();

	//Game Input
	if (curr_scn == SceneName::Game) {
		//Open the Options Menu
		if (Input::KeyPressed(SCO)) OpenMenu(MenuName::Options_I);
		
		//Move the selected_icon rect
		//THIS ISN'T QUITE RIGHT YET - FIX TO-DO
		selected_icon.x = 8 + floor(Input::MousePos().x / 32) * 32;
		selected_icon.y = 8 + floor(Input::MousePos().y / 32) * 32;
		Math::Clamp(selected_icon.x, 40, 328);
		Math::Clamp(selected_icon.y, 40, 328);
	}
}

void Scene::Update() {

	//Every frame:
	//Update entities, interface buttons, & menus
	for (auto& e : entities)
		e->Update();
	for (auto& m : menus) {
		m->Update();
		if (m->to_close) OpenMenu(m->GetName(), false);
	}

	if (game->GetGameFrames() % 10 == 0) {
		//Sort the entities vector by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
		// last (closest to the camera)
		sort(entities.begin(), entities.end(),
			[](const Entity* a, const Entity* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}
}

void Scene::Draw() {
	//Entities
	for (const auto& e : entities)
		e->Draw();

	//If we're playing the game...
	if (curr_scn == SceneName::Game) {
		//First draw the grid
		game->renderer.DrawGrid({ 40 }, { 360 }, 32);

		//Draw the currently selected icon
		game->renderer.DrawRect(selected_icon, Color(0, 0), Color(1, 0, 0));
	}
}

void Scene::DrawGUI() {
	for (const auto& e : entities) e->DrawGUI();


	//Draw the score
	if (curr_scn == SceneName::Game)
		game->renderer.DrawTxt(score_txt);

	//Menus are drawn last since they will always be closest to the camera
	for (const auto& m : menus)
		m->Draw();
}

void Scene::Resize() {
	for (const auto& m : menus) m->Resize();
}

void Scene::OpenMenu(const MenuName menu, const bool o) {
	if (Menu* m = FindMenu(menu))
		m->Open();
	else
		cout << "Scene::OpenMenu(): Cannot open non-existent menu.\n";

}

Menu* Scene::FindMenu(MenuName menu) {
	for (const auto& m : menus)
		if (m->GetName() == menu) return m;

	return nullptr;
}
