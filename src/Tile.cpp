//
//  Tile.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.h"
#include "Tile.h"

Tile::Tile(int index, char icon, Ui::color colorCode, TileFlag flags) {
    this->index = index;
    this->icon = icon;
    tileList[index] = this;

    this->colorCode = colorCode;
    this->colorCodeUnseen = Ui::C_DARK_GRAY;

    this->flags = flags;
}

char Tile::getIcon() {
    return icon;
}

int Tile::getIndex() {
    return index;
}

int Tile::getColorCode(bool inView) {
    return inView?colorCode:colorCodeUnseen;
}



Tile* tileList[20];
Tile* tileFloor;
Tile* tilePath;
Tile* tileWall;
Tile* tileDoor;
Tile* tileSecretDoor;
Tile* tileOpenDoor;

Tile* tileStairDown;
Tile* tileStairUp;

Tile* tileEdge;
Tile* tileUnset;

Tile* tileDebug1;
Tile* tileDebug2;
Tile* tileDebug3;
Tile* tileDebug4;
Tile* tileDebug5;
Tile* tileDebug6;

void initTiles() {

    int a = 0;

    tileFloor = new Tile(a++, '.', Ui::C_LIGHT_WHITE, tileFlagNone);
    tilePath = new Tile(a++, '.', Ui::C_LIGHT_GRAY, tileFlagNone);

    tileWall = new Tile(a++, '#', Ui::C_LIGHT_WHITE, tileFlagSolid | tileFlagTall);
    tileDoor = new Tile(a++, '%', Ui::C_LIGHT_YELLOW, tileFlagSolid | tileFlagTall);
    tileSecretDoor = new Tile(a++, '#', Ui::C_LIGHT_WHITE, tileFlagSolid | tileFlagTall);
    tileOpenDoor = new Tile(a++, '/', Ui::C_LIGHT_YELLOW, tileFlagNone);

    tileStairDown = new Tile(a++, 'v',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileStairUp = new Tile(a++, '^',  Ui::C_LIGHT_GREEN, tileFlagNone);

    tileEdge = new Tile(a++, 'X',  Ui::C_LIGHT_RED, tileFlagSolid);
    tileUnset = new Tile(a++, 'X',  Ui::C_LIGHT_MAGENTA, tileFlagNone);

    tileDebug1 = new Tile(a++, '1',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug2 = new Tile(a++, '2',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug3 = new Tile(a++, '3',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug4 = new Tile(a++, '4',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug5 = new Tile(a++, '5',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug6 = new Tile(a++, '6',  Ui::C_LIGHT_GREEN, tileFlagNone);
}

Tile* getTile(int index) {
    return tileList[index];
}
