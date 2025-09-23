#include "Entity.h"

int Entity::SEC = 0;

Entity::Entity(const Sprite::Info s_i)
    : sprite(s_i) {

    Entity::MoveTo(Vec2f(sprite.GetPos()));

    SEC = engine->GetFPS();
}

void Entity::Draw() {
    engine->renderer.DrawSprite(sprite);
}

void Entity::MoveBy(const Vec2f offset) {
    pos += Round(offset);

    Entity::Move();
}

void Entity::MoveTo(const Vec2f new_pos) {
    pos = Round(new_pos);

    Entity::Move();
}

void Entity::Move() {
    sprite.MoveTo(pos);
    size = sprite.GetScaledSize();

    //bbox position will always be top left
    bbox.x = pos.x - floor(sprite.GetOrigin().x * size.x);
    bbox.y = pos.y - floor(sprite.GetOrigin().y * size.y);
    bbox.w = size.x;
    bbox.h = size.y;
}
