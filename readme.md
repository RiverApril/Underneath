Underneath
==========

A dungeon crawler with ASCII graphics lightly based on the classic game Rogue
----------------------------------------------------------

### Written in c++ 11

This program uses standard c++ libraries and ncurses (pdcurses in Windows).
SDL version uses SDL2 and SDL_Image.

My development is primarily in Linux and some in MacOSX. I have successfully built on windows, but its a pain in the ass snd Windows command promt only supports 8 colors, witch doesn't look as good as 16.


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

To run imediatly after building use the -r flag.
```
python build.py -r
```

Flags can be used together.

---
