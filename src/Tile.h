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

typedef int TileFlag;
const TileFlag tileFlagSolid = 1 << 0;
const TileFlag tileFlagTall = 1 << 1;
const TileFlag tileFlagDoor = 1 << 2;
const TileFlag tileFlagPathable = 1 << 3;
const TileFlag tileFlagSecretPathable = 1 << 4;
const TileFlag tileFlagHasTileEntity = 1 << 5;

class Tile {
public:

    Tile(char icon, Ui::Color fgColor, Ui::Color bgColor, TileFlag flags, Ui::Color fgColorUnseen = Ui::C_DARK_GRAY, Ui::Color bgColorUnseen = Ui::C_BLACK);

    char getIcon();
    int getIndex();
    Ui::Color getFgColor(bool inView);
    Ui::Color getBgColor(bool inView);

    bool isSolid() {
        return flags & tileFlagSolid;
    }

    bool isTall() {
        return (flags & tileFlagTall) != 0;
    }

    bool hasFlag(TileFlag flag) {
        return (flags & flag) != 0;
    }

private:
    char icon;
    int index;
    Ui::Color fgColor;
    Ui::Color bgColor;
    Ui::Color fgColorUnseen;
    Ui::Color bgColorUnseen;
    TileFlag flags;
};

namespace Tiles{

    extern vector<Tile*> tileList;
    extern Tile* tileFloor;
    extern Tile* tilePath;

    extern Tile* tileWall;
    extern Tile* tilePonyWall;

    extern Tile* tileDoor;
    extern Tile* tileSecretDoor;
    extern Tile* tileOpenDoor;

    extern Tile* tileStairUp;
    extern Tile* tileStairDown;

    extern Tile* tileChest;
    extern Tile* tileCrate;

    extern Tile* tileEdge;
    extern Tile* tileUnset;

    extern Tile* tileDebug1;
    extern Tile* tileDebug2;
    extern Tile* tileDebug3;
    extern Tile* tileDebug4;
    extern Tile* tileDebug5;
    extern Tile* tileDebug6;

    extern Tile* tileTemp;


    void initTiles();
    void cleanupTiles();
    Tile* getTile(int index);

}

#endif /* defined(__Underneath__Tile__) */
