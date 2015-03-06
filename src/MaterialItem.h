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

    Material(double multiplier, string name, int uses){
        this->multiplier = multiplier;
        this->name = name;
        this->uses = uses;

		this->index = (int)materialList.size();
        materialList.push_back(this);
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
        materialNone =       new Material(1, "", useNone); // For stuff like Spells
        materialLeather =    new Material(1, "Leather", useArmor); //Armor Only
        materialWood =       new Material(1, "Wood", useBows); //Bows Only
        materialBone =       new Material(1, "Bone", useBlades); //Blades Only
        materialTin =        new Material(0, "Tin", useNone); //Crafting Only
        materialCopper =     new Material(2, "Copper", useArmorAndWeapons);
        materialBronze =     new Material(3, "Bronze", useArmorAndWeapons);
        materialIron =       new Material(4, "Iron", useArmorAndWeapons);
        materialSteel =      new Material(5, "Steel",useArmorAndWeapons);
        materialObsidian =   new Material(5.5, "Obsidian", useBlades); //Blades Only
        materialMythril =    new Material(6, "Mythril", useArmorAndWeapons);
        materialOrichalcum = new Material(7, "Orichalsum", useArmorAndWeapons);
        materialAdamant =    new Material(8, "Adamant", useArmorAndWeapons);
        materialEtherial =   new Material(9, "Etherial", useArmorAndWeapons);
    }

    static void cleanupMaterials(){
        materialList.erase(materialList.begin(), materialList.end());
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

    Material* material = nullptr;
    string overrideName = "";

    virtual string getExtendedName(){
        if(material == nullptr){
            return overrideName.size()>0?overrideName:name;
        }
        return overrideName.size()>0?overrideName:((material->getName().size()>0?(material->getName()+" "+name):name));
    }
    
protected:
};

#endif /* defined(__Underneath__MaterialItem__) */
