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

    static shared_ptr<AiEntity> clone(shared_ptr<AiEntity> oldE, shared_ptr<AiEntity> newE);

    AiEntity();

    AiEntity(std::string name, int aiFlags, char icon, Point2 startPos, int colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 30);

    ~AiEntity();

    void runAi(int tick, shared_ptr<Level> level);
    bool update(int tick, shared_ptr<Level> level);


    virtual void save(std::string* data);

    virtual int getEntityTypeId();

    virtual void load(char* data, int* position);


protected:
    int ai = aiNone;

    Point2 lastKnownPlayerPos;
};

#endif /* defined(__Underneath__AiEntity__) */
