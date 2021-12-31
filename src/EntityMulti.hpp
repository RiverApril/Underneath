//
//  EntityMulti.hpp
//  Underneath
//
//  Created by Emily Atlee on 7/26/16.
//  Copyright © 2016 Emily Atlee. All rights reserved.
//

#ifndef EntityMulti_hpp
#define EntityMulti_hpp

#include "Global.hpp"
#include "EntityAlive.hpp"
#include "Icon.hpp"
#include "ItemExplosive.hpp"


class EntityMultiSub;


enum AiMultiType{
    aiMultiNone = 0,
    aiMultiAttackNormal = 1 << 1,
    aiMultiAttackThrow = 1 << 2
};

class EntityMulti : public EntityAlive{
    
public:
    
    virtual void save(vector<unsigned char>* data);
    
    virtual void load(vector<unsigned char>* data, int* position);
    
    virtual int getEntityTypeId();
    
    EntityMulti();
    
    EntityMulti(string name, int aiFlags, vector<vector<char>> icons, Ui::Color color, Point2 startPos, Point2 relCenter, int maxHp = 1);
    
    ~EntityMulti();
    
    virtual void dropLoots(Level* level);
    
    virtual double hurt(Level* level, DamageType damageType, double amount, double damageMultiplier = 1);
    
    virtual double hurt(Level* level, ItemWeapon* w, double damageMultiplier = 1);
    
    void lookAi(double time, Level* level);
    void attackAi(double time, Level* level);
    
    bool update(double deltaTime, double time, Level* level);
    
    virtual bool isHostile() {
        return (aiMulti & aiMultiAttackNormal) || (aiMulti & aiMultiAttackThrow);
    }
    
    virtual bool tryToMoveAbsolute(Point2 p, Level* world, bool force = false);
    
    virtual void setActiveItemWeapon(ItemWeapon* newItemWeapon);
    
    virtual ItemWeapon* getActiveItemWeapon() {
        return activeItemWeapon;
    }
    
    virtual void setActiveItemExplosive(ItemExplosive* newItemExplosive);
    
    virtual ItemExplosive* getActiveItemExplosive() {
        return activeItemExplosive;
    }
    
    virtual void setTimes(double time) {
        lastMoveTime = time;
        lastAttackTime = time;
        lastThrowTime = time;
        EntityAlive::setTimes(time);
    }
    
    
    //temp
    vector<int> subEntitiesAsIds;

    
    vector<EntityMultiSub*> subEntities;
    bool spawnedSubs = false;
    ItemWeapon* activeItemWeapon;
    ItemExplosive* activeItemExplosive;
    
    int aiMulti = aiMultiNone;
    Point2 lastKnownTargetPos = Point2Neg1;
    EntityAlive* target = nullptr;
    bool canSeeTarget;
    
    double lastMoveTime = 0;
    
    double lastAttackTime = 0;
    
    double lastThrowTime = 0;
    
};

#endif /* EntityMulti_hpp */
