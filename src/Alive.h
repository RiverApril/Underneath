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


typedef int EffectId;

const EffectId effFire = 0;
const EffectId effBleed = 1;

struct Effect{
    Effect(EffectId eId, int duration, int power){
        this->eId = eId;
        this->duration = duration;
        this->power = power;
    }
    EffectId eId = effFire;
    int duration = 1;
    int power = 1;
};



class Alive : public Entity{

public:

    static shared_ptr<Alive> clone(shared_ptr<Alive> oldE, shared_ptr<Alive> newE);

    Alive();

    Alive(string name, char icon, Point2 startPos, Ui::color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    virtual bool update(int tick, shared_ptr<Level> level);

    int getHp(){
        return hp;
    }

    int getMaxHp(){
        return maxHp;
    }

    virtual void die(){
        hp = 0;
        dead = true;
        pos->set(-1);
    }

    int hurt(int amount){
        hp -= amount;
        if(hp<=0){
            die();
        }
        regenTick = 0;
        debug(name+" hp: "+to_string(hp));
        return amount;
    }

    int hurt(shared_ptr<Weapon> w){
        int d = w->baseDamage;
        int i;
        forVector(w->enchantments, i){
            Enchantment ench = w->enchantments[i];
            switch(ench.eId){
                case enchBleed:
                    if(rand()%ench.chance == 0){
                        addEffect(Effect(effBleed, ench.power*100, ench.power));
                    }
                    break;

                case enchFire:
                    if(rand()%ench.chance == 0){
                        addEffect(Effect(effFire, ench.power*100, ench.power));
                    }
                    break;
            }
        }
        return hurt(d);
    }

    void addEffect(Effect e);

    void heal(int amount){
        if(dead){
            return;
        }
        hp += amount;
        if(hp>maxHp){
            hp = maxHp;
        }
    }

    virtual void save(string* data);

    virtual int getEntityTypeId();

    virtual void load(char* data, int* position);
    
    vector<shared_ptr<Item>> inventory;

    void setActiveWeapon(shared_ptr<Weapon> newWeapon){
        
        for(int i=0;i<inventory.size();i++){
            if(inventory[i] == newWeapon){
                activeWeapon = dynamic_pointer_cast<Weapon>(inventory[i]);
                return;
            }
        }
        inventory.push_back(newWeapon);
        activeWeapon = newWeapon;
    }

    shared_ptr<Weapon> getActiveWeapon(){
        return activeWeapon;
    }

    vector<Effect> effects;

    int viewDistance = 8;

protected:
    int maxHp = 30;
    int hp = maxHp;
    bool dead = false;
    int regenTick = 0;

    shared_ptr<Weapon> activeWeapon;

};

#endif /* defined(__Underneath__Alive__) */
