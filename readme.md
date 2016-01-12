# Underneath

A dungeon crawler with old-school style ASCII graphics.


### Written in c++11

This program uses standard c++ libraries and ncurses (pdcurses in Windows).
SDL version uses SDL2 and SDL2_Image.
The only currently working version with music uses the [YSE library](http://www.attr-x.net/yse/). I intend to replace this with a lighter and more open licence in the future.

My development is primarily in MacOSX and some in Linux. I have successfully compiled for Windows using MinGW on Linux and Mac. Good luck compiling locally on Windows, I haven't succesfully done that in a while.


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
-r  Run after successful build

-s  Compile using SDL instead of Curses
-y  Compile with the YSE Audio Library

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
