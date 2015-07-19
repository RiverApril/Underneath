//
//  Tile.h
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Tile__
#define __Underneath__Tile__

#include "Global.hpp"
#include "Ui.hpp"
#include "Icon.hpp"

typedef int TileFlag;
const TileFlag tileFlagSolid = 1 << 0;
const TileFlag tileFlagTall = 1 << 1;
const TileFlag tileFlagDoor = 1 << 2;
const TileFlag tileFlagPathable = 1 << 3;
const TileFlag tileFlagSecretPathable = 1 << 4;
const TileFlag tileFlagHasTileEntity = 1 << 5;
const TileFlag tileFlagIndestructable = 1 << 6;
const TileFlag tileFlagReplaceable = 1 << 7;
const TileFlag tileFlagAll = 0xFFFFFFFF;

class Tile {
public:

    Tile(Icon* icon, string name, TileFlag flags, Icon* unseen);

    Tile(Icon* icon, string name, TileFlag flags);

    ~Tile(){
        delete icon;
        delete iconUnseen;
    }

    Icon* getIcon(bool inView);
    int getIndex();
    string getName();
    //Ui::Color getFgColor(bool inView);
    //Ui::Color getBgColor(bool inView);

    bool isSolid() {
        return flags & tileFlagSolid;
    }

    bool isTall() {
        return flags & tileFlagTall;
    }

    bool hasFlag(TileFlag flag) {
        return flags & flag;
    }

    bool doesNotHaveFlag(TileFlag flag) {
        return !(flags & flag);
    }

private:
    Icon* icon;
    Icon* iconUnseen;
    int index;
    string name;
    //Ui::Color fgColor;
    //Ui::Color bgColor;
    //Ui::Color fgColorUnseen;
    //Ui::Color bgColorUnseen;
    TileFlag flags;
};

namespace Tiles {

    extern vector<Tile*> tileList;

    extern Tile* tileFloor;
    extern Tile* tileBloodFloor;
    //extern Tile* tilePath;

    extern Tile* tileWall;
    extern Tile* tilePonyWall;

    extern Tile* tileDoor;
    extern Tile* tileSecretDoor;
    extern Tile* tileOpenDoor;

    extern Tile* tileStairUp;
    extern Tile* tileStairDown;

    extern Tile* tileChest;
    extern Tile* tileCrate;

    extern Tile* tileRubble;
    extern Tile* tileCorpse;

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
