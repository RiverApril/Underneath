Underneath
===

A dungeon crawler with ASCII graphics lightly based on the classic game Rogue


### Written in c++11

This program uses standard c++ libraries and ncurses (pdcurses in Windows).
SDL version uses SDL2 and SDL_Image.

My development is primarily in MacOSX and some in Linux. I have successfully compiled for Windows using MinGW on Linux.


---

## Latest Builds:

#### Linux:
Executables:
- Terminal: [Underneath Optimized Terminal Linux](Underneath_Optim_Term_Linux)
- SDL: [Underneath Optimized SDL Linux](Underneath_Optim_SDL_Linux)
No extra libs should be required to run.
To enable full color in the **Terminal** version, you most likely need to add `export TERM=xterm-256color` to your `~/.bashrc` file.


#### Mac OSX:


#### Windows:


---

## Compiling:

I made a compile script with python, I origonally used GNU MAKE, but I'm not well versed with that and I kept running into issues so I moved to python. I haven't compiled in a long time directly from Windows itself. So the windows builds are compiled on linux and mac using mingw32

To build the terminal version run the following in the repository directory:
```
python build.py
```
or
```
./build.py
```


To compile using the SDL2 port, use the -s flag: 
```
python build.py -s
```

If you need to completely rebuild all of the files you can use the -a flag to ensure no files are skipped.
```
python build.py -a
```

To run immediately after building use the -r flag.
```
python build.py -r
```

Flags can be used together.
All flags:
```
-h  List all flags
-s  Compile using SDL instead of Curses
-r  Run after successful build
-a  Compile all sources (it defaults to only ones that have changed)
-l  Link only, don't compile sources
-o  Optimization level 3 (default 0)
-d  Print all commands that the build.py is executing
-t  Do not remake ArtFiles.hpp
-i  Don't check included headers (makes compiling faster, but doesn't recompile cpp files if only included headers were modified)
-w  Use mingw32 compiler (Mac: i586-mingw32-c++, Linux: i686-w64-mingw32-c++)
--compiler COMPILER  Specify the compiler manually
```

---