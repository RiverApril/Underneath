//
//  EntityMoving.hpp
//  Underneath
//
//  Created by Braeden Atlee on 3/10/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef EntityMoving_hpp
#define EntityMoving_hpp

#include "Geometry.hpp"

#include "Entity.hpp"
#include "ItemExplosive.hpp"

#define AIR_FRICTION (.9)
#define GROUND_FRICTION (.7)
#define GRAVITY (.1)

#define FRICTION (location.z!=0 ? AIR_FRICTION : GROUND_FRICTION)

class EntityMoving : public Entity {
public:

    static EntityMoving* cloneUnsafe(EntityMoving* oldE, EntityMoving* newE);

    EntityMoving();

    EntityMoving(Point2 p, double z);

    ~EntityMoving();

    virtual void hit(Level* level){}

    virtual bool update(double deltaTime, double time, Level* level);

    virtual void save(vector<unsigned char>* data);

    virtual void load(vector<unsigned char>* data, int* position);

    virtual int getEntityTypeId();

    virtual char getChar(unsigned long tick, Point2 pos, Level* lvl);

    virtual string getName() {
        return "Moving";
    }

    virtual bool isHostile() {
        return velocity != Vector3Zero;
    }



protected:
    Vector3 location = Vector3Zero;
    Vector3 velocity = Vector3Zero;
    
    
};

#endif /* EntityMoving_hpp */
