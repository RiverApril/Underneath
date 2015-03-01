//
//  MaterialItem.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/24/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MaterialItem.h"
#include "Utility.h"

int useNone = 0;
int useArmor = 1 << 0;
int useBows = 1 << 1;
int useBlades = 1 << 2;
int useArmorAndWeapons = useArmor | useBows | useBlades;

vector<Material*> materialList;

Material* materialNone;
Material* materialLeather;
Material* materialWood;
Material* materialBone;
Material* materialTin;
Material* materialCopper;
Material* materialBronze;
Material* materialIron;
Material* materialSteel;
Material* materialObsidian;
Material* materialMythril;
Material* materialOrichalcum;
Material* materialAdamant;
Material* materialEtherial;

void MaterialItem::save(vector<unsigned char>* data){
    Item::save(data);

    FileUtility::saveInt(data, material->getIndex());
}

void MaterialItem::load(unsigned char* data, int* position){
    Item::load(data, position);

    material = materialList[FileUtility::loadInt(data, position)];
}

MaterialItem* MaterialItem::cloneUnsafe(MaterialItem* oldE, MaterialItem* newE){

    Item::cloneUnsafe(oldE, newE);

    newE->material = oldE->material;

    return newE;
    
}
