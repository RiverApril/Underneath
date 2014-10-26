//
//  AiEntity.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__AiEntity__
#define __Underneath__AiEntity__

class Level;

#include "Level.h"
#include "Ui.h"
#include "Geometry.h"

namespace AiType {
const int aiNone = 0;
const int aiMoveRandom = 1 << 0;
};

class AiEntity : public Entity {

public:

    AiEntity(int aiFlags, std::string icon, char iconAlt, Geometry::Point2* startPos, int colorCode = Ui::COLOR_DEFAULT_ENTITY);

    ~AiEntity();

    void runAi(int tick, Level* level);
    bool update(int tick, Level* level);


protected:
    int ai = AiType::aiNone;
    int moveSpeed = 4;
    Geometry::Point2* speed;
};

#endif /* defined(__Underneath__AiEntity__) */
