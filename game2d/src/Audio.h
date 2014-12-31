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

#ifndef AUDIO_H_
#define AUDIO_H_

typedef struct Mix_Chunk Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

namespace game2d {

class Audio {
public:
    Audio();
    ~Audio();

    bool init();

    static void playMusic(const bool& enable);
    static void playPong1();
    static void playPong2();
    static void playPong3();
private:
    static Mix_Music* music;
    static Mix_Chunk* pong1;
    static Mix_Chunk* pong2;
    static Mix_Chunk* pong3;
};

} /* namespace game2d */

#endif /* AUDIO_H_ */
