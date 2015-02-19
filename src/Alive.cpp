//
//  Alive.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Alive.h"
#include "Utility.h"
#include "Level.h"
#include "Random.h"


Alive::Alive() : Alive("", ' ', Point2Zero, Ui::C_WHITE){

}

Alive::Alive(string name, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : Entity(icon, startPos, colorCode){

    this->name = name;
    this->maxHp = maxHp;
    this->hp = this->maxHp;
    this->solid = true;

}

Alive::~Alive(){
    for(Item* i : inventory){
        delete i;
    }
}


bool Alive::update(double time, Level* level) {

    if(dead){
        level->removeEntity(this, true);
    }else{
        while(lastHealTime+healDelay<=time){
            heal(1);
            lastHealTime += healDelay;
        }
        while(lastManaTime+manaDelay<=time){
            healMana(1);
            lastManaTime += manaDelay;
        }
		
        forVector(effects, i){
            Effect e = effects[i];

            if(e.lastTime+1 < time){
                switch(e.eId){
                    case effBleed:
                    case effFire:
                        hurt(e.power);
                        break;
                    case effRegen:
                        heal(e.power);
                        break;
                }
                e.lastTime = time;
            }

            if(time >= e.timeEnd){
                effects.erase(effects.begin()+i);
                i--;
                continue;
            }
        }
    }

    return Entity::update(time, level);
}

double Alive::hurt(double amount, double damageMultiplier){
    amount *= damageMultiplier;
    hp -= amount;
    if(hp<=0){
        die();
    }
    //debug(name+" hp: "+to_string(hp));
    return amount;
}

double Alive::hurt(Weapon* w, double time, double damageMultiplier){
    double d = w->baseDamage * Random::randDouble(.5, 1.5);
    for(Enchantment ench : w->enchantments){
        switch(ench.eId){
            case enchBleed:
                if(rand()%ench.chance == 0){
                    addEffect(Effect(effBleed, time+(ench.power*10), ench.power));
                }
                break;

            case enchFire:
                if(rand()%ench.chance == 0){
                    addEffect(Effect(effFire, time+(ench.power*10), ench.power));
                }
                break;
        }
    }
    consolef("Hurt %s %.2f hp.", name.c_str(), d);
    return hurt(d, damageMultiplier);
}

void Alive::addEffect(Effect e){
    forVector(effects, i){
        Effect ei = effects[i];
        if(e.eId == ei.eId){
            ei.timeEnd = e.timeEnd;
            ei.power = e.power;
            return;
        }
    }
    effects.push_back(e);
}

double Alive::heal(double amount){
    if(dead){
        return 0;
    }
    hp += amount;
    if(hp>maxHp){
        double a = amount-(hp-maxHp);
        hp = maxHp;
        return a;
    }
    return amount;
}

double Alive::healMana(double amount){
    if(dead){
        return 0;
    }
    mp += amount;
    if(mp>maxMp){
        double a = amount-(hp-maxHp);
        mp = maxMp;
        return a;
    }
    return amount;
}

void Alive::setActiveWeapon(Weapon* newWeapon){

    if(newWeapon == nullptr){
        activeWeapon = nullptr;
        return;
    }

    for(Item* ie : inventory){
        if(ie == newWeapon){
            activeWeapon = dynamic_cast<Weapon*>(ie);
            return;
        }
    }
    inventory.push_back(newWeapon);
    activeWeapon = newWeapon;
}

bool Alive::removeItem(Item* item, bool deleteItem){
    forVector(inventory, i){
        Item* ie = inventory[i];
        if(ie == item){
            if(activeWeapon == item){
                activeWeapon = nullptr;
            }
            inventory.erase(inventory.begin()+(long)i);
            if(deleteItem){
                delete item;
            }
            return true;
        }
    }
    return false;
}


Alive* Alive::cloneUnsafe(Alive* oldE, Alive* newE){

    Entity::cloneUnsafe(oldE, newE);

    newE->name = oldE->name;
    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->maxMp = oldE->maxMp;
    newE->mp = oldE->mp;
    newE->viewDistance = oldE->viewDistance;

    int activeWeaponIndex = -1;
    forVector(oldE->inventory, i){
        newE->inventory.push_back(Item::clone(oldE->inventory[i]));
        if(oldE->inventory[i] == oldE->activeWeapon){
            activeWeaponIndex = (int)i;
        }
    }
    if(activeWeaponIndex != -1){
    	newE->activeWeapon = dynamic_cast<Weapon*>(newE->inventory[activeWeaponIndex]);
    }

    newE->effects = oldE->effects;

    return newE;

}

int Alive::getEntityTypeId(){
    return ENTITY_TYPE_ALIVE;
}

void Alive::save(vector<unsigned char>* data){
    Entity::save(data);
    FileUtility::saveBool(data, dead);
    FileUtility::saveInt(data, maxHp);
    FileUtility::saveInt(data, hp);
    FileUtility::saveInt(data, maxMp);
    FileUtility::saveInt(data, mp);
    FileUtility::saveInt(data, viewDistance);
    FileUtility::saveString(data, name);

    //
    int activeWeaponIndex = -1;
    FileUtility::saveInt(data, (int)inventory.size());
    forVector(inventory, i){
        Item* ie = inventory[i];
        ie->save(data);
        if(ie == activeWeapon){
            activeWeaponIndex = (int)i;
        }
    }
    FileUtility::saveInt(data, activeWeaponIndex);
    //

    FileUtility::saveInt(data, (int)effects.size());
    for(Effect e : effects){
        FileUtility::saveInt(data, e.eId);
        FileUtility::saveDouble(data, e.timeEnd);
        FileUtility::saveInt(data, e.power);
        FileUtility::saveDouble(data, e.lastTime);
    }
}

void Alive::load(unsigned char* data, int* position){
    Entity::load(data, position);
    dead = FileUtility::loadBool(data, position);
    maxHp = FileUtility::loadInt(data, position);
    hp = FileUtility::loadInt(data, position);
    maxMp = FileUtility::loadInt(data, position);
    mp = FileUtility::loadInt(data, position);
    viewDistance = FileUtility::loadInt(data, position);
    name = FileUtility::loadString(data, position);

    //
    int size = FileUtility::loadInt(data, position);
    repeat(size, i){
        Item* item = Item::loadNew(data, position);
        debug("Loaded item: "+item->name+"("+to_string(item->getItemTypeId())+")");
        inventory.push_back(item);
    }

    int activeWeaponIndex = FileUtility::loadInt(data, position);
    if(activeWeaponIndex != -1){
    	activeWeapon = dynamic_cast<Weapon*>(inventory[activeWeaponIndex]);
    }else{
        activeWeapon = nullptr;
    }
    //

    size = FileUtility::loadInt(data, position);
    repeat(size, i){
        int eId = FileUtility::loadInt(data, position);
        int timeEnd = FileUtility::loadDouble(data, position);
        int power = FileUtility::loadInt(data, position);
        int lastTime = FileUtility::loadDouble(data, position);
        effects.push_back(Effect(eId, timeEnd, power, lastTime));
    }
}



