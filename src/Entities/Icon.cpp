#include "Icon.h"
#include "../Engine/Input.h"
#include "../Engine/Math/Math.h"

Icon::Icon(const Sprite::Info& i_si) : Entity(i_si) {

	sprite.SetOrigin();

	//Randomly determine type
	type = static_cast<IconType>(rand() % 8);

	//1 in 50 chance of being a special token
	special = !(rand() % 50);
	
	//Could put all of the icons into one sheet but not doing that for clarity reasons
	string new_sheet = "Icons/";
	switch (type) {
		case IconType::Candy:
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
		chosen = game->chosen_icon == this;

		if (selected and Input::BtnPressed(LMB)) {
			if (game->chosen_icon) {
				//Swap with the chosen icon IFF chosen icon adjacent and chosen icon not of the same type
				Vec2i ci_pos = game->chosen_icon->GetPos();
				if (((ci_pos.x == pos.x and ci_pos.y == pos.y + 32) or (ci_pos.x == pos.x and ci_pos.y == pos.y - 32) or (ci_pos.x == pos.x + 32 and ci_pos.y == pos.y) or (ci_pos.x == pos.x - 32 and ci_pos.y == pos.y))
					and type != game->chosen_icon->type and !game->match_made) {

					pos_goal = ci_pos;
					old_pos = pos;
					game->chosen_icon->pos_goal = pos;
					game->chosen_icon->old_pos = game->chosen_icon->pos;
				}
				else {
					game->chosen_icon->chosen = false;
					game->chosen_icon = this;
				}
			}

			else game->chosen_icon = this;
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

		if (pos == pos_goal) {
			pos_goal = { 0 };
			//BOTH of the swapped icons will be calling the function, thereby checking both
			game->CheckSwap(this);
		}
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
