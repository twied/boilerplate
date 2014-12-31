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

#ifndef SETTINGSMENU_H_
#define SETTINGSMENU_H_

#include "Scene.h"

namespace game2d {

class SettingsMenu : public Scene {
public:
    SettingsMenu();
    ~SettingsMenu();

    void draw();
    void event(SDL_Event* event);
    void tick();
private:
    SDL_Texture* buttonSound[2];
    SDL_Texture* buttonMusic[2];
    SDL_Texture* buttonBack;
};

} /* namespace game2d */

#endif /* SETTINGSMENU_H_ */
