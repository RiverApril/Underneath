//
//  Art.h
//  Underneath
//
//  Created by Braeden Atlee on 2/1/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Art__
#define __Underneath__Art__

#include "Global.h"
#include "Geometry.h"

struct Art{

    vector<string> lines;

    Point2 getSize();

    void printAt(Point2 pos);

    void printCenter(int y, int xOffset = 0);
    
    
};

namespace Arts{

    void loadArts();

    int loadNew(string name, string ext = "txt", string dot = ".");

    extern vector<Art*> artList;

    extern int artTitle;
    extern int artScroll;
    extern int artKnife;

}

#endif /* defined(__Underneath__Art__) */
