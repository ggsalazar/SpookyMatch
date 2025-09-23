#pragma once
#include "Entity.h"

class Icon : public Entity {
public:
	IconType type;
	bool selected = false;
	Icon(const Sprite::Info& i_si, const IconType i_type);

	void GetInput() override;
	void Draw() override;
};