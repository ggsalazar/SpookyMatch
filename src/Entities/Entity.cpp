#include "Entity.h"
#include "../Engine/Engine.h" //Game

//Just trust
Engine* Entity::engine = nullptr;
Game* Entity::game = nullptr;

Entity::Entity(const Sprite::Info s_i) : sprite(s_i) {
    Entity::MoveTo(Vec2f(sprite.GetPos()));
}

void Entity::SetEngine(Engine* e, Game* g) {
    engine = e;
    game = g;
    SEC = engine->GetFPS();
}

void Entity::Draw() {
    sprite.Draw();
}

void Entity::MoveBy(const Vec2f offset) {
    pos += Round(offset);

    Entity::Move();
}
void Entity::MoveBy(const Vec2i offset) {
	pos += offset;

	Entity::Move();
}

void Entity::MoveTo(const Vec2f new_pos) {
    pos = Round(new_pos);

    Entity::Move();
}
void Entity::MoveTo(const Vec2i new_pos) {
    pos = new_pos;

    Entity::Move();
}

void Entity::Move() {
    sprite.MoveTo(pos);
    size = sprite.GetScaledSize();

    //bbox position will always be top left
    bbox.x = pos.x - floor(sprite.GetOrigin().x * size.x);
    bbox.y = pos.y - floor(sprite.GetOrigin().y * size.y);
    bbox.w = size.x-1;
    bbox.h = size.y-1;
}