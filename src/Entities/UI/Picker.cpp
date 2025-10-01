#include "Picker.h"

Picker::Picker(const Sprite::Info& s_i, Menu* m, const UIElem e)
    : UI(s_i, m, e), picking(label.GetFontSize()) {

    label_offset = 6;
    label.MoveTo({ pos.x, pos.y - label_offset });

    //Set up bbox
    l_bbox.x = bbox.x + bbox.w * .05;
    l_bbox.y = bbox.y + bbox.h * .1;
    l_bbox.w = bbox.w * .25;
    l_bbox.h = bbox.h * .75;

    //Right bbox
    r_bbox.x = bbox.x + bbox.w * .7;
    r_bbox.y = bbox.y + bbox.h * .1;
    r_bbox.w = bbox.w * .25;
    r_bbox.h = bbox.h * .75;

    //What exactly ARE we picking?
    string picking_str = "";
    switch (elem) {
        case UIElem::Resolution:
            picking_str = to_string(engine->resolution.x / engine->min_res.x);
        break;

        case UIElem::Moves_P:
            picking_str = to_string(30);
        break;

        case UIElem::Time_P:
            picking_str = to_string(SEC*60*5);
        break;
    }
    picking.SetStr(picking_str);
    picking.SetOrigin();
    picking.MoveTo({ pos.x, pos.y + 2 });
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
    }
}

void Picker::Draw() {
    if (Selected())
        engine->renderer.DrawRect(bbox, Color(0, 1, 0));

    UI::Draw();
    
    //Gotta convert the time being picked into seconds
    if (elem == UIElem::Time_P) {
        uint true_time = stoi(picking.GetStr());
        uint time_secs = true_time / SEC;
        picking.SetStr(to_string(time_secs));
        engine->renderer.DrawTxt(picking);
        picking.SetStr(to_string(true_time));
    }
    else engine->renderer.DrawTxt(picking);

    if (LeftSelected())
        engine->renderer.DrawRect(l_bbox, Color(1, 0, 0, .5)); //Red, 50% opacity
    else if (RightSelected())
        engine->renderer.DrawRect(r_bbox, Color(0, 0, 1, .5)); //Blue, 50% opacity
}

void Picker::Move() {
    //Entity::Move() is called in MoveBy/MoveTo takes care of sprite & bbox

    //Move everything else
    //l/r bboxes
    l_bbox.x = bbox.x + bbox.w * .05;
    l_bbox.y = bbox.y + bbox.h * .1;

    r_bbox.x = bbox.x + bbox.w * .7;
    r_bbox.y = l_bbox.y;

    label.MoveTo({ pos.x, pos.y - label_offset });

    picking.MoveTo(pos);
}

void Picker::SetPicking(const string new_p) {
    picking.SetStr(new_p);
}

void Picker::LeftReleased() {
    uint curr_picking = stoi(picking.GetStr());

    switch (elem) {
        case UIElem::Resolution:
            if (--curr_picking < 1)
                curr_picking = min(floor(engine->window.ScreenSize().x / engine->min_res.x), floor(engine->window.ScreenSize().y / engine->min_res.y));

            //Set the Apply button to active
            menu->SetUIElemActive(UIElem::Apply);
        break;

        case UIElem::Moves_P:
            curr_picking -= 5;
            curr_picking = curr_picking <= 0 ? 100 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;

        case UIElem::Time_P:
            curr_picking -= 30 * SEC;
            curr_picking = curr_picking <= 0 ? SEC * 60 * 15 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;
    }

    picking.SetStr(to_string(curr_picking));
}

void Picker::RightReleased() {
    uint curr_picking = stoi(picking.GetStr());

    switch (elem) {
        case UIElem::Resolution:
            if (++curr_picking > min(floor(engine->window.ScreenSize().x / engine->min_res.x), floor(engine->window.ScreenSize().y/engine->min_res.y)))
                curr_picking = 1;

            //Set the Apply button to active
            menu->SetUIElemActive(UIElem::Apply);
        break;

        case UIElem::Moves_P:
            curr_picking += 5;
            curr_picking = curr_picking > 100 ? 5 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
            break;

        case UIElem::Time_P:
            curr_picking += 30 * SEC;
            curr_picking = curr_picking > SEC * 60 * 15 ? SEC * 30 : curr_picking;
            if (game->high_scores.contains(to_string(curr_picking)))
                game->high_score = game->high_scores[to_string(curr_picking)];
            else game->high_score = 0;
        break;
    }

    picking.SetStr(to_string(curr_picking));
}
