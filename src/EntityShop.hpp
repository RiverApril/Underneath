//
//  EntityShop.h
//  Underneath
//
//  Created by Braeden Atlee on 7/21/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__EntityShop__
#define __Underneath__EntityShop__

#include "EntityAi.hpp"

class EntityShop : public EntityAi {
public:

    static EntityShop* cloneUnsafe(EntityShop* oldE, EntityShop* newE);

    EntityShop();

    EntityShop(std::string name, int aiFlags, char icon, Point2 startPos, Ui::Color colorCode = Ui::COLOR_DEFAULT_ENTITY, int maxHp = 1);

    ~EntityShop();

    bool update(double deltaTime, double time, Level* level);

    virtual void save(std::vector<unsigned char>* data);

    virtual int getEntityTypeId();

    virtual void load(unsigned char* data, int* position);


protected:

    Inventory shopInv;

};

#endif /* defined(__Underneath__EntityShop__) */
