//
//  AiEntity.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "AiEntity.h"


AiEntity::AiEntity(int aiFlags, std::string icon, Geometry::Point2* startPos, int colorCode) : Entity(icon, startPos, colorCode){
    this->ai = aiFlags;

    speed = new Geometry::Point2(0, 0);
}

AiEntity::~AiEntity(){
    delete speed;
}

void AiEntity::runAi(int tick, Level* world){

    if(ai & AiType::aiMoveRandom){
        if(rand()%20==0){
            if(rand()%2==0){
                speed->x = (rand()%3-1) * moveSpeed;
                speed->y = 0;
            }else{
                speed->x = 0;
                speed->y = (rand()%3-1) * moveSpeed;
            }
        }
    }

}

bool AiEntity::update(int tick, Level* level){

    runAi(tick, level);

    tryToMove(
              speed->x==0?0:(speed->x>0?((tick%speed->x)==0?1:0):((tick%-speed->x)==0?-1:0)),
              speed->y==0?0:(speed->y>0?((tick%speed->y)==0?1:0):((tick%-speed->y)==0?-1:0)),
              level
              );

    return Entity::update(tick, level);
}
