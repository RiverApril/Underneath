//
//  Tile.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.h"
#include "Tile.h"

Tile::Tile(int index, std::string icon, char iconAlt, Ui::color colorCode, TileFlag flags) {
    this->index = index;
    this->icon = icon;
    this->iconAlt = iconAlt;
    tileList[index] = this;

    this->colorCode = colorCode;
    this->colorCodeUnseen = Ui::C_DARK_GRAY;

    this->flags = flags;
}

std::string Tile::getIcon() {
    return icon;
}

char Tile::getIconAlt() {
    return iconAlt;
}

int Tile::getIndex() {
    return index;
}

int Tile::getColorCode(bool inView) {
    return inView?colorCode:colorCodeUnseen;
}



Tile* tileList[10];
Tile* tileAir;
Tile* tileEdge;
Tile* tileDebug1;
Tile* tileDebug2;
Tile* tileDebug3;
Tile* tileDebug4;
Tile* tileDebug5;
Tile* tileDebug6;
Tile* tileWall;

void initTiles() {

    int a = 0;

    tileAir = new Tile(a++, ".", '.', Ui::C_LIGHT_WHITE, tileFlagNone);

    tileDebug1 = new Tile(a++, "1", '1',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug2 = new Tile(a++, "2", '2',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug3 = new Tile(a++, "3", '3',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug4 = new Tile(a++, "4", '4',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug5 = new Tile(a++, "5", '5',  Ui::C_LIGHT_GREEN, tileFlagNone);
    tileDebug6 = new Tile(a++, "6", '6',  Ui::C_LIGHT_GREEN, tileFlagNone);
    
    tileEdge = new Tile(a++, "X", 'X',  Ui::C_LIGHT_RED, tileFlagSolid);

    tileWall = new Tile(a++, "░", '#', Ui::C_LIGHT_WHITE, tileFlagSolid);
}

Tile* getTile(int index) {
    return tileList[index];
}
