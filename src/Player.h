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

    static Player* clone(Player* oldE, Player* newE);

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

    virtual int hurt(int amount){
        return Alive::hurt(amount);
    }

    virtual int hurt(Weapon* w, double time){
        return Alive::hurt(w, time);
    }

    virtual int hurt(Spell* s, double time){
        return Alive::hurt(s, time);
    }

    virtual int heal(int amount){
        int a = Alive::heal(amount);
        gainXp(iCON, a/10.0);
        return a;
    }

    virtual int healMana(int amount){
        int a = Alive::heal(amount);
        gainXp(iWIS, a/10.0);
        return a;
    }


    void setDelays(){
        moveDelay = 1.0-((double)(*levels.SPD) / *maxLevels.SPD);
        healDelay = 20.0-((double)(*levels.CON) / (*maxLevels.CON/20.0));
        interactDelay = .1;
    }

    Abilities<int> levels;
    Abilities<double> xp;
    Abilities<double> nextLevelXp;
	
    Abilities<int> maxLevels = Abilities<int>(100, 100, 100, 100, 100, 100, 100);
    
    
protected:

    double moveDelay = 1;
    double interactDelay = .1;
    double waitDelay = 5;

    double useDelay(Item* item);

    void setNextLevelXp(int i){
        nextLevelXp.list[i] = (pow(levels.list[i], 1.2)*2)+10;
    }

    void gainXp(int i, double amount){
        xp.list[i] += amount;
        int l = nextLevelXp.list[i];
        if(xp.list[i] > l && levels.list[i] < maxLevels.list[i]-1){
            xp.list[i] -= l;
            levels.list[i] += 1;
            setNextLevelXp(i);
        }
    }

};

#endif /* defined(__Underneath__Player__) */
