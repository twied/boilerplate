CC      = g++
CFLAGS  = `pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf` -Wall -Wextra -pedantic -g -c
LFLAGS  = `pkg-config --libs   sdl2 SDL2_image SDL2_mixer SDL2_ttf`

all: pong

pong: src/Audio.o src/Game.o src/Gui.o src/MainMenu.o src/Match.o src/Scene.o src/Settings.o src/SettingsMenu.o
	$(CC) $(LFLAGS) -o $@ $^

src/%.o: src/%.cpp src/%.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f src/Audio.o src/Game.o src/Gui.o src/MainMenu.o src/Match.o src/Scene.o src/Settings.o src/SettingsMenu.o
	rm -f pong
