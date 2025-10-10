#pragma once
#include "Entity.h"

class Icon : public Entity {
public:
	IconType type;
	bool special = false, selected = false, chosen = false, matched = false;
	Vec2i pos_goal, old_pos;

	Icon(const Sprite::Info& i_si);

	void GetInput() override;
	void Update() override;
	void Draw() override;
};
