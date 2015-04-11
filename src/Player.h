//
//  Player.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Player__
#define __Underneath__Player__

#include "Alive.h"
#include "Weapon.h"
#include "CombatSpell.h"
#include "Abilities.h"

class Player : public Alive {
public:

    static Player* cloneUnsafe(Player* oldE, Player* newE);

    Player();

    Player(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities);

    ~Player();

    bool update(double deltaTime, double time, Level* world);

    double moveRelative(Point2 p, Level* level);

    double moveAbsalute(Point2, Level* level);

    double interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item);

    double interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item);

    double interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item);

    double calcDamageMultiplier(Weapon* weapon);

    virtual int getEntityTypeId();

    virtual int getRenderDepth() {
        return 1;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual double hurt(DamageType damageType, double amount, double damageMultiplier = 1) {
        timeSinceCombat = 0;
        if(godMode){
            return 0;
        }
        return Alive::hurt(damageType, amount, damageMultiplier);
    }

    virtual double hurt(Weapon* w, double damageMultiplier = 1) {
        timeSinceCombat = 0;
        if(godMode){
            return 0;
        }
        double chance = (((double) abilities[iAGI] / maxAbilities[iAGI]) / 2);
        if (rand() < (RAND_MAX * chance)) {
            return 0;
        }
        return Alive::hurt(w, damageMultiplier);
    }

    virtual double heal(double amount) {
        double a = Alive::heal(amount);
        return a;
    }

    virtual double healMana(double amount) {
        double a = Alive::healMana(amount);
        return a;
    }

    virtual void setActiveWeapon(Weapon* newWeapon);

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
        maxMp = 100 + (((double) (abilities[iWIS]) / maxAbilities[iWIS]) * 500);
    }

    Abilities<int> abilities;
    int abilityPoints = 0;
    int level = 0;
    double xp = 0;
    double nextLevelXp = 0;

    Abilities<int> maxAbilities = Abilities<int>(100, 100, 100, 100, 100, 100, 100);

    void gainXp(double amount);

    /*bool pickupItem(Item* newItem){
        if(newItem != nullptr){
            for(Item* i : inventory){
                if(i->equalsExceptQty(newItem)){
                    //debug("i: "+i->name+"   newItem: "+newItem->name);
                    //debug("i qty: "+to_string(i->qty));
                    //debug("newItem qty: "+to_string(newItem->qty));
                    i->qty += newItem->qty;
                    //debug("i qty: "+to_string(i->qty));
                    delete newItem;
                    //debug("Picked up item and added to stack.");
                    return true;
                }
            }
            inventory.push_back(newItem);
            //debug("Picked up item.");
            return true;
        }
        return false;
    }*/


protected:

    double moveDelay = 1;
    double interactDelay = .1;
    int timeSinceCombat = 0;
    bool outOfCombatHealing = false;

    Item* hotbar[10] = {nullptr};

    double useDelay(Item* item);

    void setNextLevelXp() {
        nextLevelXp = (pow(level, 1.2)*2) + 20;
    }

};

#endif /* defined(__Underneath__Player__) */
