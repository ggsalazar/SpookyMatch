#include "Menu.h"
#include "../Entities/UI/Button.h"
#include "../Entities/UI/Picker.h"
#include "src/Entities/UI/Slider.h"

Menu::Menu(const MenuName i_name) : name(i_name), menu_text(42), sup_text(24) {

    //Init the menu image

    //Menu and Supp text variables
    menu_text.SetOrigin();
    Vec2i m_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .27f);
    string m_t_str;

    sup_text.SetOrigin();
    Vec2i s_t_pos = { 0 };
    string s_t_str;
    uint s_t_str_max_w = 0;

    Sprite::Info elem_info = {}; elem_info.origin = ui_ori;
    int e_y_buffer = 0;
    //What we do depends on our name
    switch (name) {
        case MenuName::Main: {
            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .4f);

            e_y_buffer = round(engine->min_res.y * .1f);

            widgets.insert({ Widget::Play, new Button(elem_info, this, Widget::Play) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({ Widget::Options, new Button(elem_info, this, Widget::Options) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({Widget::Credits, new Button(elem_info, this, Widget::Credits) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({ Widget::Quit, new Button(elem_info, this, Widget::Quit) });

            break;
        }

        case MenuName::Choose_Game: {
            m_t_str = "Select Game Mode";

            elem_info.pos = Round(engine->min_res.x * .3f, engine->min_res.y * .4f);
            
            widgets.insert({ Widget::Time_P, new Picker(elem_info, this, Widget::Time_P) });

            elem_info.pos = Round(engine->min_res.x * .7f, engine->min_res.y * .4f);
            widgets.insert({ Widget::Moves_P, new Picker(elem_info, this, Widget::Moves_P) });

            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .55f);
            widgets.insert({ Widget::Infinite, new Button(elem_info, this, Widget::Infinite) });

            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .8f);
            widgets.insert({ Widget::Back, new Button(elem_info, this, Widget::Back) });

            break;
        }

        case MenuName::GO: {
            m_t_str = "Game Over!";

            elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .4f);

            widgets.insert({ Widget::Title, new Button(elem_info, this, Widget::Title) });

            elem_info.pos.y = engine->min_res.y * .6f;
            widgets.insert({ Widget::Quit, new Button(elem_info, this, Widget::Quit) });
            break;
        }
        
        case MenuName::Options: {
            m_t_str = "Settings";

            elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .4f);
            e_y_buffer = round(engine->min_res.y * .1f);

            widgets.insert({ Widget::Resolution, new Picker(elem_info, this, Widget::Resolution) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({ Widget::Apply, new Button(elem_info, this, Widget::Apply) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({Widget::Music_P, new Picker(elem_info, this, Widget::Music_P) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({Widget::Msc_Vol, new Slider(elem_info, this, Widget::Msc_Vol) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({ Widget::Back, new Button(elem_info, this, Widget::Back) });

            break;
        }

        case MenuName::Credits: {
            m_t_str = "Credits";
            s_t_str = "Programming and Game Design: Griffin G. Salazar\nArt and Sprites: Gabriela \"Brysia\" Szmit\nMusic\n\"Ghoulish Fun\" by Eric Matyas\n\"Spooky Enchantment\" by Eric Matyas\n\"Trick or Treat\" by HitCtrl\nAll music found on OpenGameArt.org";
            s_t_pos = Round(engine->min_res.x * .5f, engine->min_res.y * .35f);
            sup_text.SetColor(Color(0, 1));
            sup_text.SetLHO(5);
            elem_info.pos = Round(engine->min_res.x * .5f, engine->min_res.y * .8f);
            widgets.insert({Widget::Back, new Button(elem_info, this, Widget::Back) });
            break;
        }

        case MenuName::Options_I: {
            m_t_str = "Settings";

            elem_info.pos = Round(m_t_pos.x, engine->min_res.y * .3f);
            e_y_buffer = round(engine->min_res.y * .1f);

            //Resolution new Picker, fullscreen toggle, and apply new Button
            widgets.insert({ Widget::Resume, new Button(elem_info, this, Widget::Resume) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({ Widget::Title, new Button(elem_info, this, Widget::Title) });

            elem_info.pos.y += e_y_buffer;
            widgets.insert({ Widget::Quit, new Button(elem_info, this, Widget::Quit) });

            break;
        }
    }

    //Set our texts
    menu_text.MoveTo(m_t_pos); menu_text.SetStr(m_t_str); menu_text.SetMaxW(engine->min_res.x);
    sup_text.MoveTo(s_t_pos); sup_text.SetStr(s_t_str); sup_text.SetOrigin({ .5f, .0f }); sup_text.SetMaxW(engine->min_res.x);
    if (s_t_str_max_w != 0)
        sup_text.SetMaxW(s_t_str_max_w);
}

Menu::~Menu() {
    for (auto& [_, w] : widgets) delete w;
    widgets.clear();

    for (auto& [_, sm] : sub_menus) delete sm;
    sub_menus.clear();
}


void Menu::GetInput() {
    if (open) {
        for (auto& w : widgets)
            w.second->GetInput();

        for (auto& s_m : sub_menus)
            s_m.second->GetInput();
    }
}

void Menu::Update() {
    if (open) {
        for (auto& w : widgets) w.second->Update();

        for (const auto& s_m : sub_menus)
            s_m.second->Update();
    }
}

void Menu::Draw() {
    if (open) {
        engine->renderer.DrawTxt(menu_text);
        engine->renderer.DrawTxt(sup_text);



        if (name == MenuName::Credits) {
            Rect test = Rect(
                {
                    (int)(sup_text.GetPos().x - sup_text.GetOrigin().x * sup_text.GetStrSize().x),
                    (int)(sup_text.GetPos().y - sup_text.GetOrigin().y * sup_text.GetStrSize().y),
                },
                sup_text.GetStrSize());

            engine->renderer.DrawRect(test, Color(0,0,1, .5), Color(0,0,1, .8));
        }


        for (auto& uie : widgets)
            uie.second->Draw();

        for (auto& s_m : sub_menus)
            s_m.second->Draw();
    }
}

void Menu::Resize() {
    //Have to resize all the fonts
    menu_text.SetFont();
    sup_text.SetFont();

    for (auto& w : widgets) {
        w.second->label.SetFont();
        if (Picker* p = dynamic_cast<Picker*>(w.second)) p->SetPickingF();
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
    else cout << "Menu::OpenSM(): That Sub-Menu does not exist in this Menu!\n";
}

void Menu::RemoveWidget(const Widget w) {
    if (CheckWidget(w)) {
        //Why is this commented out?
    	//delete widgets[ui];
        widgets.erase(w);
    }
}

bool Menu::CheckWidget(const Widget w) {
    if (widgets.find(w) != widgets.end()) return true;

    cout << "Menu::CheckWidget(): That Widget does not exist!" << endl;
    return false;
}

bool Menu::GetWidgetActive(const Widget w) {
    if (CheckWidget(w))
        return widgets[w]->GetActive();

    return false;
}

void Menu::SetWidgetActive(const Widget w, const bool a) {
    if (CheckWidget(w))
        widgets[w]->SetActive(a);
}

Vec2i Menu::GetWidgetPos(const Widget w) {
    if (CheckWidget(w))
        return widgets[w]->GetPos();

    cout << "Menu::GetWidgetPos(): No such Widget exists\n";
    return Vec2i();
}

void Menu::SetWidgetStatus(const Widget w, const string new_status) {
    if (CheckWidget(w)) {
        if (auto p = dynamic_cast<Picker*>(widgets[w]))
            p->SetPicking(new_status);
    }
}

string Menu::GetWidgetStatus(const Widget w) {
    if (CheckWidget(w)) {
        if (Picker* p = dynamic_cast<Picker*>(widgets[w]))
            return p->GetPicking();
    }

    return "Menu::GetWidgetStatus(): No such Widget exists\n";
}


