//
//  AiEntity.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__AiEntity__
#define __Underneath__AiEntity__

#include "Alive.h"

typedef int AiType;

const AiType aiNone = 0;
const AiType aiMoveRandom = 1 << 0;
const AiType aiAttackPlayer = 1 << 1;
const AiType aiFleeFromPlayer = 1 << 2;

const int agroViewDistanceMultiplier = 3;



class AiEntity : public Alive {

public:

    static AiEntity* cloneUnsafe(AiEntity* oldE, AiEntity* newE);

    AiEntity();

    AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 1);

    ~AiEntity();

    void runAi(double time, Level* level);
    bool update(double deltaTime, double time, Level* level);

    virtual double hurt(DamageType damageType, double amount, double damageMultiplier = 1);

    virtual double hurt(Weapon* w, double damageMultiplier = 1);

    virtual void save(std::vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);

    void setMoveDelay(double newDelay){
        moveDelay = newDelay;
    }

    virtual void setTimes(double time){
        lastMoveTime = time;
        lastAttackTime = time;
        Alive::setTimes(time);
    }


protected:
    int ai = aiNone;

    Point2 lastKnownTargetPos;
    Alive* target = nullptr;

    double lastMoveTime = 0;
    double moveDelay = 1.5; //TODO should vary

    double lastAttackTime = 0;

    bool agro = false;
};

#endif /* defined(__Underneath__AiEntity__) */
