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

extern int useNone;
extern int useArmor;
extern int useBows;
extern int useBlades;
extern int useArmorAndWeapons;

struct Material;

extern vector<Material*> materialList;

struct Material{

    Material(int index, double multiplier, string name, int uses){
        this->index = index;
        this->multiplier = multiplier;
        this->name = name;
        this->uses = uses;
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

    bool hasUse(int use){
        return (uses & use)!=0;
    }

private:

    string name = "";
    int index = 0;
    double multiplier = 1;
    int uses = useNone;

};

extern Material* materialNone;
extern Material* materialLeather;
extern Material* materialWood;
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

        materialNone =       new Material(a++,  1, "", useNone); // For stuff like Spells
        materialLeather =    new Material(a++,  1, "Leather", useArmor); //Armor Only
        materialWood =       new Material(a++,  1, "Wood", useBows); //Bows Only
        materialBone =       new Material(a++,  1, "Bone", useBlades); //Blades Only
        materialTin =        new Material(a++,  0, "Tin", useNone); //Crafting Only
        materialCopper =     new Material(a++,  2, "Copper", useArmorAndWeapons);
        materialBronze =     new Material(a++,  3, "Bronze", useArmorAndWeapons);
        materialIron =       new Material(a++,  4, "Iron", useArmorAndWeapons);
        materialSteel =      new Material(a++,  5, "Steel",useArmorAndWeapons);
        materialObsidian =   new Material(a++,  5.5, "Obsidian", useBlades); //Blades Only
        materialMythril =    new Material(a++,  6, "Mythril", useArmorAndWeapons);
        materialOrichalcum = new Material(a++,  7, "Orichalsum", useArmorAndWeapons);
        materialAdamant =    new Material(a++,  8, "Adamant", useArmorAndWeapons);
        materialEtherial =   new Material(a++,  9, "Etherial", useArmorAndWeapons);
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
        return overrideName.size()>0?overrideName:((material->getName().size()>0?(material->getName()+" "+name):name));
    }
    
protected:
};

#endif /* defined(__Underneath__MaterialItem__) */
