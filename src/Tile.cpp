//
//  Tile.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.h"
#include "Tile.h"

Tile::Tile(char icon, string name, Ui::Color fgColor, Ui::Color bgColor, TileFlag flags, Ui::Color fgColorUnseen, Ui::Color bgColorUnseen) {
    this->icon = icon;
    this->name = name;
    this->index = (int) Tiles::tileList.size();
    Tiles::tileList.push_back(this);

    this->fgColor = fgColor;
    this->bgColor = bgColor;
    this->fgColorUnseen = fgColorUnseen;
    this->bgColorUnseen = bgColorUnseen;

    this->flags = flags;
}

char Tile::getIcon() {
    return icon;
}

int Tile::getIndex() {
    return index;
}

string Tile::getName() {
    return name;
}

Ui::Color Tile::getFgColor(bool inView) {
    return inView ? fgColor : fgColorUnseen;
}

Ui::Color Tile::getBgColor(bool inView) {
    return inView ? bgColor : bgColorUnseen;
}

namespace Tiles {

    vector<Tile*> tileList;


    Tile* tileFloor;
    //Tile* tilePath;

    Tile* tileWall;
    Tile* tilePonyWall;

    Tile* tileDoor;
    Tile* tileSecretDoor;
    Tile* tileOpenDoor;

    Tile* tileStairUp;
    Tile* tileStairDown;

    Tile* tileChest;
    Tile* tileCrate;

    Tile* tileRubble;

    Tile* tileEdge;
    Tile* tileUnset;

    Tile* tileDebug1;
    Tile* tileDebug2;
    Tile* tileDebug3;
    Tile* tileDebug4;
    Tile* tileDebug5;
    Tile* tileDebug6;

    Tile* tileTemp;

    void initTiles() {

        using namespace Ui;

        Color bg = C_BLACK;

        tileFloor = new Tile('.', "floor", C_WHITE, bg, tileFlagPathable);
        //tilePath = new Tile('.', C_LIGHT_GRAY, bg, tileFlagPathable);

        tileWall = new Tile(' ', "wall", C_BLACK, C_WHITE, tileFlagSolid | tileFlagTall, C_BLACK,
                C_DARK_GRAY);
        tilePonyWall = new Tile('.', "pony wall", C_BLACK, C_LIGHT_GRAY, tileFlagSolid, C_BLACK, C_DARK_GRAY);

        tileDoor = new Tile('%', "door", C_BLACK, C_WHITE, tileFlagSolid | tileFlagTall | tileFlagDoor | tileFlagPathable | tileFlagHasTileEntity, C_BLACK, C_DARK_GRAY);
        tileSecretDoor = new Tile(' ', "definitely a wall", C_BLACK, C_WHITE, tileFlagSolid | tileFlagTall | tileFlagDoor | tileFlagSecretPathable | tileFlagHasTileEntity, C_BLACK, C_DARK_GRAY);
        tileOpenDoor = new Tile('/', "open door", C_WHITE, bg, tileFlagPathable);



        tileStairUp = new Tile('^', "stair up",  C_LIGHT_GREEN, bg, tileFlagPathable | tileFlagHasTileEntity | tileFlagIndestructable);
        tileStairDown = new Tile('v', "stair down",  C_LIGHT_GREEN, bg, tileFlagPathable | tileFlagHasTileEntity | tileFlagIndestructable);

        tileChest = new Tile('+', "chest", C_LIGHT_GREEN, bg, tileFlagSolid | tileFlagHasTileEntity);
        tileCrate = new Tile('+', "crate", C_DARK_YELLOW, bg, tileFlagSolid | tileFlagHasTileEntity);


        tileRubble = new Tile('#', "rubble", C_LIGHT_GRAY, bg, tileFlagPathable, C_DARK_GRAY, bg);

        
        tileEdge = new Tile('.', "edge",  C_LIGHT_RED, C_WHITE, tileFlagSolid | tileFlagTall | tileFlagIndestructable);
        tileUnset = new Tile('X', "unset",  C_LIGHT_MAGENTA, bg, tileFlagSolid | tileFlagTall | tileFlagIndestructable);


        tileDebug1 = new Tile('1', "debug 1", C_LIGHT_GREEN, bg, tileFlagPathable);
        tileDebug2 = new Tile('2', "debug 2", C_LIGHT_GREEN, bg, tileFlagPathable);
        tileDebug3 = new Tile('3', "debug 3", C_LIGHT_GREEN, bg, tileFlagPathable);
        tileDebug4 = new Tile('4', "debug 4", C_LIGHT_GREEN, bg, tileFlagPathable);
        tileDebug5 = new Tile('5', "debug 5", C_LIGHT_GREEN, bg, tileFlagPathable);
        tileDebug6 = new Tile('6', "debug 6", C_LIGHT_GREEN, bg, tileFlagPathable);

        tileTemp = new Tile('?', "temp", C_LIGHT_RED, bg, tileFlagPathable);
    }

    void cleanupTiles() {
        for (Tile* tile : tileList) {
            delete tile;
        }
    }

    Tile* getTile(int index) {
        return tileList[index];
    }
}
