# L in Cubeland
"L in Cubeland" (LiC) is a simple 3D, closed/gridlike-world, sandbox/puzzle, score/survival-oriented, single-player game.

It has been programmed using C++, OpenGL, freeglut, SDL2 (mainly SDL2_mixer for sound) and NSIS for its installer.

To install the program on your computer, run LiC-installer.exe or copy LiC/ and create a folder named storage inside it.

To build the program from the source with MinGW, use the following commands, with filenames of your choice:
- windres LiC.rc -o *resources_filename*
- g++ LiC.cpp *resources_filename* -o *executable_filename* -Wl,--subsystem,windows -lfreeglut -lglu32 -lopengl32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer

For credits, license, game description, instructions for use etc, read LiC/information/LiC.pdf.
