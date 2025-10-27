#include "Slider.h"
#include "../../Engine/Input.h"
#include "../../Engine/Math/Math.h"

Slider::Slider(const Sprite::Info& s_i, Menu* m, const Widget w)
    : UI(s_i, m, w), knob_label(label.GetFontSize()) {

    Sprite::Info knob_info = {}; knob_info.sheet = "UI/Slider_Knob";
    knob_info.scale = sprite.GetScale();
    knob_spr.Init(knob_info);

    label_offset = 15;
    label.MoveTo({ pos.x, pos.y - label_offset });
    label.SetOrigin();

    //Set the origin/anchor, scale, and position
    knob_spr.SetOrigin();
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    //Setting knob position based on appropriate value
    string rounded_val;
    if (widget == Widget::Msc_Vol) {
        knob_pos = ((knob_pos_max - knob_pos_min) * engine->dj.GetVolume() * .005) + knob_pos_min;

        //Set the value
        rounded_val = to_string((knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 200);
        rounded_val = rounded_val.substr(0, rounded_val.find('.') + 3);
    }
    knob_spr.MoveTo({ (int)knob_pos, pos.y });

    knob_label.SetOrigin();
    knob_label.SetStr(rounded_val + '%');
    knob_label.MoveTo({ pos.x, pos.y + label_offset });
}

void Slider::GetInput() {
    UI::GetInput();

    if (Selected() and Input::BtnDown(LMB)) {

        //Adjust knob position
        knob_pos = Input::MousePos().x;
        Math::Clamp(knob_pos, knob_pos_min, knob_pos_max);
        knob_spr.MoveTo({ (int)knob_pos, pos.y });

        float new_val = 0;
        if (widget == Widget::Msc_Vol) {
            new_val = (knob_pos - knob_pos_min) / (knob_pos_max - knob_pos_min) * 200;
            engine->dj.SetVolume(new_val);
        }

        //Set the string
        string r_val = to_string(new_val);
        knob_label.SetStr(r_val.substr(0, r_val.find('.') + 3) + '%');
    }
}

void Slider::Draw() {
    UI::Draw();

    engine->renderer.DrawTxt(label);
    engine->renderer.DrawTxt(knob_label);
    engine->renderer.DrawSprite(knob_spr);
}

void Slider::Move() {
    //Entity::Move() takes care of sprite and bbox

    //Move everything else
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Resize and move the knob
    knob_pos_max = bbox.x + bbox.w * .9f;
    knob_pos_min = bbox.x + bbox.w * .1f;

    knob_pos = knob_pos_min + (engine->dj.GetVolume() * .005 * (knob_pos_max - knob_pos_min));

    knob_spr.MoveTo(Round(knob_pos, pos.y));
    knob_label.MoveTo({ pos.x, pos.y + label_offset });
}
