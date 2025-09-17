#include <windows.h>
#include <shellapi.h>
#include "Button.h"

void Button::Update() {
}

void Button::Draw() {
    if (Selected())
        game->renderer.DrawRect(bbox, Color(1, 0, 0));

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
                uint old_scale = game->resolution.x / game->min_res.x;
                if (new_scale != old_scale)
                    game->SetResolution(new_scale);

                //Resize all the text
                game->scene.Resize();

                SetActive(false);
            }
        break;

        case UIElem::Back:
            menu->Open(false);
            game->scene.OpenMenu(MenuName::Main);
        break;

        case UIElem::Options:
            menu->Open(false);
            game->scene.OpenMenu(MenuName::Options);
        break;

        case UIElem::Play:
            game->scene.ChangeScene(SceneName::Game);
        break;

        case UIElem::Quit:
            game->window.open = false;
        break;

        case UIElem::Resume:
            menu->Open(false);
            game->paused = false;
        break;

        case UIElem::Title:
            game->scene.ChangeScene(SceneName::Title);
        break;
    }
}
