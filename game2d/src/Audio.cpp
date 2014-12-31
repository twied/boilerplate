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
#include "Gui.h"
#include "Settings.h"

#include <SDL_mixer.h>

namespace game2d {

Mix_Music* Audio::music = NULL;
Mix_Chunk* Audio::pong1 = NULL;
Mix_Chunk* Audio::pong2 = NULL;
Mix_Chunk* Audio::pong3 = NULL;

Audio::Audio() {
}

Audio::~Audio() {
    if (pong1) {
        Mix_FreeChunk(pong1);
    }

    if (pong2) {
        Mix_FreeChunk(pong2);
    }

    if (pong3) {
        Mix_FreeChunk(pong3);
    }

    if (music) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }

    Mix_Quit();
}

bool Audio::init() {
    if (Mix_Init(MIX_INIT_MP3)) {
        Gui::fail("Mix_Init failed", Mix_GetError());
        return false;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096)) {
        Gui::fail("Mix_OpenAudio failed", Mix_GetError());
        return false;
    }

    if (!(music = Mix_LoadMUS("res/music.mp3"))) {
        Gui::fail("Mix_LoadMUS(music.mp3) failed", Mix_GetError());
        return false;
    }

    if (!(pong1 = Mix_LoadWAV("res/pong1.wav"))) {
        Gui::fail("Mix_LoadWAV(pong1.wav) failed", Mix_GetError());
        return false;
    }

    if (!(pong2 = Mix_LoadWAV("res/pong2.wav"))) {
        Gui::fail("Mix_LoadWAV(pong2.wav) failed", Mix_GetError());
        return false;
    }

    if (!(pong3 = Mix_LoadWAV("res/pong3.wav"))) {
        Gui::fail("Mix_LoadWAV(pong3.wav) failed", Mix_GetError());
        return false;
    }

    playMusic(Settings::playMusic);
    return true;
}

void Audio::playMusic(const bool& enable) {
    if (!enable) {
        Mix_PauseMusic();
        return;
    }

    if (Mix_PlayingMusic()) {
        Mix_ResumeMusic();
    } else {
        Mix_PlayMusic(music, -1);
    }
}

void Audio::playPong1() {
    if (!Settings::playSound) {
        return;
    }

    Mix_PlayChannel(-1, pong1, 0);
}

void Audio::playPong2() {
    if (!Settings::playSound) {
        return;
    }

    Mix_PlayChannel(-1, pong2, 0);
}

void Audio::playPong3() {
    if (!Settings::playSound) {
        return;
    }

    Mix_PlayChannel(-1, pong3, 0);
}

} /* namespace game2d */
