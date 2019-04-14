//
//  EntityAlive.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/2/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "EntityAlive.hpp"
#include "Utility.hpp"
#include "Level.hpp"
#include "Random.hpp"
#include "Verbalizer.hpp"

EntityAlive::EntityAlive() : EntityAlive("", ' ', Point2Zero, C_WHITE) {

}

EntityAlive::EntityAlive(string name, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : Entity(icon, startPos, colorCode) {

    this->name = name;
    this->maxHp = maxHp;
    this->hp = this->maxHp;
    this->solidity = tileFlagSolidOnGround;

}

EntityAlive::~EntityAlive() {

}

bool EntityAlive::update(double deltaTime, double time, Level* level) {

    if (dead) {
        if(level->tileAt(pos)->hasAllOfFlags(tileFlagReplaceable)){
            level->setTile(pos, Tiles::tileCorpse);
        }
        if(getEntityTypeId() != ENTITY_TYPE_PLAYER){
            level->removeEntity(this, true);
        }
    } else {
        Tile* tile = level->tileAt(pos);
        if(tile->hasAnyOfFlags(solidity)){
            hurt(level, damSuffocation, getMaxHp() * getMaxHp());
        }else if(tile->getIndex() == Tiles::tileFire->getIndex()){
            addEffect(Effect(effDamage, Random::randDouble(4, 8), 1, damFire));
        }

        int r = (int)(((double)hp/maxHp)*100.0);
        if(getHp()<getMaxHp() && ((int)getHp())>0 && rand() % (r==0?1:r) == 0){
            if(tile->hasAllOfFlags(tileFlagReplaceable)){
                level->setTile(pos, Tiles::tileBloodFloor);
            }
        }
        
        heal((healMultiplier*healBase)*deltaTime);
        healMana((healManaMultiplier*healManaBase)*deltaTime);
            
        healMultiplier *= 1+(.01*deltaTime);
        healManaMultiplier *= 1+(.01*deltaTime);

        bool removebad = false;

        forVector(effects, i) {
            Effect* e = &effects[i];

            double m = (e->timeLeft == 0) ? 1 : deltaTime;

            switch (e->eId) {
                case effDamage:
                    hurt(level, (DamageType)(int)e->meta, e->power * m);
                    switch ((DamageType)(int)e->meta) {
                        case damBlood:
                            if(level->tileAt(pos)->hasAllOfFlags(tileFlagReplaceable)){
                                level->setTile(pos, Tiles::tileBloodFloor);
                            }
                            break;

                        default:
                            break;
                    }
                    break;
                case effHeal:
                    if ((int)e->meta == 0) {
                        heal(e->power * m);
                    } else if ((int)e->meta == 1) {
                        healMana(e->power * m);
                    }
                    break;
                case effMultAttack:
                case effMultRecivedDamage:
                    //Applies on damage and hurt
                    break;
                case effPurity:
                    removebad = true;
                    break;
                default:
                    break;

            }

            if (e->timeLeft > 0) {
                e->timeLeft -= deltaTime;
            }
            if (e->timeLeft <= 0) {
                effects.erase(effects.begin()+(long) i);
                i--;
                effectsChanged();
                continue;
            }
            debug(e->toString());
        }

        if(removebad){
            forVector(effects, i) {
                Effect* e = &effects[i];

                if(isBadEffect(*e)){
                    e->timeLeft = 0;
                    effects.erase(effects.begin()+(long) i);
                    i--;
                    effectsChanged();
                    continue;
                }

            }
        }

    }

    return Entity::update(deltaTime, time, level);
}

double EntityAlive::hurt(Level* level, DamageType damageType, double amount, double damageMultiplier) {
    
    if(dead){
        return 0;
    }

    damageMultiplier *= getRecivedDamageMultiplierFromEffects(damageType);

    amount *= damageMultiplier;

    if(rand() % 20 == 0){
        if(level->tileAt(pos)->hasAllOfFlags(tileFlagReplaceable)){
            level->setTile(pos, Tiles::tileBloodFloor);
        }
    }

    healMultiplier = 0.05;
    healManaMultiplier = 0.05;

    //double amoutBeforeWeaknesses = amount;

    for (Weakness w : weaknesses) {
        if (w.damageType == damageType) {
            amount *= w.multiplier;
            //Verbalizer::weakness(this, w, amoutBeforeWeaknesses, amount);
        }
    }

    /*if (dodgeChance > 0 && rand() % (int)(dodgeChance*10000) > (dodgeChance*5000)) {
        amount = 0;
        consolef("Dodged!");
    }*/

    changeHp(-amount);
    //hp -= amount;
    if (getHp() <= 0 && !dead) {
        die();
    }
    return amount;
}

double EntityAlive::hurt(Level* level, ItemWeapon* w, double damageMultiplier) {
    double d = w->baseDamage * Random::randDouble(.8, 1);
    for (Enchantment ench : w->enchantments) {
        if (ench.style == eStyle_SelfToEnemy_EnemyEff && (rand() % ench.chance) == 0) {
            addEffect(ench.effect);
        }
    }

    return EntityAlive::hurt(level, w->damageType, d, damageMultiplier);
}

void EntityAlive::addEffect(Effect e) {

    forVector(effects, i) {
        Effect* ei = &effects[i];
        if (e.eId == ei->eId && e.meta == ei->meta) {
            ei->power = max(e.power, ei->power);
            ei->timeLeft = max(e.timeLeft, ei->timeLeft);
            return;
        }
    }
    effects.push_back(e);
    effectsChanged();
}

bool EntityAlive::hasEffect(EffectId eid){
    for(Effect e : effects) {
        if(e.eId == eid){
            return true;
        }
    }
    return false;
}

void EntityAlive::effectsChanged(){
    
}

void EntityAlive::heal(double amount) {
    if (dead) {
        return;
    }
    changeHp(amount);
}

void EntityAlive::healMana(double amount) {
    if (dead) {
        return;
    }
    changeMp(amount);
}

int EntityAlive::getEntityTypeId() {
    return ENTITY_TYPE_ALIVE;
}

void EntityAlive::save(vector<unsigned char>* data) {
    Entity::save(data);
    Utility::saveBool(data, dead);
    Utility::saveDouble(data, maxHp);
    Utility::saveDouble(data, hp);
    Utility::saveDouble(data, maxMp);
    Utility::saveDouble(data, mp);
    Utility::saveInt(data, viewDistance);
    Utility::saveString(data, name);
    Utility::saveInt(data, (int)weaknesses.size());
    for(Weakness w : weaknesses){
        w.save(data);
    }

    //
    //int activeItemWeaponIndex = -1;
    Utility::saveInt(data, (int) inventory.size());

    forVector(inventory, i) {
        Item* ie = inventory[i];
        ie->save(data);
        //if (ie == activeItemWeapon) {
            //activeItemWeaponIndex = i;
        //}
    }
    //Utility::saveInt(data, activeItemWeaponIndex);
    //

    Utility::saveInt(data, (int) effects.size());
    for (Effect e : effects) {
        e.save(data);
    }
}

void EntityAlive::load(vector<unsigned char>* data, int* position) {
    Entity::load(data, position);
    dead = Utility::loadBool(data, position);
    maxHp = Utility::loadDouble(data, position);
    hp = Utility::loadDouble(data, position);
    maxMp = Utility::loadDouble(data, position);
    mp = Utility::loadDouble(data, position);
    viewDistance = Utility::loadInt(data, position);
    name = Utility::loadString(data, position);
    int wc = Utility::loadInt(data, position);
    for(int i=0;i<wc;i++){
        weaknesses.push_back(Weakness(data, position));
    }

    //
    int size = Utility::loadInt(data, position);

    repeat(size, i) {
        Item* item = Item::loadNew(data, position);
        debug("Loaded item: " + item->getName(false) + "(" + to_string(item->getItemTypeId()) + ")");
        inventory.push_back(item);
    }

    /*int activeItemWeaponIndex = Utility::loadInt(data, position);
    if (activeItemWeaponIndex != -1) {
        activeItemWeapon = dynamic_cast<ItemWeapon*> (inventory[activeItemWeaponIndex]);
    } else {
        activeItemWeapon = nullptr;
    }*/
    //

    size = Utility::loadInt(data, position);

    repeat(size, i) {
        effects.push_back(Effect(data, position));
    }
}

bool EntityAlive::tryToMoveAbsolute(Point2 p, Level* level, bool force) {
    
    if(Entity::tryToMoveAbsolute(p, level, force)){
        int index = level->indexAt(p);
        if(index == Tiles::tileSpikes->getIndex() || index == Tiles::tileBloodSpikes->getIndex()){
            Effect e = Effect(effDamage, randomRange(10, 20), 10, damBlood);
            this->addEffect(e);
            level->setTile(p, Tiles::tileBloodSpikes);
        }
        return true;
    }else{
        return false;
    }
    
}
