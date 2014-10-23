//
//  Tile.h
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Tile__
#define __Underneath__Tile__

#include "Global.h"
#include "Ui.h"

enum TileFlag{
    tileFlagNone = 0,
    tileFlagSolid = 1 << 0,
};

class Tile{
public:

    Tile(int index, std::string icon, Ui::color colorCode, Ui::color colorCodeUnseen, TileFlag flags);

    std::string getIcon();
    int getIndex();
    int getColorCode(bool inView);

    bool isSolid(){
        return flags & tileFlagSolid;
    }

private:
    std::string icon;
    int index;
    Ui::color colorCode;
    Ui::color colorCodeUnseen;
    TileFlag flags;
};

extern Tile* tileList[10];
extern Tile* tileAir;
extern Tile* tileEdge;
extern Tile* tileWall;


void initTiles();
Tile* getTile(int index);

#endif /* defined(__Underneath__Tile__) */
