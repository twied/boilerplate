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

#ifndef GAME_H_
#define GAME_H_

namespace game2d {

class Audio;
class Gui;
class Scene;
class Settings;

class Game {
public:
    Game(int argc, char* argv[]);
    ~Game();

    operator bool();
    void event();
    void tick();
    void draw();
    void stat();

    static void set_scene(Scene *scene);
    static void quit();
private:
    Game(const Game&);
    Game operator=(const Game&);

    /** Current scene. */
    static Scene* scene;

    /** Current status. */
    static bool running;

    /** Frames drawn since last FPS calculation. */
    unsigned frames;

    /** Physic ticks since last FPS calculation. */
    unsigned ticks;

    Audio* audio;
    Gui* gui;
    Settings* settings;
};

} /* namespace game2d */

#endif /* GAME_H_ */
