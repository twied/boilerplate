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

#include <SDL.h>

#include <cstdlib>
#include <ctime>
#include <sstream>

#define BALL_VELOCITY 3

using namespace std;

namespace game2d {

Match::Match() :
        particles(), pause(30), ball_posx(400), ball_posy(300), ball_velx(1), ball_vely(1), paddleLeft(300), paddleRight(300), pointsLeft(0), pointsRight(0), scoreText(NULL) {
    srand(time(NULL));
    reset();
}

Match::~Match() {
    if (scoreText) {
        SDL_DestroyTexture(scoreText);
    }
}

void Match::draw() {
    Gui::drawBackground2();
    Gui::drawScore(scoreText);
    Gui::drawPaddle(10, paddleLeft - 65, true);
    Gui::drawPaddle(780, paddleRight - 65, false);
    Gui::drawBall(ball_posx - 16, ball_posy - 16);
    for (list<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
        Gui::drawParticle(*it);
    }
}

void Match::event(SDL_Event* event) {
    Scene::event(event);

    if (key[SDL_SCANCODE_ESCAPE]) {
        Game::set_scene(new MainMenu());
        return;
    }
}

void Match::tick() {
    /* right paddle movement */
    if (ball_posy > paddleRight && paddleRight < 490) {
        paddleRight += 2;
    }

    if (ball_posy < paddleRight && paddleRight > 110) {
        paddleRight -= 2;
    }

    /* left paddle movement */
    if (key[SDL_SCANCODE_UP] && paddleLeft > 110) {
        paddleLeft -= 2;
    }

    if (key[SDL_SCANCODE_DOWN] && paddleLeft < 490) {
        paddleLeft += 2;
    }

    /* ball movement */
    if (pause) {
        pause -= 1;
    } else {
        ball_posx += ball_velx * BALL_VELOCITY;
        ball_posy += ball_vely * BALL_VELOCITY;
    }

    /* collision with lower bound */
    if (ball_posy >= 544) {
        Audio::playPong1();
        addParticles();
        ball_vely = -1;
    }

    /* collision with upper bound */
    if (ball_posy <= 56) {
        Audio::playPong1();
        addParticles();
        ball_vely = 1;
    }

    /* collision with right bound */
    if (ball_posx > 764) {
        if (ball_posy > paddleRight - 65 && ball_posy < paddleRight + 65) {
            Audio::playPong2();
            addParticles();
            ball_velx = -1;
        } else {
            Audio::playPong3();
            pointsLeft += 1;
            reset();
            return;
        }
    }

    /* collision with left bound */
    if (ball_posx < 36) {
        if (ball_posy > paddleLeft - 65 && ball_posy < paddleLeft + 65) {
            Audio::playPong2();
            addParticles();
            ball_velx = 1;
        } else {
            Audio::playPong3();
            pointsRight += 1;
            reset();
            return;
        }
    }

    /* particle movement */
    for (list<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
        if (--(it->age) < 0) {
            particles.erase(it++);
            continue;
        }

        it->posx += it->velx;
        it->posy += it->vely;
        it->rot += it->velx * it->vely;
    }
}

void Match::reset() {
    ball_posx = 400;
    ball_posy = 300;
    paddleLeft = 300;
    paddleRight = 300;
    ball_velx = rand() & 0x01 ? 1 : -1;
    ball_vely = rand() & 0x01 ? 1 : -1;
    pause = 30;
    stringstream stream;
    stream << pointsLeft << " : " << pointsRight;
    if (scoreText) {
        SDL_DestroyTexture(scoreText);
    }
    scoreText = Gui::createScore(stream.str().c_str());
}

void Match::addParticles() {
    for (int i = 0; i < 5; ++i) {
        Particle particle = {ball_posx, ball_posy, (rand() % 5) - 3, (rand() % 5) - 3, rand() % 180, 10 + (rand() % 60)};
        particles.push_back(particle);
    }
}

} /* namespace game2d */
