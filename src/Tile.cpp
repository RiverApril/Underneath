//
//  Tile.cpp
//  Underneath
//
//  Created by Braeden Atlee on 9/29/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"
#include "Tile.hpp"

Tile::Tile(Icon* icon, string name, TileFlag flags, Icon* unseen) {
    this->icon = icon;
    this->iconUnseen = unseen;
    this->name = name;
    this->index = (int) Tiles::tileList.size();
    Tiles::tileList.push_back(this);

    this->flags = flags;
}

Tile::Tile(Icon* icon, string name, TileFlag flags) : Tile(icon, name, flags, new BasicIcon(icon->getChar(0, Point2Neg1, nullptr), Ui::C_DARK_GRAY, Ui::C_BLACK)) {
}

Icon* Tile::getIcon(bool inView) {
    return inView?icon:iconUnseen;
}

int Tile::getIndex() {
    return index;
}

string Tile::getName() {
    return name;
}

/*Ui::Color Tile::getFgColor(bool inView) {
    return inView ? fgColor : fgColorUnseen;
}

Ui::Color Tile::getBgColor(bool inView) {
    return inView ? bgColor : bgColorUnseen;
}*/

namespace Tiles {

    vector<Tile*> tileList;


    Tile* tileFloor;
    //Tile* tilePath;
    Tile* tileBloodFloor;

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

        tileFloor = new Tile(new BasicIcon('.', C_WHITE, C_BLACK), "floor", tileFlagPathable);
        tileBloodFloor = new Tile(new BasicIcon('.', C_LIGHT_RED, C_BLACK), "bloody floor", tileFlagPathable);

        tileWall = new Tile(new BasicIcon(' ', C_BLACK, C_WHITE), "wall", tileFlagSolid | tileFlagTall, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));
        tilePonyWall = new Tile(new BasicIcon('.', C_BLACK, C_LIGHT_GRAY), "pony wall", tileFlagSolid, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));

        tileDoor = new Tile(new BasicIcon('%', C_BLACK, C_WHITE), "door", tileFlagSolid | tileFlagTall | tileFlagDoor | tileFlagPathable | tileFlagHasTileEntity, new BasicIcon('%', C_BLACK, C_DARK_GRAY));
        tileSecretDoor = new Tile(new BasicIcon(' ', C_BLACK, C_WHITE), "definitely a wall", tileFlagSolid | tileFlagTall | tileFlagDoor | tileFlagSecretPathable | tileFlagHasTileEntity, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));
        tileOpenDoor = new Tile(new BasicIcon('/', C_WHITE, bg), "open door", tileFlagPathable);



        tileStairUp = new Tile(new BasicIcon('^',  C_LIGHT_GREEN, bg), "stair up", tileFlagPathable | tileFlagHasTileEntity | tileFlagIndestructable);
        tileStairDown = new Tile(new BasicIcon('v',  C_LIGHT_GREEN, bg), "stair down", tileFlagPathable | tileFlagHasTileEntity | tileFlagIndestructable);

        tileChest = new Tile(new BasicIcon('+', C_LIGHT_GREEN, bg), "chest", tileFlagSolid | tileFlagHasTileEntity);
        tileCrate = new Tile(new BasicIcon('+', C_DARK_YELLOW, bg), "crate", tileFlagSolid | tileFlagHasTileEntity);


        tileRubble = new Tile(new RandomIcon({'#', '%', '&'}, C_LIGHT_GRAY, bg), "rubble", tileFlagPathable, new RandomIcon({'#', '%', '&'}, C_DARK_GRAY, bg));

        
        tileEdge = new Tile(new BasicIcon('.',  C_LIGHT_RED, C_WHITE), "edge", tileFlagSolid | tileFlagTall | tileFlagIndestructable, new BasicIcon('.', C_BLACK, C_DARK_GRAY));
        tileUnset = new Tile(new BasicIcon('X',  C_LIGHT_MAGENTA, bg), "unset", tileFlagSolid | tileFlagTall | tileFlagIndestructable);


        tileDebug1 = new Tile(new BasicIcon('1', C_LIGHT_GREEN, bg), "debug 1", tileFlagPathable);
        tileDebug2 = new Tile(new BasicIcon('2', C_LIGHT_GREEN, bg), "debug 2", tileFlagPathable);
        tileDebug3 = new Tile(new BasicIcon('3', C_LIGHT_GREEN, bg), "debug 3", tileFlagPathable);
        tileDebug4 = new Tile(new BasicIcon('4', C_LIGHT_GREEN, bg), "debug 4", tileFlagPathable);
        tileDebug5 = new Tile(new BasicIcon('5', C_LIGHT_GREEN, bg), "debug 5", tileFlagPathable);
        tileDebug6 = new Tile(new BasicIcon('6', C_LIGHT_GREEN, bg), "debug 6", tileFlagPathable);

        tileTemp = new Tile(new BasicIcon('?', C_LIGHT_RED, bg), "temp", tileFlagPathable);
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
