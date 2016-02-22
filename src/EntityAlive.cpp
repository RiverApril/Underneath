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
            hurt(level, damSuffocation, maxHp * maxHp);
        }else if(tile->getIndex() == Tiles::tileFire->getIndex()){
            addEffect(Effect(effDamage, Random::randDouble(4, 8), 1, damFire));
        }

        if(hp<maxHp && ((int)hp)>0 && rand() % (int)(((double)hp/maxHp)*100.0) == 0){
            if(tile->hasAllOfFlags(tileFlagReplaceable)){
                level->setTile(pos, Tiles::tileBloodFloor);
            }
        }
        
        heal((healMultiplier*healBase)*deltaTime, false);
        healMana((healManaMultiplier*healManaBase)*deltaTime, false);
            
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
                        heal(e->power * m, e->timeLeft == 0);
                    } else if ((int)e->meta == 1) {
                        healMana(e->power * m, e->timeLeft == 0);
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
                    continue;
                }

            }
        }

    }

    return Entity::update(deltaTime, time, level);
}

double EntityAlive::hurt(Level* level, DamageType damageType, double amount, double damageMultiplier) {

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

    hp -= amount;
    if (hp <= 0 && !dead) {
        die();
    }
    return amount;
}

double EntityAlive::hurt(Level* level, ItemWeapon* w, double damageMultiplier) {
    double d = w->baseDamage * Random::randDouble(.8, 1);
    for (Enchantment ench : w->enchantments) {
        if (ench.style == eStyle_SelfToEnemy_EnemyEff && rand() % ench.chance == 0) {
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
}

bool EntityAlive::hasEffect(EffectId eid){
    for(Effect e : effects) {
        if(e.eId == eid){
            return true;
        }
    }
    return false;
}

double EntityAlive::heal(double amount, bool overload) {
    if (dead) {
        return 0;
    }
    hp += amount;
    if (hp > maxHp && !overload) {
        double a = amount - (hp - maxHp);
        hp = maxHp;
        return a;
    }

    return amount;
}

double EntityAlive::healMana(double amount, bool overload) {
    if (dead) {
        return 0;
    }
    mp += amount;
    if (mp > maxMp && !overload) {
        double a = amount - (mp - maxMp);
        mp = maxMp;
        return a;
    }

    return amount;
}

EntityAlive* EntityAlive::cloneUnsafe(EntityAlive* oldE, EntityAlive* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->name = oldE->name;
    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->maxMp = oldE->maxMp;
    newE->mp = oldE->mp;
    newE->viewDistance = oldE->viewDistance;

    newE->removeAllItems(true);
    forVector(oldE->inventory, i) {
        newE->inventory.push_back(Item::clone(oldE->inventory[i]));
    }

    newE->effects = oldE->effects;

    return newE;

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
