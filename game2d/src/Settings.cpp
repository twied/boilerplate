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

#include "Settings.h"

#include <fstream>

#ifdef _WIN32
#include <windows.h>
static void create_dir(const char* dir) {
    CreateDirectory(dir, NULL);
}
#else
#include <sys/stat.h>
static void create_dir(const char* dir) {
    mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
#endif

#define PATH_SAVE "save/"
#define PATH_SETTINGS (PATH_SAVE "settings.txt")

using namespace std;

namespace game2d {

bool Settings::playMusic = true;
bool Settings::playSound = true;

Settings::Settings() {
    ifstream settings_stream(PATH_SETTINGS);
    if (settings_stream) {
        settings_stream >> playMusic;
        settings_stream >> playSound;
    }
}

Settings::~Settings() {
    create_dir(PATH_SAVE);
    ofstream stream(PATH_SETTINGS);
    if (!stream) {
        return;
    }

    stream << playMusic << endl;
    stream << playSound << endl;
}

} /* namespace game2d */
