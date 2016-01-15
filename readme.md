# Underneath

A dungeon crawler with old-school style ASCII graphics.


### Written in c++11

This program uses standard c++ libraries.  
The terminal graphics version uses ncurses (pdcurses on windows).  
The SDL2 graphics version uses SDL2 and SDL2_Image libraries. (use the -s flag during compile)  
The version with music uses SDL2 and SDL2_mixer libraries. (use the -m flag during compile)  

My development is primarily in MacOSX and some in Linux. I have successfully compiled for Windows using MinGW on Linux and Mac. Good luck compiling locally on Windows, I haven't successfully done that in a while (although I am not very well versed in windows development).


Braeden Atlee reserves all rights to the souce code.
Davalynn Towell reserves all rights to the audio files.

---


## Compiling:

I made a compile script with python, I originally used GNU MAKE, but I'm not well versed with that and I kept running into issues so I moved to a langueage I have mor experience with. I haven't compiled in a long time directly from Windows itself. So the windows builds are compiled on linux and mac using mingw32

To build the terminal version run the following in the repository directory:
```
./build.py
```

To compile the version that runs in a window instead of the terminal, use the following flag: 
```
./build.py -s
```

To compile with audio, use the following flag: 
```
./build.py -m
```

If you need to completely rebuild all of the files you can use the -a flag to ensure no files are skipped.
```
./build.py -a
```

To run immediately after building use the -r flag.
```
./build.py -r
```

Flags can be used together, for example to build and run all with SDL2 Graphics, Music and debug optimization:
```
./build.py -smaur
```
All flags:
```
-h  List all flags
-r  Run after successful build

-s  Compile using SDL2 Graphics instead of Curses
-m  Compile with the SDL2 Mixer Audio Library (no audio by default)

-a  Compile all sources (it defaults to only ones that have changed)
-i  Don't check included headers (makes compiling faster, but doesn't recompile cpp files if only included headers were modified)
-l  Link only, don't compile sources
-u  Optimization level 0 (default 3) and show warnings

-d  Print all commands that the build.py is executing
-t  Do not remake ArtFiles.hpp

--use64 Compile with the -m64 flag (force 64-Bit)
--use32 Compile with the -m32 flag (force 32-bit)
-w  Use Windows mingw32 compiler (Mac: i586-mingw32-c++, Linux: i686-w64-mingw32-c++)
--compiler COMPILER  Specify the compiler manually (default is g++)
```

---
