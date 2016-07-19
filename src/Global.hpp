//
//  Global.hpp
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
#include <stdarg.h>


#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <assert.h>
#include <climits>

#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

#ifdef useSDLGraphics
    #include "SDLCursesPort.hpp"
    extern int ESCDELAY;
#endif

#ifdef __ANDROID__
	#include "../AndroidCursesInterface.hpp"
    extern int ESCDELAY;
#endif

#ifdef __NDS__
	#include "NDSPort.hpp"
    extern int ESCDELAY;
#endif

#if defined(_WIN32) || defined(__NDS__) || defined(__ANDROID__)

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

    #ifndef useSDLGraphics
        #include "curses.h"
        extern int ESCDELAY;
    #endif

#else

    #include <unistd.h>

    #define GetCurrentDir getcwd

    #ifndef useSDLGraphics
        #ifndef __ANDROID__
			#ifndef __NDS__
				#include <ncurses.h>
			#endif
        #endif
    #endif

#endif


#define forVector(list, i) for(int i=0;i<(int)list.size();i++)

#define repeat(n, i) for(int i=0;i<n;i++)

#define KEY_ESCAPE 27

int getchSafe();


extern bool running;

extern int nextUniqueId;

extern string UnderneathDir;
extern string WorldsDir;
extern string ArtDir;
extern string AudioDir;


#define defaultTimeout 100
#define fastestTimeout 0
#ifdef useSDLGraphics
	#define fastTimeout 10
#else
	#define fastTimeout 50
#endif

#ifdef NO_UNICODE
	#define SYMBOL_COIN "c"
    #define SYMBOL_TIME "t"
#else
    #define SYMBOL_COIN "¢"
    #define SYMBOL_TIME "⧖"
#endif



#endif /* defined(__Underneath__Global__) */
