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

#ifndef GUI_H_
#define GUI_H_

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;
typedef struct _TTF_Font TTF_Font;

namespace game2d {

struct Particle {
    int posx;
    int posy;
    int velx;
    int vely;
    int rot;
    int age;
};

class Gui {
public:
    Gui();
    ~Gui();

    bool init();

    static void fail(const char* title, const char* message);
    static SDL_Texture* createButton(const char* text);
    static SDL_Texture* createScore(const char* text);

    static void drawBegin();
    static void drawEnd();
    static void drawBackground1();
    static void drawBackground2();
    static void drawBall(const int& x, const int& y);
    static void drawButton(const int& x, const int& y, SDL_Texture* texture, const bool& enabled);
    static void drawPaddle(const int& x, const int& y, const bool& player);
    static void drawParticle(const Particle& particle);
    static void drawScore(SDL_Texture* texture);

private:
    static TTF_Font* font;
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static SDL_Texture* background1;
    static SDL_Texture* background2;
    static SDL_Texture* buttons;
    static SDL_Texture* ball;
    static SDL_Texture* paddle;
    static SDL_Texture* particle;
};

} /* namespace game2d */

#endif /* GUI_H_ */
