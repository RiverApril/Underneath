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

enum TileFlag {
    tileFlagNone = 0,
    tileFlagSolid = 1 << 0,
};

class Tile {
public:

    Tile(int index, std::string icon, char iconAlt, Ui::color colorCode, TileFlag flags);

    std::string getIcon();
    char getIconAlt();
    int getIndex();
    int getColorCode(bool inView);

    bool isSolid() {
        return flags & tileFlagSolid;
    }

private:
    std::string icon;
    char iconAlt;
    int index;
    Ui::color colorCode;
    Ui::color colorCodeUnseen;
    TileFlag flags;
};

extern Tile* tileList[10];
extern Tile* tileAir;
extern Tile* tileEdge;
extern Tile* tileDebug1;
extern Tile* tileDebug2;
extern Tile* tileDebug3;
extern Tile* tileDebug4;
extern Tile* tileDebug5;
extern Tile* tileDebug6;
extern Tile* tileWall;


void initTiles();
Tile* getTile(int index);

#endif /* defined(__Underneath__Tile__) */
