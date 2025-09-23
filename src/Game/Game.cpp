#include "Game.h"
#include "Menu.h"
#include "../Engine/Engine.h"
#include "../Engine/Input.h"
#include "../Engine/Graphics/Text.h"
#include "../Engine/Math/Math.h"
#include "../Entities/Icon.h"

void Game::Init(Engine* e) {
	//Initialize all the things
	engine = e;
	Menu::SetEngine(engine);
	Entity::SetEngine(engine, this);

	selected_tile.w = selected_tile.h = 33;
}

void Game::ChangeScene(Scene new_scn) {
	//Init the score text if it hasn't already
	if (!score_txt) {
		Text::Info t_info = {};
		t_info.font_size = 36; t_info.str = "Score: "; t_info.pos = { 10, 4 };
		score_txt = new Text(t_info);
	}
	
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& m : menus)
		m->Open(false);
	menus.clear();
	entities.clear();
	score = 0;
	selected_tile.x = selected_tile.y = -32;

	switch (curr_scn) {
		case Scene::Title:
			menus.push_back(new Menu(MenuName::Main));
			FindMenu(MenuName::Main)->Open();
			menus.push_back(new Menu(MenuName::Options));
		break;

		case Scene::Game:
			//First add the pause menu
			menus.push_back(new Menu(MenuName::Options_I));

			//Next we gotta add in our icons
			Sprite::Info icon_info = {}; icon_info.origin = { .5 };
			for (uchar row = 0; row <= 9; ++row) {
				for (uchar col = 0; col <= 9; ++col) {
					icon_info.pos = { 56 + col * 32, 56 + row * 32 };

					entities.push_back(new Icon(icon_info, static_cast<IconType>(rand()%8)));
				}
			}
		break;
	}
}

void Game::GetInput() {
	//Input for the entities
	for (auto& e : entities)
		e->GetInput();

	//Input for the menus
	for (uchar i = 0; i < menus.size(); ++i)
		menus[i]->GetInput();

	//Game Input
	if (curr_scn == Scene::Game) {
		//Open the Options Menu
		if (Input::KeyPressed(SCO)) OpenMenu(MenuName::Options_I);
		
		//Move the selected_tile rect
		selected_tile.x = 8 + floor((Input::MousePos().x-8) / 32) * 32;
		selected_tile.y = 8 + floor((Input::MousePos().y-8) / 32) * 32;
		Math::Clamp(selected_tile.x, 40, 328);
		Math::Clamp(selected_tile.y, 40, 328);
	}
}

void Game::Update() {

	//Every frame:
	//Update entities, interface buttons, & menus
	for (auto& e : entities)
		e->Update();
	for (auto& m : menus) {
		m->Update();
		if (m->to_close) OpenMenu(m->GetName(), false);
	}

	score_txt->SetStr("Score: " + to_string(score));

	if (engine->GetGameFrames() % 10 == 0) {
		//Sort the entities vector by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
		// last (closest to the camera)
		sort(entities.begin(), entities.end(),
			[](const Entity* a, const Entity* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}
}

void Game::Draw() {
	//If we're playing the game...
	if (curr_scn == Scene::Game) {
		//First draw the grid
		engine->renderer.DrawGrid({ 40 }, { 360 }, 32);

		//Draw the currently selected tile
		engine->renderer.DrawRect(selected_tile, Color(0, 0), Color(1, 0, 0));
	}


	//Entities
	for (const auto& e : entities)
		e->Draw();
}

void Game::DrawGUI() {
	for (const auto& e : entities) e->DrawGUI();


	//Draw the score
	if (curr_scn == Scene::Game)
		engine->renderer.DrawTxt(*score_txt);

	//Menus are drawn last since they will always be closest to the camera
	for (const auto& m : menus)
		m->Draw();
}

void Game::Resize() {
	for (const auto& m : menus) m->Resize();
}

void Game::OpenMenu(const MenuName menu, const bool o) {
	if (Menu* m = FindMenu(menu))
		m->Open();
	else
		cout << "Game::OpenMenu(): Cannot open non-existent menu.\n";

}

Menu* Game::FindMenu(MenuName menu) {
	for (const auto& m : menus)
		if (m->GetName() == menu) return m;

	return nullptr;
}
