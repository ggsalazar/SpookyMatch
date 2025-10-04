#include "UI.h"
#include "../../Engine/Input.h" //Window
#include "../../Game/Menu.h"

UI::UI(const Sprite::Info& s_i, Menu* m, const Widget e)
    : Entity(s_i), menu(m), elem(e), label(18) {

    //Label
    string l_str = "";

    switch (elem) {
        case Widget::Apply:
            l_str = "Apply";
            SetActive(false);
            break;

        case Widget::Back:
            l_str = "Back";
            break;

        case Widget::Infinite:
            l_str = "Infinite";
            break;

        case Widget::Moves:
            l_str = "Moves";
            break;

        case Widget::Moves_P:
            l_str = "Available Moves:";
            break;

        case Widget::Options:
            l_str = "Options";
            break;

        case Widget::Play:
            l_str = "Play!";
            break;

        case Widget::Quit:
            l_str = "Quit";
            break;

        case Widget::Resolution:
            l_str = "Resolution";
            break;

        case Widget::Resume:
            l_str = "Resume";
            break;

        case Widget::Time:
            l_str = "Time";
            break;

        case Widget::Time_P:
            l_str = "Available Time:";
            break;

        case Widget::Title:
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
