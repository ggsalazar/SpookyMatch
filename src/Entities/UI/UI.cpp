#include "UI.h"
#include "../../Engine/Input.h" //Window
#include "../../Game/Menu.h"

UI::UI(const Sprite::Info& s_i, Menu* m, const UIElem e)
    : Entity(s_i), menu(m), elem(e), label(18) {

    //Label
    string l_str = "";

    switch (elem) {
        case UIElem::Apply:
            l_str = "Apply";
            SetActive(false);
            break;

        case UIElem::Back:
            l_str = "Back";
            break;

        case UIElem::Infinite:
            l_str = "Infinite";
            break;

        case UIElem::Moves:
            l_str = "Moves";
            break;

        case UIElem::Moves_P:
            l_str = "Available Moves:";
            break;

        case UIElem::Options:
            l_str = "Options";
            break;

        case UIElem::Play:
            l_str = "Play!";
            break;

        case UIElem::Quit:
            l_str = "Quit";
            break;

        case UIElem::Resolution:
            l_str = "Resolution";
            break;

        case UIElem::Resume:
            l_str = "Resume";
            break;

        case UIElem::Time:
            l_str = "Time";
            break;

        case UIElem::Time_P:
            l_str = "Available Time:";
            break;

        case UIElem::Title:
            l_str = "Return to Title";
            break;

    }

    label.MoveTo(pos);
    label.SetStr(l_str);
    label.SetOrigin();

    sprite.SetDFC(-10);
}

void UI::GetInput() {
    if (Selected()) { //Selected() accounts for current state of active
        if (Input::BtnPressed(LMB))
            Pressed();

        if (Input::BtnReleased(LMB) and primed)
            Released();
    }
    else if (!Selected()) primed = false;
}

void UI::Draw() {
    Entity::Draw();
    engine->renderer.DrawTxt(label);
}

bool UI::Selected() {
    return active and Collision::RectPoint(bbox, Input::MousePos()) and menu->has_focus;
}

void UI::SetActive(const bool new_active) {
    active = new_active;
    Color new_color = sprite.GetColor();
    new_color.a = active ? 1.f : .5f;
    sprite.SetColor(new_color);
    label.SetColor(new_color);

    if (!active)
        primed = false;
}

void UI::Pressed() {
    primed = true;
}
