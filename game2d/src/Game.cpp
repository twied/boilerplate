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

#include <SDL.h>

/** Milliseconds per physics tick. */
#define MILLISECONDS_PER_TICK (1000 / 50)

/** Milliseconds per status report. */
#define MILLISECONDS_PER_STAT (5000)

namespace game2d {

Scene* Game::scene = NULL;
bool Game::running = false;

Game::Game(int argc, char *argv[]) :
        frames(0), ticks(0), audio(new Audio()), gui(new Gui()), settings(new Settings()) {
    (void) argc;
    (void) argv;
    if (!gui->init()) {
        return;
    }

    if (!audio->init()) {
        return;
    }

    set_scene(new MainMenu());
    running = true;
}

Game::~Game() {
    delete scene;
    delete audio;
    delete gui;
    delete settings;
}

void Game::draw() {
    gui->drawBegin();
    scene->draw();
    gui->drawEnd();

    frames += 1;
}

void Game::event() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        scene->event(&event);
    }
}

void Game::stat() {
    static Uint32 nextStat = SDL_GetTicks() + MILLISECONDS_PER_STAT;

    if (SDL_GetTicks() >= nextStat) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "tps: %4i, fps: %4i\n", ticks / (MILLISECONDS_PER_STAT / 1000), frames / (MILLISECONDS_PER_STAT / 1000));
        nextStat += MILLISECONDS_PER_STAT;
        frames = 0;
        ticks = 0;
    }
}

void Game::tick() {
    static Uint32 nextTick = SDL_GetTicks() + MILLISECONDS_PER_TICK;

    while (SDL_GetTicks() >= nextTick) {
        ticks += 1;
        scene->tick();
        nextTick += MILLISECONDS_PER_TICK;
    }
}

Game::operator bool() {
    return running;
}

void Game::set_scene(Scene* next_scene) {
    delete scene;
    scene = next_scene;
}

void Game::quit() {
    running = false;
}

} /* namespace game2d */

int main(int argc, char* argv[]) {
    game2d::Game game(argc, argv);

    while (game) {
        game.event();
        game.tick();
        game.draw();
        game.stat();
    }

    return 0;
}
