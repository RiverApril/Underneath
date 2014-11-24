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

    bool update(int tick, shared_ptr<Level> world);

    bool moveRelative(Point2 p, shared_ptr<Level> level);

    bool moveAbsalute(Point2, shared_ptr<Level> level);

    bool interact(shared_ptr<Level> level, Point2 posToInteract, bool needToBeSolid);

    bool interactWithTile(shared_ptr<Level> level, int tid, Point2 posOfTile);

    bool interactWithEntity(shared_ptr<Level> level, Entity* e, Point2 posOfEntity);


    virtual int getEntityTypeId();

    virtual int getRenderDepth(){
        return 1;
    }

    virtual void save(string* data);

    virtual void load(char* data, int* position);

    
    
protected:

};

#endif /* defined(__Underneath__Player__) */
