//
//  Alive.h
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Alive__
#define __Underneath__Alive__

#include "Entity.h"

class Alive : public Entity{

public:

    static Alive* clone(Alive* oldE, Alive* newE);

    Alive();

    Alive(std::string name, char icon, Point2 startPos, Ui::color colorCode = Ui::COLOR_DEFAULT_ENTITY);

    virtual bool update(int tick, Level* level);

    int getHp(){
        return hp;
    }

    int getMaxHp(){
        return maxHp;
    }

    void hurt(int amount){
        hp -= amount;
    }

    void heal(int amount){
        hp += amount;
    }

    int getViewDistance() {
        return viewDistance;
    }

    std::string getHpBar(unsigned int size, char filled = '=', char empty = '-'){
        std::string s = "";
        int l = (int)(((double)size/maxHp)*hp);
        for(int i=0;i<size;i++){
            s+=i<l?filled:empty;
        }
        return s;
    }

    virtual void save(std::string* data);

    virtual int getEntityTypeId();

    virtual void load(char* data, int* position);

protected:
    int maxHp = 30;
    int hp = maxHp;
    int viewDistance = 8;

};

#endif /* defined(__Underneath__Alive__) */
