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

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "Scene.h"

namespace game2d {

class MainMenu : public Scene {
public:
    MainMenu();
    ~MainMenu();

    void draw();
    void event(SDL_Event* event);
    void tick();
private:
    SDL_Texture* buttonPlay;
    SDL_Texture* buttonSettings;
    SDL_Texture* buttonExit;
};

} /* namespace game2d */

#endif /* MAINMENU_H_ */
