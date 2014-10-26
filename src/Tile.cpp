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
    this->colorCodeUnseen = Ui::C_BLACK;

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



Tile *tileList[10];
Tile* tileAir;
Tile* tileEdge;
Tile* tileWall;

void initTiles() {

    int a = 0;

    //Technical
    tileAir = new Tile(a++, ".", '.', Ui::C_LIGHT_WHITE, tileFlagNone);
    tileEdge = new Tile(a++, "X", 'X',  Ui::C_LIGHT_RED, tileFlagNone);
    //Solids
    tileWall = new Tile(a++, "░", '#', Ui::C_LIGHT_WHITE, tileFlagSolid);
}

Tile* getTile(int index) {
    return tileList[index];
}
