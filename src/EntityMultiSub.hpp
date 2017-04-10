//
//  EntityMultiSub.hpp
//  Underneath
//
//  Created by Braeden Atlee on 7/26/16.
//  Copyright © 2016 Braeden Atlee. All rights reserved.
//

#ifndef EntityMultiSub_hpp
#define EntityMultiSub_hpp

#include "EntityMulti.hpp"

class EntityMultiSub : public EntityAlive{
    
public:
    
    virtual void save(vector<unsigned char>* data);
    
    virtual void load(vector<unsigned char>* data, int* position);
    
    virtual int getEntityTypeId();
    
    EntityMultiSub();
    
    EntityMultiSub(EntityMulti* master, string name, char icon, Ui::Color color, Point2 relPos);
    
    ~EntityMultiSub();
    
    virtual double hurt(Level* level, DamageType damageType, double amount, double damageMultiplier = 1);
    
    virtual double hurt(Level* level, ItemWeapon* w, double damageMultiplier = 1);
    
    bool update(double deltaTime, double time, Level* level);
    
    
    
    
    EntityMulti* master;
    Point2 relPos;
    
};

#endif /* EntityMultiSub_hpp */
