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
#include "Match.h"
#include "MainMenu.h"
#include "SettingsMenu.h"

#include <SDL.h>

namespace game2d {

MainMenu::MainMenu() {
    buttonPlay = Gui::createButton("Play");
    buttonSettings = Gui::createButton("Settings");
    buttonExit = Gui::createButton("Exit");
}

MainMenu::~MainMenu() {
    if (buttonPlay) {
        SDL_DestroyTexture(buttonPlay);
    }

    if (buttonSettings) {
        SDL_DestroyTexture(buttonSettings);
    }

    if (buttonExit) {
        SDL_DestroyTexture(buttonExit);
    }
}

void MainMenu::draw() {
    Gui::drawBackground1();

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 360 && mouse_y < 440) {
        Gui::drawButton(200, 360, buttonPlay, true);
    } else {
        Gui::drawButton(200, 360, buttonPlay, false);
    }

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 440 && mouse_y < 520) {
        Gui::drawButton(200, 440, buttonSettings, true);
    } else {
        Gui::drawButton(200, 440, buttonSettings, false);
    }

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 520 && mouse_y < 600) {
        Gui::drawButton(200, 520, buttonExit, true);
    } else {
        Gui::drawButton(200, 520, buttonExit, false);
    }
}

void MainMenu::event(SDL_Event* event) {
    Scene::event(event);

    if (key[SDL_SCANCODE_ESCAPE]) {
        Game::quit();
    }

    if (event->type != SDL_MOUSEBUTTONUP) {
        return;
    }

    if (mouse_x > 200 && mouse_x < 600 && mouse_y > 360 && mouse_y < 440) {
        Audio::playPong2();
        Game::set_scene(new Match());
    } else if (mouse_x > 200 && mouse_x < 600 && mouse_y > 440 && mouse_y < 520) {
        Audio::playPong1();
        Game::set_scene(new SettingsMenu());
    } else if (mouse_x > 200 && mouse_x < 600 && mouse_y > 520 && mouse_y < 600) {
        Audio::playPong1();
        Game::quit();
        return;
    }
}

void MainMenu::tick() {
}

} /* namespace game2d */
