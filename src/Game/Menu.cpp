#include "Menu.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Picker.h"

Menu::Menu(const MenuName i_name) : name(i_name), menu_text(42), sup_text(30) {

    //Menu and Supp text variables
    menu_text.SetOrigin();
    Vec2i m_t_pos = { 0 };
    string m_t_str = "MENU DEFAULT";

    sup_text.SetOrigin();
    Vec2i s_t_pos = { 0 };
    string s_t_str = "";
    uint s_t_str_max_w = -1;

    Sprite::Info elem_info = {}; elem_info.origin = ui_ori;
    int e_y_buffer = 0;
    //What we do depends on our name
    switch (name) {
        case MenuName::Main: {
            m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "SpookyMatch!";

            elem_info.sheet = "UI/Button";
            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .4f);

            e_y_buffer = round(engine->min_res.y * .1f);

            ui_elems.insert({ UIElem::Play, new Button(elem_info, this, UIElem::Play) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Options, new Button(elem_info, this, UIElem::Options) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Quit, new Button(elem_info, this, UIElem::Quit) });

            break;
        }

        case MenuName::Choose_Game: {
            m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "Select Game Mode";

            elem_info.pos = Round(engine->min_res.x * .3f, engine->min_res.y * .4f);
            elem_info.sheet = "UI/Button";
            
            ui_elems.insert({ UIElem::Time, new Button(elem_info, this, UIElem::Time) });
            elem_info.pos.y = engine->min_res.y * .47f;
            ui_elems.insert({ UIElem::Time_P, new Picker(elem_info, this, UIElem::Time_P) });


            elem_info.pos = Round(engine->min_res.x * .7f, engine->min_res.y * .4f);
            ui_elems.insert({ UIElem::Moves, new Button(elem_info, this, UIElem::Moves) });
            elem_info.pos.y = engine->min_res.y * .47f;
            ui_elems.insert({ UIElem::Moves_P, new Picker(elem_info, this, UIElem::Moves_P) });

            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .55f);
            ui_elems.insert({ UIElem::Infinite, new Button(elem_info, this, UIElem::Infinite) });

            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .8f);
            ui_elems.insert({ UIElem::Back, new Button(elem_info, this, UIElem::Back) });

            break;
        }

        case MenuName::GO: {
            m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "Game Over!";

            elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .3f);

            elem_info.sheet = "UI/Button";
            ui_elems.insert({ UIElem::Title, new Button(elem_info, this, UIElem::Title) });

            elem_info.pos.y = engine->min_res.y * .5f;
            ui_elems.insert({ UIElem::Quit, new Button(elem_info, this, UIElem::Quit) });
            break;
        }
        
        case MenuName::Options: {
            m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "Settings";

            elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .3f);
            e_y_buffer = round(engine->min_res.y * .1f);
            elem_info.sheet = "UI/Button";

            ui_elems.insert({ UIElem::Resolution, new Picker(elem_info, this, UIElem::Resolution) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Apply, new Button(elem_info, this, UIElem::Apply) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Back, new Button(elem_info, this, UIElem::Back) });

            break;
        }

        case MenuName::Options_I: {
            m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .12f); m_t_str = "Settings";

            elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .3f);
            e_y_buffer = round(engine->min_res.y * .1f);

            //Resolution picker, fullscreen toggle, and apply button
            elem_info.sheet = "UI/Button";
            ui_elems.insert({ UIElem::Resume, new Button(elem_info, this, UIElem::Resume) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Title, new Button(elem_info, this, UIElem::Title) });

            elem_info.pos.y += e_y_buffer;
            ui_elems.insert({ UIElem::Quit, new Button(elem_info, this, UIElem::Quit) });

            break;
        }
    }


    //Set our texts (not strictly necessary but keeping for now)
    menu_text.MoveTo(m_t_pos); menu_text.SetStr(m_t_str); menu_text.SetMaxW(engine->min_res.x);
    sup_text.MoveTo(s_t_pos); sup_text.SetStr(s_t_str); sup_text.SetOrigin({ .5f, .0f }); sup_text.SetMaxW(engine->min_res.x);
    if (s_t_str_max_w != -1)
        sup_text.SetMaxW(s_t_str_max_w);
}


void Menu::GetInput() {
    if (open) {
        for (auto& uie : ui_elems)
            uie.second->GetInput();

        for (auto& s_m : sub_menus)
            s_m.second->GetInput();
    }
}

void Menu::Update() {
    if (open) {
        for (auto& ui : ui_elems) ui.second->Update();

        for (const auto& s_m : sub_menus)
            s_m.second->Update();
    }
}

void Menu::Draw() {
    if (open) {
        engine->renderer.DrawTxt(menu_text);
        engine->renderer.DrawTxt(sup_text);

        for (auto& uie : ui_elems)
            uie.second->Draw();

        for (auto& s_m : sub_menus)
            s_m.second->Draw();
    }
}

void Menu::Resize() {
    //Have to resize all the fonts
    menu_text.SetFont();
    sup_text.SetFont();

    for (auto& uie : ui_elems) {
        uie.second->label.SetFont();
        if (Picker* p = dynamic_cast<Picker*>(uie.second)) p->SetPickingF();
    }
}

void Menu::Open(const bool o) {
    open = o;
    has_focus = open;

    if (!open) {
        for (const auto& sm : sub_menus)
            sm.second->Open(false);
    }
}

void Menu::OpenSM(const MenuName s_m) {
    if (sub_menus.count(s_m) > 0)
        sub_menus[s_m]->Open();
    else cout << "That Sub-Menu does not exist in this Menu!\n";
}

void Menu::RemoveUIElem(const UIElem ui) {
    if (CheckUIElem(ui)) {
        delete ui_elems[ui];
        ui_elems.erase(ui);
    }
}

bool Menu::CheckUIElem(const UIElem ui) {
    if (ui_elems.find(ui) != ui_elems.end()) return true;

    cout << "That UI Element does not exist!" << endl;
    return false;
}

void Menu::SetUIElemStatus(const UIElem ui, const string new_status) {

    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui]))
            picker->SetPicking(new_status);
    }
}

void Menu::SetUIElemActive(const UIElem ui, const bool a) {
    if (CheckUIElem(ui))
        ui_elems[ui]->SetActive(a);
}

Vec2i Menu::GetUIElemPos(const UIElem ui) {
    if (CheckUIElem(ui))
        return ui_elems[ui]->GetPos();

    cout << "Menu::GetUIElemPos(): No such UIElem exists\n";
    return Vec2i();
}

string Menu::GetUIElemStatus(const UIElem ui) {

    if (CheckUIElem(ui)) {
        if (auto picker = dynamic_cast<Picker*>(ui_elems[ui]))
            return picker->GetPicking();
    }

    return "Menu::GetUIElemStatus(): No such UIElem exists\n";
}

bool Menu::GetUIElemActive(const UIElem ui) {
    if (CheckUIElem(ui))
        return ui_elems[ui]->GetActive();

    return false;
}
