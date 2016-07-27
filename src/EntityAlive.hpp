//
//  EntityAlive.hpp
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

    virtual double hurt(Level* level, DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(Level* level, ItemWeapon* w, double damageMultiplier = 1);

    void addEffect(Effect e);

    bool hasEffect(EffectId eid);

    virtual double heal(double amount, bool overload);

    virtual double healMana(double amount, bool overload);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(vector<unsigned char>* data, int* position);

    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon){
        console("ERROR - SET ACTIVE WEAPON FOR ALIVE");
    }

    virtual ItemWeapon* getActiveItemWeapon(){
        return nullptr;
    }

    virtual void setTimes(double time) {
        Entity::setTimes(time);
    }

    virtual TileFlag getSolidity(){
        return solidity;
    }

    //vector<Item*> inventory;

    vector<Effect> effects;

    vector<Weakness> weaknesses;

    int viewDistance = 10;

    virtual double getRecivedDamageMultiplierFromEffects(DamageType damType){
        double d = 1;
        for(Effect eff : effects){
            if(eff.eId == effMultRecivedDamage){
                if((int)eff.meta == damType || eff.meta == damNone){
                    d *= eff.power;
                }
            }
        }
        return d;
    }
    
    bool dead = false;

    double moveDelay = 1.5;
    double dodgeChance = 0;
    
    double healBase = 1;
    double healManaBase = 1;

    double healMultiplier = 0.05;
    double healManaMultiplier = 0.05;

    TileFlag solidity = tileFlagSolidOnGround;

    string name;
    double maxHp = 30;
    double hp = maxHp;
    double maxMp = 0;
    double mp = maxMp;

};

#endif /* defined(__Underneath__EntityAlive__) */
