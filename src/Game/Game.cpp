#include <fstream>
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

    //Initialize the cursor sprite
    Sprite::Info spr_info = {};
    spr_info.sheet = "UI/Cursor"; spr_info.frame_size = {10, 14};
    cursor.Init(spr_info);

    //Initialize the game board sprite
    spr_info = Sprite::Info{};
    spr_info.sheet = "UI/Game_Board";
    game_board.Init(spr_info);

	Text::Info t_info = {};
	t_info.font_size = 36; t_info.str = "Score: "; t_info.pos = { 2, -2 };
	score_txt = new Text(t_info);
	t_info.str = "Combo: "; t_info.origin.x = 1; t_info.pos.x = 398;
	combo_txt = new Text(t_info);
	t_info.pos.y = 374;
	remaining_txt = new Text(t_info);
	//matching stuff
	t_info.origin.x = .0;
	t_info.str = "MATCH MADE!!!";
	t_info.pos = { score_txt->GetPos().x, remaining_txt->GetPos().y };
	match_txt = new Text(t_info);

	//Init text displaying high scores
	t_info.origin.x = .5;
	t_info.str = "";
	t_info.pos = { 0 };
	t_info.font_size = 24;
	for (uchar i=0; i < 3; ++i)
		high_score_txts[i] = new Text(t_info);

	//Load/Init high scores
	ifstream in_file("high_scores.json");
	if (in_file.good())
		in_file >> high_scores;
	else {
		for (uint i = 5; i <= 100; i+=5)
			high_scores["Moves"][to_string(i)] = 0;
		for (uint i = 0; i <= 900; i += 30)
			high_scores["Time"][to_string(i)] = 0;
		high_scores["Infinite"] = 0;
	}
}

void Game::ChangeScene(Scene new_scn) {
	curr_scn = new_scn;

	//Wipe the slate clean
	for (auto& m : menus)
		m->Open(false);
	menus.clear();
	icons.clear();
	score = combo = 0;
	paused = false;
	match_timer = match_timer_max;

	switch (curr_scn) {
		case Scene::Title:
			menus.push_back(new Menu(MenuName::Main));
			FindMenu(MenuName::Main)->Open();
			menus.push_back(new Menu(MenuName::Options));
			menus.push_back(new Menu(MenuName::Choose_Game));

			high_score = 0;
			moves_remaining = 0;
			time_remaining = 0;
		break;

		case Scene::Game:
			//First add the pause and game over menus
			menus.push_back(new Menu(MenuName::Options_I));
			menus.push_back(new Menu(MenuName::GO));


			if (gm_mode == GameMode::Moves) max_moves_remaining = moves_remaining;
			else if (gm_mode == GameMode::Time) max_time_remaining = time_remaining;


			//Next we gotta add in our icons
			Sprite::Info icon_info = {};
			for (uchar row = 0; row <= 9; ++row) {
				for (uchar col = 0; col <= 9; ++col) {
					icon_info.pos = { 43 + col * icon_gap, 43 + row * icon_gap };

					icons.push_back(new Icon(icon_info));
				}
			}
		break;
	}
}

void Game::GetInput() {

	//Update cursor position and frame
	cursor.MoveTo(Input::MousePos());
	cursor.SetCurrFrame(Input::BtnDown(LMB));

	//Input for the icons
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
	//Update icons, interface buttons, & menus
	for (auto& i : icons) {
		i->Update();

		if (i->GetPos() == i->pos_goal) CheckSwap(i);
	}

	//If there was no match made, then the chosen icons need to swap back to their original positions
	if (!match_made and swap_back and chosen_icons[0] and chosen_icons[1]) {
		move_buffer = move_buffer_max;
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

	//Are we waiting for icons to move into place?
	move_buffer -= (move_buffer != 0 and !paused);

	//Texts
	score_txt->SetStr("Score: " + to_string(score) + "|" + to_string(high_score));
	combo_txt->SetStr("Combo: " + to_string(combo) + "x|" + to_string(max_combo) + 'x');
	if (gm_mode == GameMode::Moves)
		remaining_txt->SetStr("Moves: " + to_string(moves_remaining));
	else if (gm_mode == GameMode::Time) {
		remaining_txt->SetStr("Time: " + to_string((int)ceil(time_remaining / Entity::SEC)));
		//Count down time
		time_remaining -= (!paused and !match_made);
	}

	//End the game
	if (((gm_mode == GameMode::Moves and moves_remaining <= 0) or (gm_mode == GameMode::Time and time_remaining <= 0))
		and !match_made and !paused and !move_buffer) {

		paused = true;
		chosen_icons[0] = chosen_icons[1] = nullptr;
		OpenMenu(MenuName::GO);
	}


	//Get high scores
	if (curr_scn == Scene::Title and FindMenu(MenuName::Choose_Game)->GetOpen()) {
		Menu* m = FindMenu(MenuName::Choose_Game);
		high_score_txts[0]->MoveTo({m->GetWidgetPos(Widget::Moves_P).x, m->GetWidgetPos(Widget::Moves_P).y - 40});
		high_score_txts[1]->MoveTo({m->GetWidgetPos(Widget::Time_P).x, m->GetWidgetPos(Widget::Time_P).y - 40});
		high_score_txts[2]->MoveTo({m->GetWidgetPos(Widget::Infinite).x, m->GetWidgetPos(Widget::Infinite).y + ((int)high_score_txts[2]->GetFontSize() - 8)});

		high_score_txts[0]->SetStr("High Score: " + to_string(high_scores["Moves"][m->GetWidgetStatus(Widget::Moves_P)]));
		high_score_txts[1]->SetStr("High Score: " + to_string(high_scores["Time"][to_string(stoi(m->GetWidgetStatus(Widget::Time_P)) / Entity::SEC)]));
		high_score_txts[2]->SetStr("High Score: " + to_string(high_scores["Infinite"]));
	}

	if (engine->GetGameFrames() % 10 == 0) {
		//Sort the icons vector by dfc value every 6th of a second (every 10 game frames) so that icons of a lower dfc value are drawn
		// last (closest to the camera)
		sort(icons.begin(), icons.end(),
			[](const Icon* a, const Icon* b) { return a->sprite.GetDFC() > b->sprite.GetDFC(); });
	}

	//Remove dead icons walking
	for (auto it = icons.begin(); it != icons.end();) {
		Icon* i = *it;
		if (i->expired) it = icons.erase(it);
		else ++it;
	}
}

void Game::Draw() {
	//If we're playing the game draw the game board
	if (curr_scn == Scene::Game) engine->renderer.DrawSprite(game_board);

	//Entities
	for (auto& i : icons) i->Draw();
}

void Game::DrawGUI() {
	for (auto& i : icons) i->DrawGUI();

	//Draw the score & combo
	if (curr_scn == Scene::Game) {
		engine->renderer.DrawTxt(*score_txt);
		engine->renderer.DrawTxt(*combo_txt);
		if (match_made) engine->renderer.DrawTxt(*match_txt);
		if (gm_mode != GameMode::Infinite) engine->renderer.DrawTxt(*remaining_txt);
	}
	//Display high scores
	else if (curr_scn == Scene::Title and FindMenu(MenuName::Choose_Game)->GetOpen())
		for (uchar i = 0; i < 3; ++i) engine->renderer.DrawTxt(*high_score_txts[i]);

	//Menus are drawn last since they will always be closest to the camera
	for (const auto& m : menus)
		m->Draw();

	//JK lol the cursor is drawn last
	engine->renderer.DrawSprite(cursor);
}

void Game::Resize() {
	//Reset the fonts of the texts
	score_txt->SetFont();
	combo_txt->SetFont();
	match_txt->SetFont();
	remaining_txt->SetFont();
	for (auto& s : high_score_txts) s->SetFont();

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
			check_pos.x = icon->GetPos().x - icon_gap * i;
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
			check_pos.x = icon->GetPos().x + icon_gap * i;
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
			check_pos.y = icon->GetPos().y - icon_gap * i;
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
			check_pos.y = icon->GetPos().y + icon_gap * i;
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
	move_buffer = move_buffer_max;
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
	//Special icons/mark matched_icons as expired
	vector<Icon*> remove_chain;
	for (auto& mi : matched_icons) {
		mi->expired = true;

		//Special effects
		if (mi->special) {
			switch (mi->type) {
				case IconType::Ghost:
					//Add moves/time/points
					new_score += 250 * (gm_mode == GameMode::Infinite);
					moves_remaining += 5 * (gm_mode == GameMode::Moves);
					time_remaining += 30 * Entity::SEC * (gm_mode == GameMode::Time);
				break;

				case IconType::Pumpkin:
					/*
					//Explode
					Vec2i pos_to_check;
					for (char i = -1; i <= 1; ++i) {
						pos_to_check.x = mi->GetPos().x + icon_gap * i;
						for (char j = -1; j <= 1; ++j) {
							pos_to_check.y = mi->GetPos().y + icon_gap * i;
							//Add icons to remove_chain and mark as matched!
							for (auto& ic : icons) {
								if (!ic->matched and ic->GetPos() == pos_to_check) {
									ic->matched = true;
									remove_chain.push_back(ic);
								}
							}

						}
					}
					*/
				break;
			}
		}
	}
	score += new_score * combo;
	//Save the score for the given max time or moves remaining
	if (score > high_score) {
		high_score = score;
		if (gm_mode == GameMode::Moves) high_scores["Moves"][to_string(max_moves_remaining)] = score;
		else if (gm_mode == GameMode::Time) high_scores["Time"][to_string(max_time_remaining / Entity::SEC)] = score;
		else high_scores["Infinite"] = score;

		ofstream out_file("high_scores.json");
		out_file << high_scores.dump(4) << '\n';
		out_file.close();
	}

	//Move icons down by counting the amount of icons to be removed in a given column
	uchar num_expired = 0;
	int x_to_check, highest_y = 400;
	vector<Icon*> icons_to_shift;
	for (uchar col = 0; col < 10; ++col) {
		num_expired = 0;
		x_to_check = 43 + icon_gap * col;
		icons_to_shift.clear();
		highest_y = 400;
		for (auto& mi : matched_icons)
			if (mi->GetPos().x == x_to_check) highest_y = mi->GetPos().y < highest_y ? mi->GetPos().y : highest_y;

		for (auto& i : icons) {
			if (i->GetPos().x == x_to_check) {
				if (i->GetPos().y < highest_y) icons_to_shift.push_back(i);
				else if (i->expired) ++num_expired;
			}
		}

		if (num_expired) {
			//Create the replacement icons
			Sprite::Info icon_info = {};
			for (uchar i = 0; i < num_expired; ++i) {
				icon_info.pos = { x_to_check, 43 - icon_gap * (i+1) };
				icons.push_back(new Icon(icon_info));
				icons_to_shift.push_back(icons[icons.size() - 1]);
			}

			//Set the position goals of the icons to shift
			for (auto& i : icons_to_shift)
				i->pos_goal = { i->GetPos().x, i->GetPos().y + icon_gap * num_expired };
		}
	}

	//Clear out matched_icons
	matched_icons.clear();

	if (!remove_chain.empty()) {
		matched_icons = remove_chain;
		RemoveIcons();
	}
}
