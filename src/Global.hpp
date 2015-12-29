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
#include <algorithm>
#include <stdint.h>


#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <assert.h>

#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

#ifdef useSDLLightCurses
    #include "LC_SDLLightcurses.hpp"
    extern int ESCDELAY;
#endif

#ifdef __ANDROID__
	#include "../AndroidCursesInterface.hpp"
    extern int ESCDELAY;

    template <typename T>
    string to_string(T a) {
        stringstream ss;
        ss << a;
        return ss.str();
    }
#endif

#ifdef _WIN32

    #include <memory>
    #include <direct.h>
    #include <stdint.h>
    #define GetCurrentDir _getcwd
    int mkdir(const char *filename, int useless);

    template <typename T>
    string to_string(T a) {
        stringstream ss;
        ss << a;
        return ss.str();
    }

    #ifndef useSDLLightCurses
        #include "curses.h"
        extern int ESCDELAY;
    #endif

#else

    #include <unistd.h>

    #define GetCurrentDir getcwd

    #ifndef useSDLLightCurses
        #ifndef __ANDROID__
            #include <ncurses.h>
        #endif
    #endif

#endif

#define forVector(list, i) for(int i=0;i<(int)list.size();i++)

#define repeat(n, i) for(int i=0;i<n;i++)

#define KEY_ESCAPE 27

int getchSafe();


extern char workingDirectory[FILENAME_MAX];

extern bool running;

extern int nextUniqueId;

extern string CustomWorkingDirectory;
extern string UnderneathDir;
extern string WorldsDir;
extern string ArtDir;


#define defaultTimeout 100
#define fastTimeout 10




#endif /* defined(__Underneath__Global__) */
