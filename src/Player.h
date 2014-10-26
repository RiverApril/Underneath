//
//  Player.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Player__
#define __Underneath__Player__

#include "Entity.h"

class Player : public Entity {

public:

    Player(std::string name, std::string icon, char iconAlt, const Geometry::Point2* startPos, Ui::color colorCode);

    ~Player();

    bool update(int tick, Level* world);

    void move(int x, int y, Level* level);

private:
    std::string name;

};

#endif /* defined(__Underneath__Player__) */
