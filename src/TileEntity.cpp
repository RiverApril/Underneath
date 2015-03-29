//
//  TileEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/20/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "TileEntity.h"
#include "Utility.h"
#include "TEStair.h"
#include "TEChest.h"
#include "TEMimic.h"

void TileEntity::save(vector<unsigned char>* data){
    Utility::saveInt(data, getTileEntityTypeId());
    Point2::save(pos, data);
}

void TileEntity::load(unsigned char* data, int* position){
    pos = Point2::load(data, position);
}

TileEntity* TileEntity::cloneUnsafe(TileEntity* oldE, TileEntity* newE){

    newE->pos = oldE->pos;

    return newE;
}

template<class Super, class Sub>
Sub* TileEntity::makeNewAndClone(Super* oldT){
    Sub* newT = new Sub();
    return Sub::cloneUnsafe(dynamic_cast<Sub*>(oldT), newT);
}

TileEntity* TileEntity::clone(TileEntity* oldI){

    int type = oldI->getTileEntityTypeId();

    switch (type) {
        case TILE_ENTITY_TYPE_NONE:
            return makeNewAndClone<TileEntity, TileEntity>(oldI);

        case TILE_ENTITY_TYPE_STAIR:
            return makeNewAndClone<TileEntity, TEStair>(oldI);

        case TILE_ENTITY_TYPE_CHEST:
            return makeNewAndClone<TileEntity, TEChest>(oldI);

        case TILE_ENTITY_TYPE_MIMIC:
            return makeNewAndClone<TileEntity, TEMimic>(oldI);

        default:
            throw Utility::FileExceptionLoad("Tile Entity type unknown: "+to_string(type));
            return nullptr;
            break;
    }



}

TileEntity* TileEntity::loadNew(unsigned char* data, int* position){
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

        default:
            throw Utility::FileExceptionLoad("Tile Entity type unknown: "+to_string(type));
            return nullptr;
            break;
    }
    e->load(data, position);

    return e;
}
