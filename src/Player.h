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
#include "Spell.h"
#include "Abilities.h"

class Player : public Alive {

public:

    static Player* cloneUnsafe(Player* oldE, Player* newE);

    Player();

    Player(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities);

    ~Player();

    bool update(double time, Level* world);

    double moveRelative(Point2 p, Level* level);

    double moveAbsalute(Point2, Level* level);

    double interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item);

    double interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item);

    double interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item);

    double waitUntilHealed();

    double useItem(Item* item);


    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return 1;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    virtual double hurt(double amount){
        return Alive::hurt(amount);
    }

    virtual double hurt(Weapon* w, double time){
        return Alive::hurt(w, time);
    }

    virtual double heal(double amount){
        int a = Alive::heal(amount);
        gainXp(iCON, a/10.0);
        return a;
    }

    virtual double healMana(double amount){
        int a = Alive::heal(amount);
        gainXp(iWIS, a/10.0);
        return a;
    }


    void updateVariablesForAbilities(){
        moveDelay = 1.0-((double)(levels[iSPD]) / maxLevels[iSPD]);
        healDelay = 20.0-((double)(levels[iCON]) / (maxLevels[iCON]/20.0));
        interactDelay = .1;

        maxHp = 30 + (((double)(levels[iCON]) / maxLevels[iCON]) * 1000);
        maxMp = 30 + (((double)(levels[iWIS]) / maxLevels[iWIS]) * 1000);
    }

    Abilities<int> levels;
    Abilities<double> xp;
    Abilities<double> nextLevelXp;
	
    Abilities<int> maxLevels = Abilities<int>(100, 100, 100, 100, 100, 100, 100);

    void gainXp(int i, double amount){
        xp.list[i] += amount;
        int l = nextLevelXp.list[i];
        while(xp.list[i] > l && levels.list[i] < maxLevels.list[i]-1){
            xp.list[i] -= l;
            levels.list[i] += 1;
            setNextLevelXp(i);
            updateVariablesForAbilities();
        }
    }

    bool pickupItem(Item* newItem){
        if(newItem != nullptr){
            for(Item* i : inventory){
                if(i->equalsExceptQty(newItem)){
                    debug("i: "+i->name+"   newItem: "+newItem->name);
                    debug("i qty: "+to_string(i->qty));
                    debug("newItem qty: "+to_string(newItem->qty));
                    i->qty += newItem->qty;
                    debug("i qty: "+to_string(i->qty));
                    delete newItem;
                    debug("Picked up item and added to stack.");
                    return true;
                }
            }
            inventory.push_back(newItem);
            debug("Picked up item.");
            return true;
        }
        return false;
    }
    
    
protected:

    double moveDelay = 1;
    double interactDelay = .1;
    double waitDelay = 5;

    double useDelay(Item* item);

    void setNextLevelXp(int i){
        nextLevelXp.list[i] = (pow(levels.list[i], 1.2)*2)+10;
    }

};

#endif /* defined(__Underneath__Player__) */
