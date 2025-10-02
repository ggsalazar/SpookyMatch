#pragma once
#include "../Entity.h" //Collision (Geometry (Vec2 (iostream))), Enums, Sprite
#include "../../Engine/Graphics/Text.h"

class Menu;

class UI : public Entity {
public:
    Menu* menu = nullptr;
    Text label;

    UI(const Sprite::Info& s_i = {}, Menu* m = nullptr, const UIElem e = UIElem::NONE);

    virtual void GetInput() override;
    virtual void Draw() override;

    bool Selected();

    void SetActive(const bool new_active = true);
    inline bool GetActive() const { return active; }
    inline bool GetActivated() const { return activated; }
    inline UIElem GetElem() const { return elem; }

protected:
    //Variables
    int label_offset = 0;
    UIElem elem = UIElem::NONE;
    bool active = true;
    bool primed = false;
    bool activated = false;

    //Functions
    virtual void Pressed();
    virtual void Released() {}
};