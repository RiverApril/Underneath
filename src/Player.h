//
//  Player.h
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Player__
#define __Underneath__Player__

#include "Alive.h"
#include "EMagical.h"

class Player : public Alive, public EMagical {

public:

    static shared_ptr<Player> clone(shared_ptr<Player> oldE, shared_ptr<Player> newE);

    Player();

    Player(string name, char icon, Point2 startPos, Ui::color colorCode);

    ~Player();

    bool update(int tick, shared_ptr<Level> world);

    bool moveRelative(Point2 p, shared_ptr<Level> level);

    bool moveExact(Point2, shared_ptr<Level> level);

    bool use(shared_ptr<Level> level);


    virtual int getEntityTypeId();

    virtual void save(string* data);

    virtual void load(char* data, int* position);
    
    
protected:

};

#endif /* defined(__Underneath__Player__) */
