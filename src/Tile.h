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
    tileFlagTall = 1 << 1,

};

inline TileFlag operator|(TileFlag a, TileFlag b){
	return static_cast<TileFlag>(static_cast<int>(a)|static_cast<int>(b));
}

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

    bool isTall() {
        return flags & tileFlagTall;
    }

private:
    std::string icon;
    char iconAlt;
    int index;
    Ui::color colorCode;
    Ui::color colorCodeUnseen;
    TileFlag flags;
};

extern Tile* tileList[20];
extern Tile* tileFloor;
extern Tile* tilePath;
extern Tile* tileWall;

extern Tile* tileEdge;
extern Tile* tileUnset;

extern Tile* tileDebug1;
extern Tile* tileDebug2;
extern Tile* tileDebug3;
extern Tile* tileDebug4;
extern Tile* tileDebug5;
extern Tile* tileDebug6;


void initTiles();
Tile* getTile(int index);

#endif /* defined(__Underneath__Tile__) */
