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

#include "Scene.h"

#include <SDL.h>

namespace game2d {

Scene::Scene() :
        mouse_x(0), mouse_y(0), button(0) {
    for (size_t i = 0; i < sizeof(key); ++i) {
        key[i] = false;
    }
}

Scene::~Scene() {
}

void Scene::event(SDL_Event* event) {
    switch (event->type) {
    case SDL_MOUSEMOTION:
        mouse_x = event->motion.x;
        mouse_y = event->motion.y;
        break;

    case SDL_MOUSEBUTTONDOWN:
        button |= 1 << event->button.button;
        mouse_x = event->button.x;
        mouse_y = event->button.y;
        break;

    case SDL_MOUSEBUTTONUP:
        button ^= 1 << event->button.button;
        mouse_x = event->button.x;
        mouse_y = event->button.y;
        break;

    case SDL_KEYDOWN:
        key[event->key.keysym.scancode] = true;
        break;

    case SDL_KEYUP:
        key[event->key.keysym.scancode] = false;
        break;

    default:
        return;
    }
}

} /* namespace game2d */
