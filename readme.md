Underneath
==========

A dungeon crawler with ASCII graphics lightly based on the classic game Rogue
----------------------------------------------------------

### Written in c++ 11

This program uses standard c++ libraries and ncurses (pdcurses in Windows).
SDL version uses SDL2 and SDL_Image.

My development is primarily in Linux and some in MacOSX. I have successfully compiled for Windows using mingw32 on Linux.


---

### Compiling:

I made a compile script with python, I origonally used GNU MAKE, but I'm not well versed with that and I kept running into issues so I moved to python. Windows compilation is difficult and I've run into many issues with that, so good luck.

To build the terminal version run the following in the repository directory:
```
python build.py
```
or
```
./build.py
```


To try the work-in-progress SDL2 port use the -s flag:
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
-t  Do not recompile ArtFiles.hpp
-w  Use "i686-w64-mingw32-c++" as the compiler (this is to build for windows on linux)
```

---
