#include <windows.h>
#include <shellapi.h>
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

        case UIElem::Apply:
            if (menu->GetName() == MenuName::Options) {
                //Set the game's current resolution to the scale determined by the resolution picker
                uint new_scale = stoi(menu->GetUIElemStatus(UIElem::Resolution));
                uint old_scale = engine->resolution.x / engine->min_res.x;
                if (new_scale != old_scale)
                    engine->SetResolution(new_scale);

                //Resize all the text
                game->Resize();

                SetActive(false);
            }
        break;

        case UIElem::Back:
            menu->Open(false);
            game->OpenMenu(MenuName::Main);
        break;

        case UIElem::Infinite:
            game->gm_mode = GameMode::Infinite;
            game->ChangeScene(Scene::Game);
        break;

        case UIElem::Moves:
            game->moves_remaining = stoi(menu->GetUIElemStatus(UIElem::Moves_P));
            game->gm_mode = GameMode::Moves;
            game->ChangeScene(Scene::Game);
        break;

        case UIElem::Options:
            menu->Open(false);
            game->OpenMenu(MenuName::Options);
        break;

        case UIElem::Play:
            menu->Open(false);
            game->OpenMenu(MenuName::Choose_Game);
        break;

        case UIElem::Quit:
            engine->window.open = false;
        break;

        case UIElem::Resume:
            menu->Open(false);
            game->paused = false;
        break;

        case UIElem::Time:
            game->time_remaining = stoi(menu->GetUIElemStatus(UIElem::Time_P));
            game->gm_mode = GameMode::Time;
            game->ChangeScene(Scene::Game);
        break;

        case UIElem::Title:
            game->gm_mode = GameMode::NONE;
            game->ChangeScene(Scene::Title);
        break;
    }
}
