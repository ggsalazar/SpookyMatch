#pragma once
#include "Entity.h"

class Icon : public Entity {
public:
	IconType type;
	bool selected = false;
	Vec2i pos_goal;

	Icon(const Sprite::Info& i_si, const IconType i_type);

	void GetInput() override;
	void Update() override;
	void Draw() override;

	void MoveTo(const Vec2i new_pos) override;
};