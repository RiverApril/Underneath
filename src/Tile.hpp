//
//  Tile.hpp
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

enum TileFlag{
    tileFlagNone = 0,
    tileFlagSolidOnGround = 1 << 0,
    tileFlagSolidInAir = 1 << 1,
    tileFlagSolidBoth = tileFlagSolidOnGround | tileFlagSolidInAir,
    tileFlagDoor = 1 << 2,
    tileFlagPathable = 1 << 3,
    tileFlagSecretPathable = 1 << 4,
    tileFlagHasTileEntity = 1 << 5,
    tileFlagIndestructable = 1 << 6,
    tileFlagReplaceable = 1 << 7,
    tileFlagFlammable = 1 << 8,
    tileFlagIsTile = 1 << 9,
    tileFlagMonsterSpawningDoor = 1 << 10,

    tileFlagAll = tileFlagSolidBoth | tileFlagDoor | tileFlagPathable | tileFlagSecretPathable | tileFlagHasTileEntity | tileFlagIndestructable | tileFlagReplaceable | tileFlagFlammable | tileFlagIsTile | tileFlagMonsterSpawningDoor,
};

inline TileFlag operator| (TileFlag a, TileFlag b){
    return static_cast<TileFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline TileFlag operator& (TileFlag a, TileFlag b){
    return static_cast<TileFlag>(static_cast<int>(a) & static_cast<int>(b));
}



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

    bool hasAllOfFlags(TileFlag flag) {
        return (flags & flag) == flag;
    }

    bool hasAnyOfFlags(TileFlag flag) {
        return flags & flag;
    }

    bool doesNotHaveAnyOfFlags(TileFlag flag) {
        return !hasAnyOfFlags(flag);
    }

    bool doesNotHaveAllOfFlags(TileFlag flag) {
        return !hasAllOfFlags(flag);
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
    extern Tile* tileHall;
    extern Tile* tileBloodFloor;

    extern Tile* tileWall;
    extern Tile* tilePonyWall;
    extern Tile* tileBookcase;

    extern Tile* tileDoor;
    extern Tile* tileSecretDoor;
    extern Tile* tileOpenDoor;
    extern Tile* tileLockedDoor;
    extern Tile* tileThreateningDoor;
    
    extern Tile* tileSpikes;
    extern Tile* tileBloodSpikes;
    
    extern Tile* tileAlter;
    extern Tile* tileBrokenAlter;

    extern Tile* tileStairUp;
    extern Tile* tileStairDown;

    extern Tile* tileChest;
    extern Tile* tileCrate;

    extern Tile* tileRubble;
    extern Tile* tileCorpse;
    extern Tile* tileBones;

    extern Tile* tileTree;
    extern Tile* tileGrass;
    extern Tile* tilePath;

    extern Tile* tileFire;
    extern Tile* tileAsh;

    extern Tile* tilePillar;
    extern Tile* tileBreakable;

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
