//
//  Tile.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.h"
#include "Tile.h"

Tile::Tile(int index, std::string icon, Ui::color colorCode, Ui::color colorCodeUnseen, TileFlag flags){
    this->index = index;
    this->icon = icon;
    tileList[index] = this;

    this->colorCode = colorCode;
    this->colorCodeUnseen = colorCodeUnseen;

    this->flags = flags;
}

std::string Tile::getIcon(){
    return icon;
}

int Tile::getIndex(){
    return index;
}

int Tile::getColorCode(bool inView){
    return inView?colorCode:colorCodeUnseen;
}



Tile *tileList[10];
Tile* tileAir;
Tile* tileEdge;
Tile* tileWall;

void initTiles(){

    int a = 0;

    //Technical
    tileAir = new Tile(a++, ".", Ui::C_LIGHT_WHITE, Ui::C_LIGHT_BLACK, tileFlagNone);
    tileEdge = new Tile(a++, "X", Ui::C_LIGHT_RED, Ui::C_DARK_RED, tileFlagNone);

    //Solids
    tileWall = new Tile(a++, "░", Ui::C_LIGHT_WHITE, Ui::C_DARK_WHITE, tileFlagSolid);
}

Tile* getTile(int index){
    return tileList[index];
}
