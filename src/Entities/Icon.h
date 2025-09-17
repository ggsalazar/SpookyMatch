#pragma once
#include "Entity.h"

class Icon : public Entity {
public:

	Icon(const Sprite::Info& s_info) : Entity(s_info) {}
};