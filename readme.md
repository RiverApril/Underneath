# Underneath

A dungeon crawler with old-school style ASCII graphics.


### Written in c++11

This program uses standard c++ libraries and ncurses (pdcurses in Windows).
SDL version uses SDL2 and SDL2_Image.

My development is primarily in MacOSX and some in Linux. I have successfully compiled for Windows using MinGW on Linux and Mac. Good luck compiling locally on Windows, I haven't succesfully done that in a while.


---

## Latest Builds:

#### Linux:
##### Terminal version:
- Executable: [Underneath_Optim_Term_Linux](https://github.com/user/repo/raw/master/Underneath_Optim_Term_Linux)
- To enable full color, you most likely need to add `export TERM=xterm-256color` to your `~/.bashrc` file.

##### SDL2 version:
- Executable: [Underneath_Optim_SDL_Linux](https://github.com/user/repo/raw/master/Underneath_Optim_SDL_Linux)
- Font image file: [font.png](https://github.com/user/repo/raw/master/font.png)

---

#### Mac OSX:
##### Terminal version:
- Executable: [Underneath_Optim_Term_OSX](https://github.com/user/repo/raw/master/Underneath_Optim_Term_OSX)

##### SDL2 version:
- Executable: [Underneath_Optim_SDL_OSX](https://github.com/user/repo/raw/master/Underneath_Optim_SDL_OSX)
- Font image file: [font.png](https://github.com/user/repo/raw/master/font.png)

---

#### Windows:
##### Terminal version:
- Executable: [Underneath_Optim_Term_Windows.exe](https://github.com/user/repo/raw/master/Underneath_Optim_Term_Windows.exe)
- DLL file: [pdcurses.dll](https://github.com/user/repo/raw/master/pdcurses.dll)

##### SDL2 version:
- Executable: [Underneath_Optim_SDL_Windows.exe](https://github.com/user/repo/raw/master/Underneath_Optim_SDL_Windows.exe)
- Font image file: [font.png](https://github.com/user/repo/raw/master/font.png)
- DLL files:
  - [SDL2.dll](https://github.com/user/repo/raw/master/SDL2.dll)
  - [SDL2_image.dll](https://github.com/user/repo/raw/master/SDL2_image.dll)
  - [zlib1.dll](https://github.com/user/repo/raw/master/zlib1.dll)
  - [libpng16-16.dll](https://github.com/user/repo/raw/master/libpng16-16.dll)

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
