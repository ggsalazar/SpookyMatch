#include "Icon.h"
#include "../Engine/Input.h"
#include "../Engine/Math/Math.h"

Icon::Icon(const Sprite::Info& i_si, const IconType i_type) :
	Entity(i_si), type(i_type) {

	//CONSIDER PUTTING ALL OF THE ICONS INTO ONE SHEET
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
	++bbox.w;
	++bbox.h;
}

void Icon::GetInput() {
	if (Collision::RectPoint(bbox, Input::MousePos()) and Input::BtnPressed(LMB)) {
		if (game->selected_icon)
			game->selected_icon->selected = false;
		game->selected_icon = this;
		selected = true;
	}

	else if (Collision::RectPoint(bbox, Input::MousePos()) and Input::BtnPressed(RMB) and game->selected_icon) {
		//Swap with the selected icon IFF selected icon adjacent
		Vec2i si_pos = game->selected_icon->GetPos();
		if ((si_pos.x == pos.x and si_pos.y == pos.y + 32) or (si_pos.x == pos.x and si_pos.y == pos.y - 32) or (si_pos.x == pos.x + 32 and si_pos.y == pos.y) or (si_pos.x == pos.x - 32 and si_pos.y == pos.y)) {
			pos_goal = si_pos;
			game->selected_icon->pos_goal = pos;
		}

		game->selected_icon->selected = false;
		game->selected_icon = nullptr;
	}
}

void Icon::Update() {
	Entity::Update();

	//Move to our goal
	if (pos_goal != Vec2i(0)) {
		Vec2i new_pos = Round(Math::Lerp(Vec2f(pos), Vec2f(pos_goal), .15));
		//Moving up/down
		if (pos_goal.x == pos.x and new_pos.y == pos.y) {
			//Up
			if (pos_goal.y < pos.y) --new_pos.y;
			//Down
			else if (pos_goal.y > pos.y) ++new_pos.y;
		}
		//Moving l/r
		else if (pos_goal.y == pos.y and new_pos.x == pos.x) {
			//left
			if (pos_goal.x < pos.x) --new_pos.x;
			//right
			else if (pos_goal.x > pos.x) ++new_pos.x;
		}

		MoveTo(new_pos);

		if (pos == pos_goal) {
			pos_goal = { 0 };
			//This works because BOTH of the swapped icons will be calling the function, thereby checking both
			game->CheckSwap(this);
		}
	}
}

void Icon::Draw() {
	Entity::Draw();

	if (selected)
		engine->renderer.DrawRect(bbox, Color(0, 0), Color(0, 1, 0));
}

void Icon::MoveTo(const Vec2i new_pos) {
	Entity::MoveTo(new_pos);

	++bbox.w; ++bbox.h;
}
