//
//  Alive.h
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Alive__
#define __Underneath__Alive__

#include "Entity.h"
#include "Weapon.h"
#include "Spell.h"
#include "Math.h"


typedef int EffectId;

const EffectId effFire = 0;
const EffectId effBleed = 1;
const EffectId effRegen = 2;

struct Effect{

    Effect(EffectId eId, double timeEnd, int power) : Effect(eId, timeEnd, power, 0){

    }

    Effect(EffectId eId, double timeEnd, int power, double lastTime){
        this->eId = eId;
        this->timeEnd = timeEnd;
        this->power = power;
        this->lastTime = lastTime;
    }

    string toString(){
        return "id:"+to_string(eId)+", timeEnd:"+to_string(timeEnd)+", power:"+to_string(power);
    }

    EffectId eId = effFire;
    double timeEnd;
    double lastTime = 0;
    int power = 1;
};



class Alive : public Entity{

public:

    static Alive* clone(Alive* oldE, Alive* newE);

    Alive();

    Alive(string name, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    ~Alive();

    virtual bool update(double time, Level* level);

    virtual string getName(){
        return name;
    }

    int getHp(){
        return hp;
    }

    int getMaxHp(){
        return maxHp;
    }

    int getMp(){
        return mp;
    }

    int getMaxMp(){
        return maxMp;
    }

    virtual void die(){
        hp = 0;
        dead = true;
        //pos.set(-1);
    }

    virtual int hurt(int amount){
        hp -= amount;
        if(hp<=0){
            die();
        }
        //debug(name+" hp: "+to_string(hp));
        return amount;
    }

    virtual int hurt(Spell* w, double time){
        int d = Math::max((rand()%(w->baseDamage)) + (rand()%(w->baseDamage)), 1);
        return hurt(d);
    }

    virtual int hurt(Weapon* w, double time){
        int d = Math::max((rand()%(w->baseDamage)) + (rand()%(w->baseDamage)), 1);
        for(Enchantment ench : w->enchantments){
            switch(ench.eId){
                case enchBleed:
                    if(rand()%ench.chance == 0){
                        addEffect(Effect(effBleed, time+(ench.power*10), ench.power));
                    }
                    break;

                case enchFire:
                    if(rand()%ench.chance == 0){
                        addEffect(Effect(effFire, time+(ench.power*10), ench.power));
                    }
                    break;
            }
        }
        return hurt(d);
    }

    void addEffect(Effect e);

    virtual int heal(int amount){
        if(dead){
            return 0;
        }
        hp += amount;
        if(hp>maxHp){
            int a = amount-(hp-maxHp);
            hp = maxHp;
            return a;
        }
        return amount;
    }

    virtual int healMana(int amount){
        if(dead){
            return 0;
        }
        mp += amount;
        if(mp>maxMp){
            int a = amount-(hp-maxHp);
            mp = maxMp;
            return a;
        }
        return amount;
    }

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);
    
    vector<Item*> inventory;

    void setActiveWeapon(Weapon* newWeapon){

        if(newWeapon == nullptr){
            activeWeapon = nullptr;
            return;
        }
        
        for(Item* ie : inventory){
            if(ie == newWeapon){
                activeWeapon = dynamic_cast<Weapon*>(ie);
                return;
            }
        }
        inventory.push_back(newWeapon);
        activeWeapon = newWeapon;
    }

    Weapon* getActiveWeapon(){
        return activeWeapon;
    }

    bool removeItem(Item* item, bool deleteItem){
        forVector(inventory, i){
            Item* ie = inventory[i];
            if(ie == item){
                if(activeWeapon == item){
                    activeWeapon = nullptr;
                }
                inventory.erase(inventory.begin()+(long)i);
                if(deleteItem){
                    delete item;
                }
                return true;
            }
        }
        return false;
    }

    vector<Effect> effects;

    int viewDistance = 8;

protected:
    string name;
    int maxHp = 30;
    int hp = maxHp;
    int maxMp = 10;
    int mp = maxMp;
    bool dead = false;

    double lastHealTime = 0;
    double healDelay = 10;

    double lastManaTime = 0;
    double manaDelay = 10;

    Weapon* activeWeapon;

};

#endif /* defined(__Underneath__Alive__) */
