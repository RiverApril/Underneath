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
    tileFlagDoor = 1 << 2,

};

inline TileFlag operator|(TileFlag a, TileFlag b){
    return static_cast<TileFlag>(static_cast<int>(a)|static_cast<int>(b));
}

class Tile {
public:

    Tile(int index, char icon, Ui::color fgColor, Ui::color bgColor, TileFlag flags, Ui::color fgColorUnseen = Ui::C_DARK_GRAY, Ui::color bgColorUnseen = Ui::C_BLACK);

    char getIcon();
    int getIndex();
    Ui::color getFgColor(bool inView);
    Ui::color getBgColor(bool inView);

    bool isSolid() {
        return flags & tileFlagSolid;
    }

    bool isTall() {
        return flags & tileFlagTall;
    }

    bool hasFlag(TileFlag flag) {
        return flags & flag;
    }

private:
    char icon;
    int index;
    Ui::color fgColor;
    Ui::color bgColor;
    Ui::color fgColorUnseen;
    Ui::color bgColorUnseen;
    TileFlag flags;
};

extern Tile* tileList[20];
extern Tile* tileFloor;
extern Tile* tilePath;
extern Tile* tileWall;
extern Tile* tileDoor;
extern Tile* tileSecretDoor;
extern Tile* tileOpenDoor;

extern Tile* tileStairDown;
extern Tile* tileStairUp;

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
