#include "Picker.h"
#include "../../Engine/Input.h"

Picker::Picker(const Sprite::Info& s_i, Menu* m, const Widget w)
    : UI(s_i, m, w), picking(label.GetFontSize()) {

    //We want those arrow sprites
    Sprite::Info arrow_info = {};
    arrow_info.sheet = "UI/Picker_Arrow"; arrow_info.origin = {.5f};
    arrow_info.frame_size = {15, 10}; arrow_info.num_frames = 2;
    l_arrow.Init(arrow_info);
    r_arrow.Init(arrow_info);

    label_offset = 6;

    //What exactly ARE we picking?
    string picking_str;
    switch (widget) {
        case Widget::Resolution:
            picking_str = to_string(engine->resolution.x / engine->min_res.x);
        break;

        case Widget::Music_P: {
            Song curr_playing = engine->dj.CurrSong();
            if (curr_playing == Song::Ghoulish_Fun) picking_str = "Ghoulish Fun";
            else if (curr_playing == Song::Spooky_Enchantment) picking_str = "Spooky Enchantment";
            else if (curr_playing == Song::Trick_or_Treat) picking_str = "Trick or Treat";
            else picking_str = "Unknown";
        } break;

        case Widget::Moves_P:
            picking_str = to_string(30);
        break;

        case Widget::Time_P:
            picking_str = to_string(SEC*60*5);
        break;
    }
    picking.SetStr(picking_str);
    picking.SetOrigin();

    //Move shit
    Move();
}

void Picker::GetInput() {
    if (active) {
        if (LeftSelected()) {
            if (Input::BtnPressed(LMB))
                LeftPressed();
            if (Input::BtnReleased(LMB) and l_primed)
                LeftReleased();
        }
        else if (!LeftSelected()) l_primed = false;

        if (RightSelected()) {
            if (Input::BtnPressed(LMB))
                RightPressed();
            if (Input::BtnReleased(LMB) and r_primed)
                RightReleased();
        }
        else if (!RightSelected()) r_primed = false;

        //Pulling double duty as a button
        if (Selected() and !LeftSelected() and !RightSelected()) {
            if (Input::BtnPressed(LMB))
                Pressed();

            if (Input::BtnReleased(LMB) and primed)
                Released();
        }
        else if (!Selected()) primed = false;
    }
}

void Picker::Update() {
    //Change the current frame of the arrows
    l_arrow.SetCurrFrame(LeftSelected());
    r_arrow.SetCurrFrame(RightSelected());
    //And ourself
    sprite.SetCurrFrame(Selected() + primed);

    if (widget == Widget::Music_P) {
        Song curr_playing = engine->dj.CurrSong();
        if (curr_playing == Song::Ghoulish_Fun) picking.SetStr("Ghoulish Fun");
        else if (curr_playing == Song::Spooky_Enchantment) picking.SetStr("Spooky Enchantment");
        else if (curr_playing == Song::Trick_or_Treat) picking.SetStr("Trick or Treat");
        else picking.SetStr("Unknown");
    }
}

void Picker::Draw() {
    UI::Draw();

    //Draw the label
    if (sprite.GetSheet() == "UI/Btn_Blank")
        engine->renderer.DrawTxt(label);

    //Draw the arrows
    engine->renderer.DrawSprite(l_arrow);
    engine->renderer.DrawSprite(r_arrow);

    //Gotta convert the time being picked into seconds
    if (widget == Widget::Time_P) {
        uint true_time = stoi(picking.GetStr());
        uint time_secs = true_time / SEC;
        picking.SetStr(to_string(time_secs));
        engine->renderer.DrawTxt(picking);
        picking.SetStr(to_string(true_time));
    }
    else engine->renderer.DrawTxt(picking);

    /*if (LeftSelected())
        engine->renderer.DrawRect(l_bbox, Color(1, 0, 0, .5)); //Red, 50% opacity
    else if (RightSelected())
        engine->renderer.DrawRect(r_bbox, Color(0, 0, 1, .5)); //Blue, 50% opacity
    */
}

void Picker::Move() {
    //Entity::Move() takes care of sprite & bbox

    //Move everything else

    //l/r bboxes
    //Set up left bbox
    l_bbox.x = bbox.x + round(bbox.w * .025);
    l_bbox.y = bbox.y + round(bbox.h * .15);
    l_bbox.w = round(bbox.w * .2);
    l_bbox.h = round(bbox.h * .75);
    l_arrow.SetScale({-1, 1});
    l_arrow.MoveTo(Round(l_bbox.x + l_bbox.w*.5, l_bbox.y + l_bbox.h*.5));

    //Right bbox
    r_bbox.x = bbox.x + round(bbox.w * .775);
    r_bbox.y = l_bbox.y;
    r_bbox.w = l_bbox.w;
    r_bbox.h = l_bbox.h;
    r_arrow.MoveTo(Round(r_bbox.x + r_bbox.w*.5, r_bbox.y + r_bbox.h*.5));

    label.MoveTo({ pos.x, pos.y - label_offset });
    picking.MoveTo({pos.x, pos.y + label_offset});
}

void Picker::SetPicking(const string& new_p) {
    picking.SetStr(new_p);
}

bool Picker::LeftSelected() {
    return active and Collision::RectPoint(l_bbox, Input::MousePos());
}

void Picker::LeftReleased() {
    uint curr_picking = widget != Widget::Music_P ? stoi(picking.GetStr()) : 0;

    switch (widget) {
        case Widget::Resolution:
            if (--curr_picking < 1)
                curr_picking = min(floor(engine->window.ScreenSize().x / engine->min_res.x), floor(engine->window.ScreenSize().y / engine->min_res.y));

            //Set the Apply button to active
            menu->SetWidgetActive(Widget::Apply);
        break;

        case Widget::Music_P:
            if (picking.GetStr() == "Ghoulish Fun") picking.SetStr("Trick or Treat");
            else if (picking.GetStr() == "Spooky Enchantment") picking.SetStr("Ghoulish Fun");
            else if (picking.GetStr() == "Trick or Treat") picking.SetStr("Spooky Enchantment");
        break;

        case Widget::Moves_P:
            curr_picking -= 5;
            curr_picking = curr_picking <= 0 ? 100 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;

        case Widget::Time_P:
            curr_picking -= 30 * SEC;
            curr_picking = curr_picking <= 0 ? SEC * 60 * 15 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;
    }

    if (widget != Widget::Music_P)
        picking.SetStr(to_string(curr_picking));
}

bool Picker::RightSelected() { 
    return active and Collision::RectPoint(r_bbox, Input::MousePos());
}

void Picker::RightReleased() {
    uint curr_picking = widget != Widget::Music_P ? stoi(picking.GetStr()) : 0;

    switch (widget) {
        case Widget::Resolution:
            if (++curr_picking > min(floor(engine->window.ScreenSize().x / engine->min_res.x), floor(engine->window.ScreenSize().y/engine->min_res.y)))
                curr_picking = 1;

            //Set the Apply button to active
            menu->SetWidgetActive(Widget::Apply);
        break;

        case Widget::Music_P:
            if (picking.GetStr() == "Ghoulish Fun") picking.SetStr("Spooky Enchantment");
            else if (picking.GetStr() == "Spooky Enchantment") picking.SetStr("Trick or Treat");
            else if (picking.GetStr() == "Trick or Treat") picking.SetStr("Ghoulish Fun");
        break;

        case Widget::Moves_P:
            curr_picking += 5;
            curr_picking = curr_picking > 100 ? 5 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;

        case Widget::Time_P:
            curr_picking += 30 * SEC;
            curr_picking = curr_picking > SEC * 60 * 15 ? SEC * 30 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;
    }

    if (widget != Widget::Music_P)
        picking.SetStr(to_string(curr_picking));
}

void Picker::Released() {
    primed = false;

    switch (widget) {
        case Widget::Music_P:
            if (picking.GetStr() == "Ghoulish Fun") engine->dj.PlaySong(Song::Ghoulish_Fun);
            else if (picking.GetStr() == "Spooky Enchantment") engine->dj.PlaySong(Song::Spooky_Enchantment);
            else if (picking.GetStr() == "Trick or Treat") engine->dj.PlaySong(Song::Trick_or_Treat);
        break;

        case Widget::Moves_P:
            game->moves_remaining = stoi(picking.GetStr());
            game->high_score = game->high_scores["Moves"][picking.GetStr()];
            game->gm_mode = GameMode::Moves;
            game->ChangeScene(Scene::Game);
        break;

        case Widget::Time_P:
            game->time_remaining = stoi(picking.GetStr());
            game->high_score = game->high_scores["Time"][to_string(stoi(picking.GetStr())/SEC)];
            game->gm_mode = GameMode::Time;
            game->ChangeScene(Scene::Game);
        break;
    }
}
