#include "Icon.h"
#include "../Engine/Input.h"

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

	else if (Collision::RectPoint(bbox, Input::MousePos()) and Input::BtnPressed(RMB)) {
		//Swap with the selected icon IFF selected icon adjacent - TO-DO
	}
}

void Icon::Draw() {
	Entity::Draw();

	if (selected)
		engine->renderer.DrawRect(bbox, Color(0, 0), Color(0, 1, 0));
}
