#include "Icon.h"
#include "../Engine/Engine.h" //Game
#include "../Engine/Input.h"
#include "../Engine/Math/Math.h"

Icon::Icon(const Sprite::Info& i_si) : Entity(i_si) {

	sprite.SetOrigin();

	//Randomly determine type
	type = static_cast<IconType>(rand() % 8);

	//1 in 50 chance of being a special token
	special = !(rand() % 50);
	if (type == IconType::Pumpkin) special = !(rand() % 4);
	
	//Could put all of the icons into one sheet but not doing that for clarity reasons
	string new_sheet = "Icons/";
	switch (type) {
		case IconType::Bat:
			new_sheet += "Candy";
		break;
	
		case IconType::Ghost:
			new_sheet += "Ghost";
		break;
	
		case IconType::Mummy:
			new_sheet += "Mummy";
		break;

		case IconType::Pumpkin:
			new_sheet += "Pumpkin";
			break;

		case IconType::Skeleton:
			new_sheet += "Skeleton";
			break;

		case IconType::Spider:
			new_sheet += "Spider";
			break;

		case IconType::Witch:
			new_sheet += "Witch";
			break;

		case IconType::Zombie:
			new_sheet += "Zombie";
			break;
	}
	sprite.SetSheet(new_sheet);
	Entity::Move();
}

void Icon::GetInput() {
	if (!game->paused) {
		selected = Collision::RectPoint(bbox, Input::MousePos());
		chosen = game->chosen_icons[0] == this;

		if (selected and Input::BtnPressed(LMB)) {
			if (game->chosen_icons[0]) {
				//Swap with the chosen icon IFF chosen icon adjacent and chosen icon not of the same type
				Vec2i ci_pos = game->chosen_icons[0]->GetPos();
				if (((ci_pos.x == pos.x and ci_pos.y == pos.y + 32) or (ci_pos.x == pos.x and ci_pos.y == pos.y - 32) or (ci_pos.x == pos.x + 32 and ci_pos.y == pos.y) or (ci_pos.x == pos.x - 32 and ci_pos.y == pos.y))
					and type != game->chosen_icons[0]->type and !game->match_made) {

					//If we're swapping, gotta reset the combo
					game->combo = 0;

					//Decrement moves remaining if appropriate
					game->moves_remaining -= game->gm_mode == GameMode::Moves;

					//Start the move_buffer
					game->move_buffer = game->move_buffer_max;

					pos_goal = ci_pos;
					old_pos = pos;
					game->chosen_icons[1] = this;

					game->chosen_icons[0]->pos_goal = pos;
					game->chosen_icons[0]->old_pos = ci_pos;
				}
				else {
					game->chosen_icons[0]->chosen = false;
					game->chosen_icons[0] = this;
				}
			}

			else game->chosen_icons[0] = this;
		}
	}
}

void Icon::Update() {
	Entity::Update();

	//Move to our goal
	if (pos_goal != Vec2i(0)) {
		Vec2i new_pos = Round(Math::Lerp(Vec2f(pos), Vec2f(pos_goal), .142));
		//Moving up/down
		if (pos_goal.x == pos.x and new_pos.y == pos.y) {
			//Up
			new_pos.y -= pos_goal.y < pos.y;
			//Down
			new_pos.y += pos_goal.y > pos.y;
		}
		//Moving l/r
		else if (pos_goal.y == pos.y and new_pos.x == pos.x) {
			//left
			new_pos.x -= pos_goal.x < pos.x;
			//right
			new_pos.x += pos_goal.x > pos.x;
		}

		MoveTo(new_pos);
	}
}

void Icon::Draw() {
	//Only draw if on the grid
	if (Collision::AABB(bbox, Rect({ 40 }, 320))) {
		Entity::Draw();

		engine->renderer.DrawRect(Rect({ bbox.x, bbox.y }, { bbox.w + 1, bbox.h + 1 }), Color(0, 0), Color(selected, chosen, special, (selected or chosen or special)));

		engine->renderer.DrawRect(Rect({ bbox.x + 1, bbox.y + 1 }, { bbox.w - 1, bbox.h - 1 }), Color(0, 0), Color(1, 0, 0, matched));
	}
}
