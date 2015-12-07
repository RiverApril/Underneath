//
//  EntityPlayer.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityPlayer__
#define __Underneath__EntityPlayer__

#include "EntityAlive.hpp"
#include "ItemWeapon.hpp"
#include "ItemCombatSpell.hpp"
#include "Abilities.hpp"
#include "ItemArmor.hpp"

class EntityPlayer : public EntityAlive {
public:

    static EntityPlayer* cloneUnsafe(EntityPlayer* oldE, EntityPlayer* newE);

    EntityPlayer();

    EntityPlayer(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities);

    ~EntityPlayer();

    bool update(double deltaTime, double time, Level* world);

    double moveRelative(Point2 p, Level* level);

    double moveAbsalute(Point2, Level* level, bool canInteract);

    double useItemOnOther(Item* itemToUse, Item* itemToBeUsedOn);
    
    int repairToolPower();

    double interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item);

    double interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item);

    double interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item);

    double calcDamageMultiplier(ItemWeapon* weapon);

    virtual int getEntityTypeId();

    virtual int getRenderDepth() {
        return 1;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual double hurt(DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(ItemWeapon* w, double damageMultiplier = 1);

    virtual double heal(double amount) {
        double a = EntityAlive::heal(amount);
        return a;
    }

    virtual double healMana(double amount) {
        double a = EntityAlive::healMana(amount);
        return a;
    }

    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon);

    virtual ItemWeapon* getActiveItemWeapon() {
        return dynamic_cast<ItemWeapon*>(equipedItems[slotWep1]);
    }

    virtual ItemWeapon* getSecondaryItemWeapon() {
        return dynamic_cast<ItemWeapon*>(equipedItems[slotWep2]);
    }

    EquipSlot getSlot(ItemEquipable* e){
        for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
            if(p.second == e){
                return p.first;
            }
        }
        return slotNone;
    }

    virtual ItemEquipable* getEquiped(EquipSlot slot) {
        if(equipedItems.count(slot) <= 0){
            return nullptr;
        }
        return equipedItems[slot];
    }

    virtual bool removeItem(Item* item, bool deleteItem);

    void updateVariablesForAbilities() {
        moveDelay = 1.0 - ((double) (abilities[iSPD]) / maxAbilities[iSPD]);
        if (outOfCombatHealing) {
            healDelay = .5;
            manaDelay = .5;
        } else {
            healDelay = 20.0 - ((double) (abilities[iCON]) / (maxAbilities[iCON] / 20.0));
            manaDelay = 20.0 - ((double) (abilities[iWIS]) / (maxAbilities[iWIS] / 20.0));
        }
        interactDelay = .1;

        maxHp = 100 + (((double) (abilities[iCON]) / maxAbilities[iCON]) * 500);
        maxMp = 0 + (((double) (abilities[iWIS]) / maxAbilities[iWIS]) * 500);
    }

    Abilities<int> abilities;
    int abilityPoints = 0;
    int level = 0;
    double xp = 0;
    double nextLevelXp = 0;

    Abilities<int> maxAbilities = Abilities<int>(100, 100, 100, 100, 100, 100, 100);

    void gainXp(double amount);

    bool equipItem(ItemEquipable* newItem, bool forceDefaultSlot = false);
    bool equipItem(ItemEquipable* newItem, EquipSlot slot);


    //don't need to save
    bool leveledUp = false;


    int xpForLevel(int l){
        l+=1;
        return (pow(l, 2)+(5*l)+20);
    }

    double getDefenseMultiplierFromArmor(DamageType damType, bool reduceDurability){


        double def = 0;

        for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
            if(p.second){
                ItemArmor* armor = dynamic_cast<ItemArmor*>(p.second);
                if(armor){
                    for(Defense d : armor->defenses){
                        if(d.damageType == damType || d.damageType == -1){
                            if(reduceDurability){
                                if(armor->durability > -1){
                                    if(armor->durability == 0){
                                        consolef("&%cA peice of your armor is broken!", Ui::cc(C_LIGHT_RED));
                                    }else if(armor->durability < 16){
                                        consolef("&%cA peice of your armor is almost broken!", Ui::cc(C_LIGHT_RED));
                                    }
                                    if(armor->durability > 0){
                                        armor->durability--;
                                    }
                                }
                            }
                            if(armor->durability > 0){
                                def += d.amount;
                            }else{
                                def += d.amount / 2;
                            }
                        }
                    }
                }
            }
        }

        return def;
    }

    double getAttackMultiplierFromEffectsAndArmor(DamageType damType){
        double d = 1;
        for(Effect eff : effects){
            if(eff.eId == effBuffAttack){
                if((int)eff.meta == damType){
                    d += eff.power;
                }
            }
        }
        for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
            ItemArmor* a = dynamic_cast<ItemArmor*>(p.second);
            if(a){
                for(Enchantment e : a->enchantments){
                    if(e.effectId == effBuffAttack){
                        if((int)e.meta == damType){
                            d += e.power;
                        }
                    }
                }
            }
        }
        return d;
    }


    double useDelay(Item* item);

    map<EquipSlot, ItemEquipable*> equipedItems;


protected:


    double moveDelay = 1;
    double interactDelay = .1;
    int timeSinceCombat = 0;
    bool outOfCombatHealing = false;

    void setNextLevelXp() {
        nextLevelXp = xpForLevel(level);
    }

};

#endif /* defined(__Underneath__EntityPlayer__) */
