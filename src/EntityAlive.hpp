//
//  EntityAlive.h
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityAlive__
#define __Underneath__EntityAlive__

#include "Entity.hpp"
#include "ItemWeapon.hpp"
#include "ItemCombatSpell.hpp"
#include "Math.hpp"
#include "Inventory.hpp"
#include "Effect.hpp"
#include "Weakness.hpp"

class EntityAlive : public Entity, public Inventory {
public:

    static EntityAlive* cloneUnsafe(EntityAlive* oldE, EntityAlive* newE);

    EntityAlive();

    EntityAlive(string name, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    ~EntityAlive();

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

    virtual bool isSolid() {
        return !dead;
    }

    virtual double hurt(DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(ItemWeapon* w, double damageMultiplier = 1);

    void addEffect(Effect e);

    bool hasEffect(EffectId eid);

    virtual double heal(double amount);

    virtual double healMana(double amount);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);

    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon){
        console("ERROR - SET ACTIVE WEAPON FOR ALIVE");
    }

    virtual ItemWeapon* getActiveItemWeapon(){
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

    double getDefenseMultiplierFromEffects(DamageType damType){
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
    double maxMp = 0;
    double mp = maxMp;
    bool dead = false;

    double lastHealTime = 0;
    double healDelay = 20;

    double lastManaTime = 0;
    double manaDelay = 20;

};

#endif /* defined(__Underneath__EntityAlive__) */
