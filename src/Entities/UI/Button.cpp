#include "Button.h"
#include "../../Engine/Engine.h"
#include "../../Game/Menu.h"

void Button::Update() {
}

void Button::Draw() {
    if (Selected())
        engine->renderer.DrawRect(bbox, Color(1, 0, 0));

    UI::Draw();
}

void Button::Pressed() {
    UI::Pressed();
}

void Button::Released() {
    activated = true;
    switch (elem) {

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

        case Widget::Infinite:
            game->gm_mode = GameMode::Infinite;
            game->high_score = game->high_scores["Infinite"];
            game->ChangeScene(Scene::Game);
        break;

        case Widget::Moves:
            game->moves_remaining = stoi(menu->GetWidgetStatus(Widget::Moves_P));
            game->high_score = game->high_scores["Moves"][menu->GetWidgetStatus(Widget::Moves_P)];
            game->gm_mode = GameMode::Moves;
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

        case Widget::Time:
            game->time_remaining = stoi(menu->GetWidgetStatus(Widget::Time_P));
            game->high_score = game->high_scores["Time"][menu->GetWidgetStatus(Widget::Time_P)];
            game->gm_mode = GameMode::Time;
            game->ChangeScene(Scene::Game);
        break;

        case Widget::Title:
            game->gm_mode = GameMode::NONE;
            game->ChangeScene(Scene::Title);
        break;
    }
}
