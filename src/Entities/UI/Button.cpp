#include "Button.h"

void Button::Update() {
    //When not Selected or primed, this is 0; when Selected but not primed, this is 1; when selected and primed, this is 2
    sprite.SetCurrFrame(Selected() + primed);
}

void Button::Draw() {
    UI::Draw();

    if (sprite.GetSheet() == "UI/Btn_Blank")
        engine->renderer.DrawTxt(label);
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    primed = false;

    switch (widget) {
        case Widget::Apply:
            if (menu->GetName() == MenuName::Options) {
                //Set the game's current resolution to the scale determined by the resolution picker
                uint new_scale = stoi(menu->GetWidgetStatus(Widget::Resolution));
                uint old_scale = engine->resolution.x / engine->min_res.x;
                if (new_scale != old_scale)
                    engine->SetResolution(new_scale);

                //Resize all the text
                game->Resize();

                SetActive(false);
            }
        break;

        case Widget::Back:
            menu->Open(false);
            game->OpenMenu(MenuName::Main);
        break;

        case Widget::Credits:
            menu->Open(false);
            game->OpenMenu(MenuName::Credits);
        break;

        case Widget::Infinite:
            game->gm_mode = GameMode::Infinite;
            game->high_score = game->high_scores["Infinite"];
            game->ChangeScene(Scene::Game);
        break;

        case Widget::Options:
            menu->Open(false);
            game->OpenMenu(MenuName::Options);
        break;

        case Widget::Play:
            menu->Open(false);
            game->OpenMenu(MenuName::Choose_Game);
        break;

        case Widget::Quit:
            engine->window.open = false;
        break;

        case Widget::Resume:
            menu->Open(false);
            game->paused = false;
        break;

        case Widget::Title:
            game->gm_mode = GameMode::NONE;
            game->ChangeScene(Scene::Title);
        break;
    }
}
