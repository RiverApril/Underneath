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
const AiType aiFollowPlayerDumb = 1 << 1;
const AiType aiAttackPlayer = 1 << 2;
const AiType aiFleeFromPlayerDumb = 1 << 3;
const AiType aiFollowPlayerSmart = 1 << 4;



class AiEntity : public Alive {

public:

    static AiEntity* clone(AiEntity* oldE, AiEntity* newE);

    AiEntity();

    AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    ~AiEntity();

    void runAi(double time, Level* level);
    bool update(double time, Level* level);


    virtual void save(std::vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);


protected:
    int ai = aiNone;

    Point2 lastKnownPlayerPos;

    double lastMoveTime = 0;
    double moveDelay = 1; //should varry
};

#endif /* defined(__Underneath__AiEntity__) */
