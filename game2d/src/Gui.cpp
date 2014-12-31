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

#include "Gui.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/** Width of the screen in px. */
#define SCREEN_WIDTH 800

/** Height of the screen in px. */
#define SCREEN_HEIGHT 600

/** Window title. */
#define SCREEN_TITLE "game2d - PONG"

namespace game2d {

TTF_Font* Gui::font = NULL;
SDL_Window* Gui::window = NULL;
SDL_Renderer* Gui::renderer = NULL;
SDL_Texture* Gui::background1 = NULL;
SDL_Texture* Gui::background2 = NULL;
SDL_Texture* Gui::buttons = NULL;
SDL_Texture* Gui::ball = NULL;
SDL_Texture* Gui::paddle = NULL;
SDL_Texture* Gui::particle = NULL;

static SDL_Color textColors[] = {
        { 28, 141,   6, 255},    /* dark green */
        { 49, 242,  10, 255},    /* light green */
        {255, 255, 255, 255}     /* white */
};

Gui::Gui() {
}

Gui::~Gui() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    if (background1) {
        SDL_DestroyTexture(background1);
    }

    if (background2) {
        SDL_DestroyTexture(background2);
    }

    if (buttons) {
        SDL_DestroyTexture(buttons);
    }

    if (ball) {
        SDL_DestroyTexture(ball);
    }

    if (paddle) {
        SDL_DestroyTexture(paddle);
    }

    if (particle) {
        SDL_DestroyTexture(particle);
    }

    if (font) {
        TTF_CloseFont(font);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Gui::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fail("SDL_Init failed", SDL_GetError());
        return false;
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        fail("IMG_Init failed", IMG_GetError());
        return false;
    }

    if (TTF_Init()) {
        fail("TTF_Init failed", TTF_GetError());
        return false;
    }

    if (!(font = TTF_OpenFont("res/TranscendsGames.otf", 32))) {
        fail("TTF_OpenFont(font.ttf) failed", TTF_GetError());
        return false;
    }
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT);

    if (!(window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0))) {
        fail("SDL_CreateWindow failed", SDL_GetError());
        return false;
    }

    if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
        fail("SDL_CreateRenderer failed", SDL_GetError());
        return false;
    }

    if (!(background1 = IMG_LoadTexture(renderer, "res/background1.png"))) {
        fail("IMG_LoadTexture(background1.png) failed", IMG_GetError());
        return false;
    }

    if (!(background2 = IMG_LoadTexture(renderer, "res/background2.png"))) {
        fail("IMG_LoadTexture(background2.png) failed", IMG_GetError());
        return false;
    }

    if (!(buttons = IMG_LoadTexture(renderer, "res/buttons.png"))) {
        fail("IMG_LoadTexture(buttons.png) failed", IMG_GetError());
        return false;
    }

    if (!(ball = IMG_LoadTexture(renderer, "res/ball.png"))) {
        fail("IMG_LoadTexture(ball.png) failed", IMG_GetError());
        return false;
    }

    if (!(paddle = IMG_LoadTexture(renderer, "res/paddle.png"))) {
        fail("IMG_LoadTexture(paddle.png) failed", IMG_GetError());
        return false;
    }

    if (!(particle = IMG_LoadTexture(renderer, "res/particle.png"))) {
        fail("IMG_LoadTexture(particle.png) failed", IMG_GetError());
        return false;
    }

    return true;
}

void Gui::fail(const char* title, const char* message) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "title=\"%s\", message=\"%s\"", title, message);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, window);
}

SDL_Texture* Gui::createButton(const char* text) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 400, 2 * 80);
    if (!texture) {
        return NULL;
    }

    SDL_Surface* surface0 = TTF_RenderText_Blended_Wrapped(font, text, textColors[0], 380);
    if (!surface0) {
        return NULL;
    }

    SDL_Surface* surface1 = TTF_RenderText_Blended_Wrapped(font, text, textColors[1], 380);
    if (!surface1) {
        return NULL;
    }

    SDL_Texture* overlay0 = SDL_CreateTextureFromSurface(renderer, surface0);
    if (!overlay0) {
        return NULL;
    }

    SDL_Texture* overlay1 = SDL_CreateTextureFromSurface(renderer, surface1);
    if (!overlay1) {
        return NULL;
    }

    SDL_FreeSurface(surface0);
    SDL_FreeSurface(surface1);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, buttons, NULL, NULL);

    SDL_Rect src0 = {0, 0, 0, 0};
    SDL_Rect src1 = {0, 0, 0, 0};
    SDL_QueryTexture(overlay0, NULL, NULL, &src0.w, &src0.h);
    SDL_QueryTexture(overlay1, NULL, NULL, &src1.w, &src1.h);

    SDL_Rect dst0 = {(400 - src0.w) / 2, (80 - src0.h) / 2, src0.w, src0.h};
    SDL_Rect dst1 = {(400 - src1.w) / 2, 80 + (80 - src1.h) / 2, src1.w, src1.h};

    SDL_RenderCopy(renderer, overlay0, &src0, &dst0);
    SDL_RenderCopy(renderer, overlay1, &src1, &dst1);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(overlay0);
    SDL_DestroyTexture(overlay1);
    return texture;
}

SDL_Texture* Gui::createScore(const char* text) {
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, textColors[2], 380);
    if (!surface) {
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        return NULL;
    }

    SDL_FreeSurface(surface);
    return texture;
}

void Gui::drawBegin() {
    SDL_SetRenderDrawColor(renderer, 10, 1, 2, 255);
    SDL_RenderClear(renderer);
}

void Gui::drawEnd() {
    SDL_RenderPresent(renderer);
}

void Gui::drawBackground1() {
    SDL_RenderCopy(renderer, background1, NULL, NULL);
}

void Gui::drawBackground2() {
    SDL_RenderCopy(renderer, background2, NULL, NULL);
}

void Gui::drawBall(const int& x, const int& y) {
    SDL_Rect dst = {x, y, 32, 32};
    SDL_RenderCopy(renderer, ball, NULL, &dst);
}

void Gui::drawButton(const int& x, const int& y, SDL_Texture* texture, const bool& enabled) {
    if (!texture) {
        return;
    }

    SDL_Rect src = {0, enabled ? 80 : 0, 400, 80};
    SDL_Rect dst = {x, y, 400, 80};
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void Gui::drawPaddle(const int& x, const int& y, const bool& player) {
    SDL_Rect src = {player ? 0 : 10, 0, 10, 130};
    SDL_Rect dst = {x, y, 10, 130};
    SDL_RenderCopy(renderer, paddle, &src, &dst);
}

void Gui::drawParticle(const Particle& p) {
    SDL_Rect dst = {p.posx, p.posy, 4, 4};
    SDL_RenderCopyEx(renderer, particle, NULL, &dst, p.rot, NULL, SDL_FLIP_NONE);
}

void Gui::drawScore(SDL_Texture* texture) {
    SDL_Rect dst = {0, 5, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = (800 - dst.w) / 2;
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

} /* namespace game2d */
