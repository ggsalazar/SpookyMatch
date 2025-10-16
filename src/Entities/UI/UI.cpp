#include "UI.h"
#include "../../Engine/Input.h" //Window

UI::UI(const Sprite::Info& s_i, Menu* m, const Widget w)
    : Entity(s_i), menu(m),  label(18), widget(w) {

    //Label
    string l_str;
    Sprite::Info info = {}; info.sheet = "UI/Btn_Blank";
    info.pos = pos; info.origin = sprite.GetOrigin();
    info.frame_size = {112, 33}; info.num_frames = 3;
    info.anim_fps = 0; info.dfc = -10;

    switch (widget) {
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

        case Widget::Moves_P:
            l_str = "Moves";
            break;

        case Widget::Msc_Vol:
            l_str = "Music Volume";
            break;

        case Widget::Music_P:
            l_str = "Song";
            break;

        case Widget::Options:
            l_str = "Options";
            info.sheet = "UI/Settings_Btn";
            break;

        case Widget::Play:
            l_str = "Play!";
            info.sheet = "UI/Play_Btn";
            break;

        case Widget::Quit:
            l_str = "Quit";
            info.sheet = "UI/Quit_Btn";
            break;

        case Widget::Resolution:
            l_str = "Resolution";
            break;

        case Widget::Resume:
            l_str = "Resume";
            info.sheet = "UI/Resume_Btn";
            break;

        case Widget::Time_P:
            l_str = "Time";
            break;

        case Widget::Title:
            l_str = "Return to Title";
            info.sheet = "UI/Title_Btn";
            break;

    }

    sprite.Init(info);

    //Set bbox size
    Entity::Move();

    label.MoveTo({pos.x, pos.y});
    label.SetStr(l_str);
    label.SetOrigin();
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
