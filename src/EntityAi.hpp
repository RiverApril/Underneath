//
//  EntityAi.hpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityAi__
#define __Underneath__EntityAi__

#include "EntityAlive.hpp"
#include "ItemGenerator.hpp"

enum AiType{
    aiNone = 0,
    aiMoveRandom = 1 << 0,
    aiAttack = 1 << 1,
    aiFlee = 1 << 2,
    aiAttackAndFleeAtLowHealth = 1 << 3,
    aiTeleport = 1 << 4,
};

inline AiType operator| (AiType a, AiType b){
    return static_cast<AiType>(static_cast<int>(a) | static_cast<int>(b));
}

inline AiType operator& (AiType a, AiType b){
    return static_cast<AiType>(static_cast<int>(a) & static_cast<int>(b));
}

const double healthPercentLowerBoundry = .25;
const double healthPercentUpperBoundry = .75;

const int agroViewDistanceMultiplier = 3;

class EntityAi : public EntityAlive {
public:

    static EntityAi* cloneUnsafe(EntityAi* oldE, EntityAi* newE);

    EntityAi();

    EntityAi(string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 1);

    ~EntityAi();

    void lookAi(double time, Level* level);
    void moveAi(double time, Level* level);
    void attackAi(double time, Level* level);
    bool update(double deltaTime, double time, Level* level);

    virtual void dropLoots(Level* level);

    virtual double hurt(Level* level, DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(Level* level, ItemWeapon* w, double damageMultiplier = 1);

    virtual void save(vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(vector<unsigned char>* data, int* position);

    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon);

    virtual ItemWeapon* getActiveItemWeapon() {
        return activeItemWeapon;
    }

    virtual bool removeItem(Item* item, bool deleteItem);

    bool equipItem(ItemEquipable* newItem);

    bool equipItem(ItemEquipable* newItem, EquipSlot slot);

    virtual void setTimes(double time) {
        lastMoveTime = time;
        lastAttackTime = time;
        EntityAlive::setTimes(time);
    }

    virtual bool isHostile() {
        return ai & aiAttack;
    }

    double getAttackMultiplierFromEffectsAndEquips(DamageType damType){
        double d = 1;
        for(Effect eff : effects){
            if(eff.eId == effMultRecivedDamage){
                if((int)eff.meta == damType || eff.meta == damNone){
                    d *= eff.power;
                }
            }
        }
        if(activeItemWeapon){
            for(Enchantment e : activeItemWeapon->enchantments){
                if(e.style == eStyle_SelfToEnemy_SelfEff && e.effect.eId == effMultRecivedDamage){
                    if((int)e.effect.meta == damType || e.effect.meta == damNone){
                        d *= e.effect.power;
                    }
                }
            }
        }
        return d;
    }


    double attackMultiplier = 1;

    int lootProfileIndex = -1;

    int maxLootDrop = 1;
    int minLootDrop = 0;

protected:
    int ai = aiNone;

    Point2 lastKnownTargetPos = Point2Neg1;
    EntityAlive* target = nullptr;
    bool canSeeTarget;

    double lastMoveTime = 0;

    double lastAttackTime = 0;

    bool agro = false;

    ItemWeapon* activeItemWeapon = nullptr;
};

#endif /* defined(__Underneath__EntityAi__) */
