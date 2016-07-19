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
	#ifdef useSDLGraphics
		int ESCDELAY;
	#endif
#endif

#ifdef __ANDROID__
	int ESCDELAY;
#endif

#ifdef __NDS__
    int ESCDELAY;
#endif

int getchSafe(){
    int g = getch();
    if(g == KEY_ESCAPE){//27
        int gg = getch();
        int ic = 1;
        while(gg != -1){
            g |= gg << 8*ic;
            gg = getch();
            ic++;
        }
    }
    return g;
}

bool running;

int nextUniqueId;

string UnderneathDir;
string WorldsDir;
string ArtDir;
string AudioDir;
