//
//  Global.h
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Global__
#define __Underneath__Global__

#include <iostream>
#include <sstream>
#include <ostream>
#include <vector>
#include <queue>
#include <map>
#include <functional>


#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <assert.h>

#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
    #include <memory>
    #include <direct.h>
    #include <stdint.h>
    #include "curses.hpp"
    extern int ESCDELAY;
    #define GetCurrentDir _getcwd
    int mkdir(const char *filename, int useless);

    template <typename T>
    std::string to_string(T a) {
        std::stringstream ss;
        ss << a;
        return ss.str();
    }

#else
    #include <unistd.h>
    #include <ncurses.h>
    #define GetCurrentDir getcwd
#endif


#undef COLOR_WHITE
#undef COLOR_RED
#undef COLOR_BLUE
#undef COLOR_GREEN
#undef COLOR_YELLOW
#undef COLOR_MAGENTA
#undef COLOR_CYAN
#undef COLOR_BLUE
#undef COLOR_BLACK

#define forVector(list, i) for(int i=0;i<list.size();i++)

#define repeat(n, i) for(int i=0;i<n;i++)

#define KEY_ESCAPE 27

using namespace std;

extern char workingDirectory[FILENAME_MAX];

extern bool running;

extern int nextUniqueId;

extern bool godMode;

extern string CustomWorkingDirectory;
extern string UnderneathDir;
extern string WorldsDir;
extern string ArtDir;

#define defaultTimeout 100



#endif /* defined(__Underneath__Global__) */
