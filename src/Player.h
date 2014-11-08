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

class Player : public Alive{

public:

    static Player* clone(Player* oldE, Player* newE);

    Player();

    Player(std::string name, char icon, Point2 startPos, Ui::color colorCode);

    ~Player();

    bool update(int tick, Level* world);

    bool moveRelative(Point2 p, Level* level);

    bool moveExact(Point2, Level* level);

    bool use(Level* level);


    virtual void save(std::string* data);

    virtual int getEntityTypeId();

    virtual void load(char* data, int* position);
    
    
protected:

};

#endif /* defined(__Underneath__Player__) */
