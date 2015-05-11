//
//  Alive.h
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Alive__
#define __Underneath__Alive__

#include "Entity.hpp"
#include "Weapon.hpp"
#include "CombatSpell.hpp"
#include "Math.hpp"
#include "Inventory.hpp"
#include "Effect.hpp"
#include "Weakness.hpp"

class Alive : public Entity, public Inventory {
public:

    static Alive* cloneUnsafe(Alive* oldE, Alive* newE);

    Alive();

    Alive(string name, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    ~Alive();

    virtual bool update(double deltaTime, double time, Level* level);

    virtual string getName() {
        return name;
    }

    double getHp() {
        return hp;
    }

    double getMaxHp() {
        return maxHp;
    }

    double getMp() {
        return mp;
    }

    double getMaxMp() {
        return maxMp;
    }

    virtual void die() {
        hp = 0;
        dead = true;
        //pos.set(-1);
    }

    virtual double hurt(DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(Weapon* w, double damageMultiplier = 1);

    void addEffect(Effect e);

    virtual double heal(double amount);

    virtual double healMana(double amount);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);

    virtual void setActiveWeapon(Weapon* newWeapon){
        console("ERROR - SET ACTIVE WEAPON FOR ALIVE");
    }

    virtual Weapon* getActiveWeapon(){
        return nullptr;
    }

    virtual void setTimes(double time) {
        lastHealTime = time;
        lastManaTime = time;
        Entity::setTimes(time);
    }

    //vector<Item*> inventory;

    vector<Effect> effects;

    vector<Weakness> weaknesses;

    int viewDistance = 10;
    
    double getDefenseFromEffects(DamageType damType){
        double d = 0;
        for(Effect eff : effects){
            if(eff.eId == effBuffDefense){
                if((int)eff.meta == damType){
                    d += eff.power;
                }
            }
        }
        return 1.0 - d;
    }

protected:
    string name;
    double maxHp = 30;
    double hp = maxHp;
    double maxMp = 30;
    double mp = maxMp;
    bool dead = false;

    double lastHealTime = 0;
    double healDelay = 20;

    double lastManaTime = 0;
    double manaDelay = 20;

};

#endif /* defined(__Underneath__Alive__) */
