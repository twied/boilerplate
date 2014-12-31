/*
 * game2d PONG: A simple game engine demo
 * Copyright (C) 2014 Tim Wiederhake
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "Audio.h"
#include "Game.h"
#include "Gui.h"
#include "MainMenu.h"
#include "Settings.h"
#include "SettingsMenu.h"

#include <SDL.h>

namespace game2d {

SettingsMenu::SettingsMenu() {
    buttonSound[0] = Gui::createButton("Sound: Off");
    buttonSound[1] = Gui::createButton("Sound: On");
    buttonMusic[0] = Gui::createButton("Music: Off");
    buttonMusic[1] = Gui::createButton("Music: On");
    buttonBack = Gui::createButton("Back");
}

SettingsMenu::~SettingsMenu() {
    for (int i = 0; i < 2; ++i) {
        if (buttonSound[i]) {
            SDL_DestroyTexture(buttonSound[i]);
        }

        if (buttonMusic[i]) {
            SDL_DestroyTexture(buttonMusic[i]);
        }
    }

    if (buttonBack) {
        SDL_DestroyTexture(buttonBack);
    }
}

void SettingsMenu::draw() {
    Gui::drawBackground1();

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 360 && mouse_y < 440) {
        Gui::drawButton(200, 360, buttonSound[Settings::playSound ? 1 : 0], true);
    } else {
        Gui::drawButton(200, 360, buttonSound[Settings::playSound ? 1 : 0], false);
    }

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 440 && mouse_y < 520) {
        Gui::drawButton(200, 440, buttonMusic[Settings::playMusic ? 1 : 0], true);
    } else {
        Gui::drawButton(200, 440, buttonMusic[Settings::playMusic ? 1 : 0], false);
    }

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 520 && mouse_y < 600) {
        Gui::drawButton(200, 520, buttonBack, true);
    } else {
        Gui::drawButton(200, 520, buttonBack, false);
    }
}

void SettingsMenu::event(SDL_Event* event) {
    Scene::event(event);

    if (event->type != SDL_MOUSEBUTTONUP) {
        return;
    }

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 360 && mouse_y < 440) {
        Audio::playPong1();
        Settings::playSound = !Settings::playSound;
    } else if (mouse_x > 200 && mouse_x < 600 && mouse_y > 440 && mouse_y < 520) {
        Audio::playPong1();
        Settings::playMusic = !Settings::playMusic;
        Audio::playMusic(Settings::playMusic);
    } else if (mouse_x > 200 && mouse_x < 600 && mouse_y > 520 && mouse_y < 600) {
        Audio::playPong1();
        Game::set_scene(new MainMenu());
        return;
    }
}

void SettingsMenu::tick() {
}

} /* namespace game2d */
