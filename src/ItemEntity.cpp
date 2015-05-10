//
//  ItemEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "ItemEntity.hpp"
#include "Level.hpp"

ItemEntity::ItemEntity() : ItemEntity(nullptr, Point2Zero) {

}

ItemEntity::ItemEntity(Item* item, Point2 p) : Entity(' ', p, Ui::C_WHITE) {
    this->item = item;
    defaultIcon = '*';
}

ItemEntity::~ItemEntity() {

}

char ItemEntity::getIcon(Point2 p, double time, Level* level) {
    if (removed) {
        return defaultIcon;
    }
    if (!item) {
        return defaultIcon;
    }
    switch (item->getItemTypeId()) {
        case ITEM_TYPE_ITEM:
            return '*';
        case ITEM_TYPE_WEAPON:
            return '(';
        case ITEM_TYPE_RANGED:
            return ')';
        case ITEM_TYPE_COMBAT_SPELL:
            return '~';
        case ITEM_TYPE_UTILITY_SPELL:
            return '~';

        default:
            return '*';
    }
}

bool ItemEntity::update(double deltaTime, double time, Level* level) {
    return Entity::update(deltaTime, time, level);
}

ItemEntity* ItemEntity::cloneUnsafe(ItemEntity* oldE, ItemEntity* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->item = Item::clone(oldE->item);

    return newE;

}

int ItemEntity::getEntityTypeId() {
    return ENTITY_TYPE_ITEMENTITY;
}

void ItemEntity::save(vector<unsigned char>* data) {
    Entity::save(data);
    item->save(data);
}

void ItemEntity::load(unsigned char* data, int* position) {
    Entity::load(data, position);
    item = Item::loadNew(data, position);
}
