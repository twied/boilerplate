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

#ifndef SCENE_H_
#define SCENE_H_

union SDL_Event;

namespace game2d {

class Scene {
public:
    Scene();
    virtual ~Scene() = 0;

    /** Draw the scene. */
    virtual void draw() = 0;

    /** Handle an event. */
    virtual void event(SDL_Event* event);

    /** Single physics step. */
    virtual void tick() = 0;

    /** Keyboard status. */
    bool key[512];

    /** Horizontal mouse position. */
    int mouse_x;

    /** Vertical mouse position. */
    int mouse_y;

    /** Bitmask of currently pressed mouse buttons. */
    unsigned button;
};

} /* namespace game2d */

#endif /* SCENE_H_ */
