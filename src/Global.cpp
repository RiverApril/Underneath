//
//  Global.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"

#ifdef _WIN32
int ESCDELAY;

int mkdir(const char *filename, int useless) {
    return mkdir(filename);
}
#endif

char workingDirectory[FILENAME_MAX];

bool running;

int nextUniqueId;

bool godMode = false;

string CustomWorkingDirectory;
string UnderneathDir;
string WorldsDir;
string ArtDir;
