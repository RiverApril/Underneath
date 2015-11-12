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
#else
	#ifdef useSDLLightCurses
		int ESCDELAY;
	#endif
#endif

#ifdef __ANDROID__
	int ESCDELAY;
#endif

char workingDirectory[FILENAME_MAX];

bool running;

int nextUniqueId;

string CustomWorkingDirectory;
string UnderneathDir;
string WorldsDir;
string ArtDir;
