//
//  TileEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "TileEntity.hpp"
#include "Utility.hpp"
#include "TEStair.hpp"
#include "TEChest.hpp"
#include "TEAlter.hpp"

void TileEntity::save(vector<unsigned char>* data) {
    Utility::saveInt(data, getTileEntityTypeId());
    pos.save(data);
}

void TileEntity::load(vector<unsigned char>* data, int* position) {
    pos = Point2(data, position);
}

TileEntity* TileEntity::clone(TileEntity* oldI) {
    vector<unsigned char>* data = new vector<unsigned char>();
    oldI->save(data);
    int* position = new int(0);
    TileEntity* te = loadNew(data, position);
    
    delete data;
    delete position;
    
    return te;
}

TileEntity* TileEntity::loadNew(vector<unsigned char>* data, int* position) {
    TileEntity* e;

    int type = Utility::loadInt(data, position);

    switch (type) {
        case TILE_ENTITY_TYPE_NONE:
            e = new TileEntity();
            break;
        case TILE_ENTITY_TYPE_STAIR:
            e = new TEStair();
            break;
        case TILE_ENTITY_TYPE_CHEST:
            e = new TEChest();
            break;
        case TILE_ENTITY_TYPE_ALTER:
            e = new TEAlter();
            break;

        default:
            throw Utility::FileExceptionLoad("Tile Entity type unknown: " + to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}
