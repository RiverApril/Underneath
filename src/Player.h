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
#include "Weapon.h"

class Player : public Alive {

public:

    static Player* clone(Player* oldE, Player* newE);

    Player();

    Player(string name, char icon, Point2 startPos, Ui::color colorCode);

    ~Player();

    bool update(int tick, Level* world);

    bool moveRelative(Point2 p, Level* level);

    bool moveAbsalute(Point2, Level* level);

    bool interact(Level* level, Point2 posToInteract, bool needToBeSolid);

    bool interactWithTile(Level* level, int tid, Point2 posOfTile);

    bool interactWithEntity(Level* level, Entity* e, Point2 posOfEntity);


    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return 1;
    }

    virtual void save(vector<unsigned char>* data);

    virtual void load(unsigned char* data, int* position);

    
    
protected:

};

#endif /* defined(__Underneath__Player__) */
