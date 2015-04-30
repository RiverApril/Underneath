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

Alive::Alive() : Alive("", ' ', Point2Zero, Ui::C_WHITE) {

}

Alive::Alive(string name, char icon, Point2 startPos, Ui::Color colorCode, int maxHp) : Entity(icon, startPos, colorCode) {

    this->name = name;
    this->maxHp = maxHp;
    this->hp = this->maxHp;
    this->solid = true;

}

Alive::~Alive() {
    
}

bool Alive::update(double deltaTime, double time, Level* level) {

    if (dead) {
        level->removeEntity(this, true);
    } else {
        while (lastHealTime + healDelay <= time) {
            heal(1);
            lastHealTime += healDelay;
        }
        while (lastManaTime + manaDelay <= time) {
            healMana(1);
            lastManaTime += manaDelay;
        }

        forVector(effects, i) {
            Effect* e = &effects[i];

            double m = (e->timeLeft == 0) ? 1 : deltaTime;

            switch (e->eId) {
                case effDamage:
                    hurt((int) e->meta, e->power * m);
                    break;
                case effHeal:
                    if (e->meta == 0) {
                        heal(e->power * m);
                    } else if (e->meta == 1) {
                        healMana(e->power * m);
                    }
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
    }

    return Entity::update(deltaTime, time, level);
}

double Alive::hurt(DamageType damageType, double amount, double damageMultiplier) {
    amount *= damageMultiplier;

    for (Weakness w : weaknesses) {
        if (w.damageType == damageType) {
            amount *= w.multiplier;
        }
    }

    hp -= amount;
    if (hp <= 0 && !dead) {
        die();
    }
    return amount;
}

double Alive::hurt(Weapon* w, double damageMultiplier) {
    double d = w->baseDamage * Random::randDouble(.5, 1);
    for (Enchantment ench : w->enchantments) {
        if (rand() % ench.chance == 0) {
            addEffect(Effect(ench.effectId, ench.time, ench.power).setMeta(ench.meta));
        }
    }
    return hurt(w->damageType, d, damageMultiplier);
}

void Alive::addEffect(Effect e) {

    forVector(effects, i) {
        Effect ei = effects[i];
        if (e.eId == ei.eId && ei.power == e.power && e.meta == ei.meta) {
            ei.timeLeft = max(e.timeLeft, ei.timeLeft);
            return;
        }
    }
    effects.push_back(e);
}

double Alive::heal(double amount) {
    if (dead) {
        return 0;
    }
    hp += amount;
    if (hp > maxHp) {
        double a = amount - (hp - maxHp);
        hp = maxHp;
        return a;
    }
    return amount;
}

double Alive::healMana(double amount) {
    if (dead) {
        return 0;
    }
    mp += amount;
    if (mp > maxMp) {
        double a = amount - (mp - maxMp);
        mp = maxMp;
        return a;
    }
    return amount;
}

Alive* Alive::cloneUnsafe(Alive* oldE, Alive* newE) {

    Entity::cloneUnsafe(oldE, newE);

    newE->name = oldE->name;
    newE->maxHp = oldE->maxHp;
    newE->hp = oldE->hp;
    newE->maxMp = oldE->maxMp;
    newE->mp = oldE->mp;
    newE->viewDistance = oldE->viewDistance;

    forVector(oldE->inventory, i) {
        newE->inventory.push_back(Item::clone(oldE->inventory[i]));
    }

    newE->effects = oldE->effects;

    return newE;

}

int Alive::getEntityTypeId() {
    return ENTITY_TYPE_ALIVE;
}

void Alive::save(vector<unsigned char>* data) {
    Entity::save(data);
    Utility::saveBool(data, dead);
    Utility::saveDouble(data, maxHp);
    Utility::saveDouble(data, hp);
    Utility::saveDouble(data, maxMp);
    Utility::saveDouble(data, mp);
    Utility::saveInt(data, viewDistance);
    Utility::saveString(data, name);

    //
    //int activeWeaponIndex = -1;
    Utility::saveInt(data, (int) inventory.size());

    forVector(inventory, i) {
        Item* ie = inventory[i];
        ie->save(data);
        //if (ie == activeWeapon) {
            //activeWeaponIndex = i;
        //}
    }
    //Utility::saveInt(data, activeWeaponIndex);
    //

    Utility::saveInt(data, (int) effects.size());
    for (Effect e : effects) {
        e.save(data);
    }
}

void Alive::load(unsigned char* data, int* position) {
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
        debug("Loaded item: " + item->name + "(" + to_string(item->getItemTypeId()) + ")");
        inventory.push_back(item);
    }

    /*int activeWeaponIndex = Utility::loadInt(data, position);
    if (activeWeaponIndex != -1) {
        activeWeapon = dynamic_cast<Weapon*> (inventory[activeWeaponIndex]);
    } else {
        activeWeapon = nullptr;
    }*/
    //

    size = Utility::loadInt(data, position);

    repeat(size, i) {
        effects.push_back(Effect(data, position));
    }
}
