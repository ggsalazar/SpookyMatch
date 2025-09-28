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

	Text::Info t_info = {};
	t_info.font_size = 36; t_info.str = "Score: "; t_info.pos = { 10, 4 };
	score_txt = new Text(t_info);
	t_info.str = "Combo: "; t_info.origin.x = 1; t_info.pos.x = 390;
	combo_txt = new Text(t_info);

	//Debug stuff
	t_info.origin.x = .5;
	t_info.str = "MATCH MADE!!!";
	t_info.pos = { 200, 365 };
	match_txt = new Text(t_info);
}

void Game::ChangeScene(Scene new_scn) {
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& m : menus)
		m->Open(false);
	menus.clear();
	entities.clear();
	icons.clear();
	score = 0;
	combo = 0;
	paused = false;
	match_timer = match_timer_max;

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
			Sprite::Info icon_info = {};
			for (uchar row = 0; row <= 9; ++row) {
				for (uchar col = 0; col <= 9; ++col) {
					icon_info.pos = { 56 + col * 32, 56 + row * 32 };

					icons.push_back(new Icon(icon_info));
				}
			}
		break;
	}
}

void Game::GetInput() {
	//Input for the entities
	for (auto& e : entities)
		e->GetInput();
	for (auto& i : icons) i->GetInput();

	//Input for the menus
	for (uchar i = 0; i < menus.size(); ++i)
		menus[i]->GetInput();

	//Game Input
	if (curr_scn == Scene::Game) {
		//Open the Options Menu
		if (Input::KeyPressed(SCO)) {
			OpenMenu(MenuName::Options_I, !paused);
			paused = true;
		}
	}
}

void Game::Update() {
	//Update entities, interface buttons, & menus
	for (auto& e : entities) e->Update();
	for (auto& i : icons) {
		i->Update();

		if (i->GetPos() == i->pos_goal)	CheckSwap(i);
	}

	//If there was no match made, then the chosen icons need to swap back to their original positions
	if (!match_made and swap_back and chosen_icons[0] and chosen_icons[1]) {
		chosen_icons[0]->pos_goal = chosen_icons[0]->old_pos;
		chosen_icons[1]->pos_goal = chosen_icons[1]->old_pos;
		chosen_icons[0] = chosen_icons[1] = nullptr;
	}
	swap_back = false;


	for (auto& m : menus) {
		m->Update();
		if (m->to_close) OpenMenu(m->GetName(), false);
	}

	//Did we make a match?
	match_timer -= match_made;
	if (match_timer <= 0) {
		RemoveIcons();
		match_timer = match_timer_max;
	}

	score_txt->SetStr("Score: " + to_string(score));
	combo_txt->SetStr("Combo: " + to_string(combo) + "x | " + to_string(max_combo) + 'x');

	if (engine->GetGameFrames() % 10 == 0) {
		//Sort the entities vector by dfc value every 6th of a second (every 10 game frames) so that entities of a lower dfc value are drawn
		// last (closest to the camera)
		sort(entities.begin(), entities.end(),
			[](const Entity* a, const Entity* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
		sort(icons.begin(), icons.end(),
			[](const Icon* a, const Icon* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}

	//Remove dead icons walking
	for (auto it = icons.begin(); it != icons.end();) {
		Icon* i = *it;
		if (i->to_remove) it = icons.erase(it);
		else ++it;
	}
}

void Game::Draw() {
	//If we're playing the game...
	if (curr_scn == Scene::Game) {
		//First draw the grid
		engine->renderer.DrawGrid({ 40 }, { 360 }, 32);
	}


	//Entities
	for (const auto& e : entities)
		e->Draw();
	for (const auto& i : icons) i->Draw();
}

void Game::DrawGUI() {
	for (const auto& e : entities) e->DrawGUI();
	for (const auto& i : icons) i->DrawGUI();

	//Draw the score & combo
	if (curr_scn == Scene::Game) {
		engine->renderer.DrawTxt(*score_txt);
		engine->renderer.DrawTxt(*combo_txt);
		if (match_made) engine->renderer.DrawTxt(*match_txt);
	}

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

void Game::CheckSwap(Icon* icon) {

	icon->pos_goal = { 0 };

	//Check the icons that were just swapped to see if we matched 3+
	if (!match_made) {
		vector<Icon*> horiz_match;
		vector<Icon*> vert_match;

		horiz_match.push_back(icon);
		bool check_left = true, check_right = true;

		uchar i = 1;
		Vec2i check_pos = icon->GetPos();
		//Check left
		while (check_left and check_pos.x > 40) {
			check_pos.x = icon->GetPos().x - 32 * i;
			for (auto& ic : icons) {
				if (ic->GetPos() == check_pos or ic->pos_goal == check_pos) {
					if (ic->type == icon->type) {
						++i;
						horiz_match.push_back(ic);
					}
					else check_left = false;

					break;
				}
			}
		}
		//Check right
		i = 1;
		while (check_right and check_pos.x < 360) {
			check_pos.x = icon->GetPos().x + 32 * i;
			for (auto& ic : icons) {
				if (ic->GetPos() == check_pos or ic->pos_goal == check_pos) {
					if (ic->type == icon->type) {
						++i;
						horiz_match.push_back(ic);
					}
					else check_right = false;

					break;
				}
			}
		}

		//Check up/down even if we have made a horizontal match to see if we can make a bigger match
		vert_match.push_back(icon);
		check_pos = icon->GetPos();
		bool check_up = true, check_down = true;

		//Check up
		i = 1;
		while (check_up and check_pos.y > 40) {
			check_pos.y = icon->GetPos().y - 32 * i;
			for (auto& ic : icons) {
				if (ic->GetPos() == check_pos or ic->pos_goal == check_pos) {
					if (ic->type == icon->type) {
						++i;
						vert_match.push_back(ic);
					}
					else check_up = false;

					break;
				}
			}
		}

		//Check down
		i = 1;
		while (check_down and check_pos.y < 360) {
			check_pos.y = icon->GetPos().y + 32 * i;
			for (auto& ic : icons) {
				if (ic->GetPos() == check_pos or ic->pos_goal == check_pos) {
					if (ic->type == icon->type) {
						++i;
						vert_match.push_back(ic);
					}
					else check_down = false;

					break;
				}
			}
		}

		if (horiz_match.size() >= vert_match.size())
			for (auto& hm : horiz_match) matched_icons.push_back(hm);
		else if (vert_match.size() > horiz_match.size())
			for (auto& vm : vert_match) matched_icons.push_back(vm);

		match_made = matched_icons.size() >= 3;
		combo += match_made;
		max_combo = combo > max_combo ? combo : max_combo;

		for (auto& mi : matched_icons) mi->matched = match_made;

		if (!match_made) {
			matched_icons.clear();

			swap_back = (chosen_icons[0] and chosen_icons[1]);
		}
	}
}

void Game::RemoveIcons() {
	chosen_icons[0] = chosen_icons[1] = nullptr;
	match_made = false;

	//Score
	uint new_score = 0;
	switch (matched_icons.size()) {
		case 3: new_score = 100; break;
		case 4: new_score = 250; break;
		case 5: new_score = 1000; break;
		case 6: new_score = 5000; break;
		case 7: new_score = 25000; break;
		case 8: new_score = 100000; break;
		case 9: new_score = 1000000; break;
	}
	score += new_score * combo;

	//Create replacement icons
	Sprite::Info icon_info = {};
	for (uchar i = 0; i < matched_icons.size(); ++i) {
		icon_info.pos = { matched_icons[i]->GetPos().x, 24 - (32 * (i * (matched_icons[0]->GetPos().x == matched_icons[1]->GetPos().x))) };
		icons.push_back(new Icon(icon_info));
	}


	//Mark the matched icons for removal, the icons above them to move down, and create the replacement icons
	for (auto& mi : matched_icons) {
		mi->to_remove = true;

		for (auto& i : icons) {
			//Horizontal match
			if (matched_icons[0]->GetPos().y == matched_icons[1]->GetPos().y and i->GetPos().x == mi->GetPos().x and i->GetPos().y < mi->GetPos().y) {
				i->pos_goal = { i->GetPos().x, i->GetPos().y + 32 };
			}

			//Vertical match
			else if (matched_icons[0]->GetPos().x == matched_icons[1]->GetPos().x and i->GetPos().x == matched_icons[0]->GetPos().x and i->GetPos().y < matched_icons[0]->GetPos().y)
				i->pos_goal = { i->GetPos().x, i->GetPos().y + 32 * (int)matched_icons.size() };
		}
	}
	//Clear out matched_icons
	matched_icons.clear();
}
