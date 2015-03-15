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

    Point2 calcSize();
    Point2 getSize();

    void printAt(Point2 pos);

    void printCenter(int y, int xOffset = 0);

    Point2 size;
    
};

namespace Arts{

    void loadArts();
    void cleanupArts();

    int loadNew(string name, string ext = "txt", string dot = ".");
    
    int getArtFromName(string name);

    Art* getArt(int index);

    extern vector<Art*> artList;

    extern Art* defaultArt;

    extern int artTitle;

    extern int artScroll;
    extern int artCoin;
    
    extern int artKnife;
    extern int artShortSword;
    extern int artLongSword;
    extern int artMace;
    extern int artSpear;
    extern int artBattleAxe;

    extern int artLongbow;
    extern int artRecurveBow;
    extern int artCrossbow;

}

#endif /* defined(__Underneath__Art__) */
