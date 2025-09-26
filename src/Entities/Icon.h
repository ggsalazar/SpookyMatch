#pragma once
#include "Entity.h"

class Icon : public Entity {
public:
	IconType type;
	bool selected = false, chosen = false, to_remove = false, swapping = false;
	Vec2i pos_goal, old_pos;

	Icon(const Sprite::Info& i_si, const IconType i_type);

	void GetInput() override;
	void Update() override;
	void Draw() override;
};