//
//  ItemEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "ItemEntity.h"

#include "Level.h"


ItemEntity::ItemEntity() : ItemEntity(nullptr, Point2Zero){

}

ItemEntity::ItemEntity(Item* item, Point2 p) : Entity(' ', p, Ui::C_WHITE){
    this->item = item;
    switch (item->getItemTypeId()) {
        case ITEM_TYPE_ITEM:
            defaultIcon = '*';
            break;
        case ITEM_TYPE_WEAPON:
            defaultIcon = '\\';
            break;
        case ITEM_TYPE_RANGED:
            defaultIcon = ')';
            break;
        case ITEM_TYPE_SPELL:
            defaultIcon = '~';
            break;

        default:
            defaultIcon = '*';
            break;
    }
}

ItemEntity::~ItemEntity(){

}


bool ItemEntity::update(double time, Level* level) {
    return Entity::update(time, level);
}


ItemEntity* ItemEntity::cloneUnsafe(ItemEntity* oldE, ItemEntity* newE){

    Entity::cloneUnsafe(oldE, newE);

    newE->item = oldE->item->clone(oldE->item);

    return newE;

}

int ItemEntity::getEntityTypeId(){
    return ENTITY_TYPE_ITEMENTITY;
}

void ItemEntity::save(vector<unsigned char>* data){
    Entity::save(data);
    item->save(data);
}

void ItemEntity::load(unsigned char* data, int* position){
    Entity::load(data, position);
    item = Item::loadNew(data, position);
}


