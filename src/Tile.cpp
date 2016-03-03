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

    this->flags = flags | tileFlagIsTile;
}

Tile::Tile(Icon* icon, string name, TileFlag flags) : Tile(icon, name, flags, new BasicIcon(icon->getChar(0, Point2Neg1, nullptr), C_DARK_GRAY, C_BLACK)) {
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
    Tile* tileHall;
    Tile* tileBloodFloor;

    Tile* tileWall;
    Tile* tilePonyWall;
    Tile* tileBookcase;

    Tile* tileDoor;
    Tile* tileSecretDoor;
    Tile* tileNonThreateningDoor;
    Tile* tileOpenDoor;
    Tile* tileLockedDoor;

    Tile* tileStairUp;
    Tile* tileStairDown;

    Tile* tileChest;
    Tile* tileCrate;

    Tile* tileRubble;
    Tile* tileCorpse;
    Tile* tileBones;

    Tile* tileTree;
    Tile* tileGrass;
    Tile* tilePath;

    Tile* tileFire;
    Tile* tileAsh;

    Tile* tilePillar;
    Tile* tileBreakable;

    Tile* tileWater;

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

        tileFloor = new Tile(new BasicIcon('.', C_WHITE, C_BLACK), "floor", tileFlagPathable | tileFlagReplaceable);
        tileHall = new Tile(new BasicIcon('.', C_WHITE, C_BLACK), "hall", tileFlagPathable | tileFlagReplaceable);
        tileBloodFloor = new Tile(new BasicIcon('.', C_LIGHT_RED, C_BLACK), "bloody floor", tileFlagPathable | tileFlagReplaceable);

        tileWall = new Tile(new BasicIcon(' ', C_BLACK, C_WHITE), "wall", tileFlagSolidBoth, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));
        tilePonyWall = new Tile(new BasicIcon(' ', C_BLACK, C_LIGHT_GRAY), "pony wall", tileFlagSolidOnGround, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));
        tileBookcase = new Tile(new BasicIcon('=', C_WHITE, C_BLACK), "bookcase", tileFlagSolidBoth, new BasicIcon('=', C_DARK_GRAY, C_BLACK));

        tileDoor = new Tile(new BasicIcon('%', C_BLACK, C_WHITE), "door", tileFlagSolidBoth | tileFlagDoor | tileFlagPathable | tileFlagHasTileEntity | tileFlagFlammable | tileFlagMonsterSpawningDoor, new BasicIcon('%', C_BLACK, C_DARK_GRAY));
        tileNonThreateningDoor = new Tile(new BasicIcon('%', C_BLACK, C_WHITE), "door", tileFlagSolidBoth | tileFlagDoor | tileFlagPathable | tileFlagHasTileEntity | tileFlagFlammable, new BasicIcon('%', C_BLACK, C_DARK_GRAY));
        tileSecretDoor = new Tile(new BasicIcon(' ', C_BLACK, C_WHITE), "definitely a wall", tileFlagSolidBoth | tileFlagDoor | tileFlagSecretPathable | tileFlagHasTileEntity | tileFlagFlammable | tileFlagMonsterSpawningDoor, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));
        tileOpenDoor = new Tile(new BasicIcon('/', C_WHITE, bg), "open door", tileFlagPathable | tileFlagFlammable);
        tileLockedDoor = new Tile(new BasicIcon('%', C_LIGHT_MAGENTA, C_WHITE), "locked door", tileFlagSolidBoth | tileFlagSecretPathable, new BasicIcon('%', C_DARK_MAGENTA, C_DARK_GRAY));


        tileStairUp = new Tile(new BasicIcon('^',  C_LIGHT_GREEN, bg), "stair up", tileFlagPathable | tileFlagHasTileEntity | tileFlagIndestructable);
        tileStairDown = new Tile(new BasicIcon('v',  C_LIGHT_GREEN, bg), "stair down", tileFlagPathable | tileFlagHasTileEntity | tileFlagIndestructable);

        tileChest = new Tile(new BasicIcon('+', C_LIGHT_GREEN, bg), "chest", tileFlagSolidOnGround | tileFlagHasTileEntity | tileFlagFlammable);
        tileCrate = new Tile(new BasicIcon('+', C_DARK_YELLOW, bg), "crate", tileFlagSolidOnGround | tileFlagHasTileEntity | tileFlagPathable | tileFlagFlammable);


        tileRubble = new Tile(new RandomIcon({'#', '#', ','}, C_LIGHT_GRAY, bg), "rubble", tileFlagPathable | tileFlagReplaceable, new BasicIcon('#', C_DARK_GRAY, bg));
        tileCorpse = new Tile(new BasicIcon('&', C_LIGHT_RED, bg), "corpse", tileFlagPathable | tileFlagReplaceable | tileFlagFlammable, new BasicIcon('&', C_DARK_GRAY, bg));
        tileBones = new Tile(new BasicIcon('&', C_LIGHT_GRAY, bg), "bones", tileFlagPathable | tileFlagReplaceable, new BasicIcon('.', C_DARK_GRAY, bg));

        tileTree = new Tile(new RandomColorIcon({'T', 't'}, {C_LIGHT_GREEN, C_LIGHT_GREEN, C_DARK_GREEN}, {bg}), "tree", tileFlagSolidBoth | tileFlagFlammable, new RandomIcon({'T', 't'}, C_DARK_GRAY, bg));
        tileGrass = new Tile(new RandomColorIcon({',', '.', '\'', '`', ' ', ' ', ' '}, {C_LIGHT_GREEN, C_LIGHT_GREEN, C_DARK_GREEN}, {bg}), "grass", tileFlagPathable | tileFlagReplaceable | tileFlagFlammable, new RandomIcon({',', '.', '\'', '`', ' ', ' ', ' '}, C_DARK_GRAY, bg));
        tilePath = new Tile(new BasicIcon('.', C_DARK_YELLOW, bg), "path", tileFlagPathable | tileFlagReplaceable, new BasicIcon({'.'}, C_DARK_GRAY, bg));

        tileFire = new Tile(new ColorAnimatedIcon({',', '.', '\'', '`'}, 1, {C_LIGHT_RED, C_LIGHT_YELLOW, C_LIGHT_YELLOW}, {C_BLACK}), "Fire", tileFlagReplaceable | tileFlagPathable, new BasicIcon({','}, C_DARK_GRAY, bg));
        tileAsh = new Tile(new RandomIcon({'.', '\'', ','}, C_DARK_GRAY, bg), "Ash", tileFlagReplaceable | tileFlagPathable, new RandomIcon({'.', '\'', ','}, C_DARK_GRAY, bg));

        tilePillar = new Tile(new RandomIcon({'I'}, C_WHITE, bg), "pillar", tileFlagSolidBoth);
        tileBreakable = new Tile(new BasicIcon('#', C_BLACK, C_WHITE), "breakable wall", tileFlagSolidBoth | tileFlagPathable | tileFlagFlammable, new BasicIcon('#', C_BLACK, C_DARK_GRAY));

        tileEdge = new Tile(new BasicIcon(' ',  C_LIGHT_RED, C_WHITE), "edge", tileFlagSolidBoth | tileFlagIndestructable, new BasicIcon(' ', C_BLACK, C_DARK_GRAY));
        tileUnset = new Tile(new BasicIcon('X',  C_LIGHT_MAGENTA, bg), "unset", tileFlagSolidBoth | tileFlagIndestructable);

        tileDebug1 = new Tile(new BasicIcon('1', C_LIGHT_GREEN, bg), "debug 1", tileFlagPathable | tileFlagReplaceable);
        tileDebug2 = new Tile(new BasicIcon('2', C_LIGHT_GREEN, bg), "debug 2", tileFlagPathable | tileFlagReplaceable);
        tileDebug3 = new Tile(new BasicIcon('3', C_LIGHT_GREEN, bg), "debug 3", tileFlagPathable | tileFlagReplaceable);
        tileDebug4 = new Tile(new BasicIcon('4', C_LIGHT_GREEN, bg), "debug 4", tileFlagPathable | tileFlagReplaceable);
        tileDebug5 = new Tile(new BasicIcon('5', C_LIGHT_GREEN, bg), "debug 5", tileFlagPathable | tileFlagReplaceable);
        tileDebug6 = new Tile(new BasicIcon('6', C_LIGHT_GREEN, bg), "debug 6", tileFlagPathable | tileFlagReplaceable);

        tileTemp = new Tile(new BasicIcon('?', C_LIGHT_RED, bg), "temp", tileFlagPathable | tileFlagReplaceable);
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
