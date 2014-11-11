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
#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>
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

using namespace std;

extern char cCurrentPath[FILENAME_MAX];

extern bool running;

extern int nextUniqueId;

extern string UnderneathDir;
extern string WorldsDir;

#endif /* defined(__Underneath__Global__) */
