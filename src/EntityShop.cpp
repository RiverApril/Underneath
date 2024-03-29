//
//  EntityShop.cpp
//  Underneath
//
//  Created by Emily Atlee on 7/21/15.
//  Copyright (c) 2015 Emily Atlee. All rights reserved.
//

#include "EntityShop.hpp"
#include "ItemGenerator.hpp"


EntityShop::EntityShop() : EntityShop("", aiNone, ' ', Point2Zero, C_WHITE, 1) {

}

EntityShop::EntityShop(string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : EntityAi(name, aiFlags, icon, startPos, colorCode, maxHp) {
}

EntityShop::~EntityShop() {

}

bool EntityShop::update(double deltaTime, double time, Level* level) {



    return EntityAi::update(deltaTime, time, level);
}

void EntityShop::dropLoots(Level* level){

}

int EntityShop::getEntityTypeId() {
    return ENTITY_TYPE_SHOP;
}

void EntityShop::save(vector<unsigned char>* data) {
    EntityAi::save(data);


    Utility::saveInt(data, (int) shopInv.inventory.size());
    forVector(shopInv.inventory, i) {
        Item* ie = shopInv.inventory[i];
        ie->save(data);
    }

}

void EntityShop::load(vector<unsigned char>* data, int* position) {
    EntityAi::load(data, position);

    int size = Utility::loadInt(data, position);

    repeat(size, i) {
        Item* item = Item::loadNew(data, position);
        debug("Loaded item: " + item->getName(false) + "(" + to_string(item->getItemTypeId()) + ")");
        shopInv.inventory.push_back(item);
    }

}


int EntityShop::getCoinValue(Item* item){
    int value = ItemGenerator::calculateItemValue(item);
    if(value < 0){
        return value;
    }
    return value * 1.5;
}
