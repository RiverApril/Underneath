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
#include <vector>
#include <queue>

#include <ncurses.h>
#undef COLOR_WHITE
#undef COLOR_RED
#undef COLOR_BLUE
#undef COLOR_GREEN
#undef COLOR_YELLOW
#undef COLOR_MAGENTA
#undef COLOR_CYAN
#undef COLOR_BLUE
#undef COLOR_BLACK

#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <assert.h>

#include <stdio.h>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


#define forVector(list, i) for(size_t i=0;i<list.size();i++)

#define repeat(n, i) for(int i=0;i<n;i++)

#define KEY_ESCAPE 27

using namespace std;

extern char workingDirectory[FILENAME_MAX];

extern bool running;

extern int nextUniqueId;

extern string UnderneathDir;
extern string WorldsDir;


#endif /* defined(__Underneath__Global__) */
