//
//  MaterialItem.h
//  Underneath
//
//  Created by Braeden Atlee on 2/24/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MaterialItem__
#define __Underneath__MaterialItem__

#include "Global.h"
#include "Item.h"

struct Material;

extern vector<Material*> materialList;

struct Material{

    Material(int index, double multiplier, string name){
        this->index = index;
        this->multiplier = multiplier;
        this->name = name;
        materialList[index] = this;
    }

    int getIndex(){
        return index;
    }

    string getName(){
        return name;
    }

    double getMultipler(){
        return multiplier;
    }

private:
    string name = "";
    int index = 0;
    double multiplier = 1;

};

extern Material* materialNone;
extern Material* materialLeather;
extern Material* materialBone;
extern Material* materialTin;
extern Material* materialCopper;
extern Material* materialBronze;
extern Material* materialIron;
extern Material* materialSteel;
extern Material* materialObsidian;
extern Material* materialMythril;
extern Material* materialOrichalcum;
extern Material* materialAdamant;
extern Material* materialEtherial;


class MaterialItem : public Item {
public:

    static void initMaterials(){
        materialList.resize(13);

        int a = 0;

        materialNone =       new Material(a++,  1, ""); // For stuff like Spells
        materialLeather =    new Material(a++,  1, "Leather"); //Armor Only
        materialWood =       new Material(a++,  1, "Wood"); //Bows Only
        materialBone =       new Material(a++,  1, "Bone"); //Blades Only
        materialTin =        new Material(a++,  0, "Tin"); //Crafting Only
        materialCopper =     new Material(a++,  2, "Copper");
        materialBronze =     new Material(a++,  3, "Bronze");
        materialIron =       new Material(a++,  4, "Iron");
        materialSteel =      new Material(a++,  5, "Steel");
        materialObsidian =   new Material(a++,  6, "Obsidian"); //Blades Only
        materialMythril =    new Material(a++,  7, "Mythril");
        materialOrichalcum = new Material(a++,  8, "Orichalsum");
        materialAdamant =    new Material(a++,  9, "Adamant");
        materialEtherial =   new Material(a++, 10, "Etherial"); //Magicly Made
    }

    static void cleanupMaterials(){
        for(Material* m : materialList){
            delete m;
        }
    }


    static MaterialItem* cloneUnsafe(MaterialItem* oldE, MaterialItem* newE = nullptr);

    MaterialItem() : MaterialItem(materialNone, "UNDEFINED", 0){

    }

    MaterialItem(Material* material, string name, Weight weight) : Item(name, weight){
        this->material = material;
    }

    virtual int getItemTypeId(){
        return ITEM_TYPE_MATERIAL_ITEM;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual bool equalsExceptQty(Item* other){
        MaterialItem* otherW = dynamic_cast<MaterialItem*>(other);
        return Item::equalsExceptQty(other)
        &&(otherW)
        &&(material == otherW->material);
    }

    Material* material;
    string overrideName = "";

    virtual string getExtendedName(){
        return overrideName.size()>0?overrideName:((material > 0?material->getName():"")+" "+name);
    }
    
protected:
};

#endif /* defined(__Underneath__MaterialItem__) */
