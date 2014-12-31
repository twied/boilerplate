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

#ifndef MATCH_H_
#define MATCH_H_

#include "Gui.h"
#include "Scene.h"

#include <list>

namespace game2d {

class Match : public Scene {
public:
    Match();
    ~Match();

    void draw();
    void event(SDL_Event* event);
    void tick();
private:
    void reset();
    void addParticles();

    std::list<Particle> particles;
    unsigned pause;
    int ball_posx;
    int ball_posy;
    int ball_velx;
    int ball_vely;
    int paddleLeft;
    int paddleRight;
    int pointsLeft;
    int pointsRight;
    SDL_Texture* scoreText;
};

} /* namespace game2d */

#endif /* MATCH_H_ */
