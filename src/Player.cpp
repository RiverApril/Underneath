//
//  Player.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Player.h"
#include "Level.h"
#include "ItemEntity.h"
#include "MenuChest.h"
#include "Verbalizer.h"
#include "Potion.h"
#include "UtilitySpell.h"
#include "EnemyGenerator.h"
#include "EntityTimeActivated.h"

Player::Player() : Player("", ' ', Point2Zero, Ui::C_WHITE, Abilities<int>()) {

}

Player::Player(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities) : Alive(name, icon, startPos, colorCode) {
    abilities = startAbilities;
    viewDistance = 14;
    updateVariablesForAbilities();
    for (int i = 0; i < abilityCount; i++) {
        setNextLevelXp();
    }
    hp = maxHp;
    mp = maxMp;
}

Player::~Player() {

}

bool Player::update(double deltaTime, double time, Level* level) {
    if (dead) {
        level->currentWorld->currentPlayer = nullptr;
    }
    if (timeSinceCombat > 30) {
        if (!outOfCombatHealing) {
            lastHealTime = time;
            lastManaTime = time;
            outOfCombatHealing = true;
            updateVariablesForAbilities();
        }
    } else {
        outOfCombatHealing = false;
        updateVariablesForAbilities();
    }
    timeSinceCombat += deltaTime;

    return Alive::update(deltaTime, time, level);
}

double Player::moveAbsalute(Point2 p, Level* level) {
    if (tryToMoveAbsalute(p, level)) {
        return moveDelay;
    } else {
        return interact(level, p, true, activeWeapon);
    }
    return 0;
}

double Player::moveRelative(Point2 p, Level* level) {
    return moveAbsalute(p + pos, level);
}

double Player::interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item) {



    if (item == nullptr || containsItem(item)) {


        Potion* p = dynamic_cast<Potion*> (item);
        if (p) {
            for (Effect e : p->effects) {
                addEffect(e);
            }
            if (item->qty == 1) {
                removeItem(item, false);
            } else {
                item->qty -= 1;
            }
            return interactDelay;
        }

        ItemTimeActivated* ita = dynamic_cast<ItemTimeActivated*>(item);
        if(ita){
            level->newEntity(new EntityTimeActivated(ita, posToInteract));
            if (item->qty == 1) {
                removeItem(item, true);
            } else {
                item->qty -= 1;
            }
            return interactDelay;
        }

        UtilitySpell* s = dynamic_cast<UtilitySpell*> (item);
        if (s) {
            int use = 0;
            if (s->manaCost == -1) {
                use = 2;
            } else if (mp >= s->manaCost) {
                mp -= s->manaCost;
                use = 1;
            }
            if (use) {
                switch (s->spellEffect) {
                    case spellRemoteUse:
                        if (!interact(level, posToInteract, false, nullptr)) {
                            return 0;
                        }
                        break;
                    case spellRelocate:
                        if (posToInteract == pos) {
                            posToInteract = level->findRandomWithoutFlag(tileFlagSolid);
                        }
                        if (!moveAbsalute(posToInteract, level)) {
                            hurt(damSuffocation, maxHp * 2);
                            return 0;
                        }
                        break;
                }
                if (use == 2) {
                    if (item->qty == 1) {
                        removeItem(item, true);
                    } else {
                        item->qty -= 1;
                    }
                }
                return interactDelay;
            }
        }



        int tid = level->indexAt(posToInteract);
        for (Entity* e : level->entityList) {
            if (e->uniqueId != uniqueId) {
                if (!needToBeSolid || e->isSolid()) {
                    if (e->pos == posToInteract) {
                        double d = interactWithEntity(level, e, posToInteract, item);
                        if (d > 0) {
                            return d;
                        }
                    }
                }
            }
        }
        return interactWithTile(level, tid, posToInteract, item);
    }
    return 0;
}

double Player::interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item) {

    bool use = true;

    Weapon* weapon = dynamic_cast<Weapon*> (item);
    if (weapon) {
        if (distanceSquared(posOfTile, pos) > 1) {
            use = false;
        }
    }

    if (use) {
        if (Tiles::tileList[tid]->hasFlag(tileFlagHasTileEntity)) {

            for (TileEntity* te : level->tileEntityList) {
                //debugf("te's pos: %s", te->pos.toString().c_str());
                if (te->pos == posOfTile) {
                    //debugf("te type id: %d", te->getTileEntityTypeId());
                    switch (te->getTileEntityTypeId()) {

                        case TILE_ENTITY_TYPE_CHEST:
                        {
                            if (tid == Tiles::tileChest->getIndex()) {
                                level->menuGame->openMenu(new Ui::MenuChest(dynamic_cast<TEChest*> (te), this, level->currentWorld));
                            } else if (tid == Tiles::tileCrate->getIndex()) {
                                TEChest* c = dynamic_cast<TEChest*> (te);
                                for (Item* i : c->inventory) {
                                    level->newEntity(new ItemEntity(i, posOfTile));
                                }
                                level->removeTileEntity(c);
                                level->setTile(posOfTile, Tiles::tileFloor);
                            }
                            return interactDelay;
                        }

                        case TILE_ENTITY_TYPE_MIMIC:
                        {
                            AiEntity* e = EnemyGenerator::makeEntity(EnemyGenerator::mimic, level->getDifficulty());

                            Tile* t = Tiles::getTile(tid);

                            e->defaultIcon = t->getIcon();
                            e->fgColorCode = t->getFgColor(true);
                            e->bgColorCode = t->getBgColor(true);

                            e->pos = posOfTile;

                            level->newEntity(e);
                            level->removeTileEntity(te);
                            level->setTile(posOfTile, Tiles::tileFloor);
                            return interactDelay;
                        }

                        case TILE_ENTITY_TYPE_STAIR:
                        {
                            TEStair* s = dynamic_cast<TEStair*> (te);
                            if (s) {
                                WorldLoader::changeLevel(level->currentWorld, s->pos, s->levelName);
                                return interactDelay;
                            }
                        }
                    }
                }
            }

        }
        if (tid == Tiles::tileOpenDoor->getIndex()) {
            if (posOfTile != pos) {
                level->setTile(posOfTile, Tiles::tileDoor);
                return interactDelay;
            }
        } else if (Tiles::tileList[tid]->hasFlag(tileFlagDoor)) {
            level->setTile(posOfTile, Tiles::tileOpenDoor);
            return interactDelay;
        } else if (tid == Tiles::tileCrate->getIndex()) {
            level->setTile(posOfTile, Tiles::tileFloor);
            return interactDelay;
        }
    }

    return 0;
}

double Player::calcDamageMultiplier(Weapon* weapon) {
    double x = 1;
    if (weapon) {
        if (weapon->weaponType == wepMelee) {
            x = 1 + (((double) abilities[iSTR] / (double) maxAbilities[iSTR]) * 10.0);
        } else if (weapon->weaponType == wepRanged) {
            x = 1 + (((double) abilities[iDEX] / (double) maxAbilities[iDEX]) * 10.0);
        } else if (weapon->weaponType == wepMagic) {
            x = 1 + (((double) abilities[iINT] / (double) maxAbilities[iINT]) * 10.0);
        }
    }
    return x;
}

double Player::interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item) {

    if (e->removed) {
        return 0;
    }

    Alive* a = dynamic_cast<Alive*> (e);
    if (a) {
        if (item != nullptr) {
            Weapon* weapon = dynamic_cast<Weapon*> (item);
            Ranged* ranged = dynamic_cast<Ranged*> (item);
            CombatSpell* spell = dynamic_cast<CombatSpell*> (item);

            if (ranged) {
                if (distanceSquared(pos, posOfEntity) > ranged->range * ranged->range) {
                    console(Ui::colorCode(C_CODE_LIGHT_RED) + "Out of range!");
                    return 0;
                } else if (!level->canSee(pos, posOfEntity, viewDistance, false)) {
                    console(Ui::colorCode(C_CODE_LIGHT_RED) + "No line of sight!");
                    return 0;
                }
            }

            double x = calcDamageMultiplier(weapon);

            if (spell) {
                if (mp >= spell->manaCost) {
                    mp -= spell->manaCost;

                    timeSinceCombat = 0;
                    double d = a->hurt(spell, x + 1);
                    Verbalizer::attack(this, a, spell, d);

                    return useDelay(item);
                }
                console(Ui::colorCode(C_CODE_LIGHT_RED) + "Not enough mana.");
                return 0;
            }

            if (weapon) {
                timeSinceCombat = 0;
                double d = a->hurt(weapon, x);
                Verbalizer::attack(this, a, weapon, d);

                return useDelay(item);
            }

            return 0;
        } else {
            console(Ui::colorCode(C_CODE_LIGHT_RED) + "No item equiped.");
            return 0;
        }
    }

    if (distanceSquared(posOfEntity, pos) <= 1) {

        ItemEntity* ie = dynamic_cast<ItemEntity*> (e);

        if (ie) {
            Item* iei = ie->getItem();
            if (iei != nullptr) {
                //debug("Picking up item: "+iei->getExtendedName());
                if (addItem(iei)) {
                    level->removeEntity(e, true);
                    return interactDelay;
                }
            }
            level->removeEntity(e, true);
            return 0;
        }

    }

    return 0;
}

Player* Player::cloneUnsafe(Player* oldE, Player* newE) {

    Alive::cloneUnsafe(oldE, newE);

    newE->abilities = oldE->abilities;
    newE->abilityPoints = oldE->abilityPoints;
    newE->level = oldE->level;
    newE->xp = oldE->xp;
    newE->nextLevelXp = oldE->nextLevelXp;
    newE->timeSinceCombat = oldE->timeSinceCombat;
    newE->outOfCombatHealing = oldE->outOfCombatHealing;

    return newE;
}

int Player::getEntityTypeId() {
    return ENTITY_TYPE_PLAYER;
}

void Player::save(vector<unsigned char>* data) {
    Alive::save(data);
    abilities.save(data);
    Utility::saveInt(data, abilityPoints);
    Utility::saveInt(data, level);
    Utility::saveDouble(data, xp);
    Utility::saveDouble(data, nextLevelXp);
    Utility::saveInt(data, timeSinceCombat);
    Utility::saveBool(data, outOfCombatHealing);

}

void Player::load(unsigned char* data, int* position) {
    Alive::load(data, position);
    abilities.load(data, position);
    abilityPoints = Utility::loadInt(data, position);
    level = Utility::loadInt(data, position);
    xp = Utility::loadDouble(data, position);
    nextLevelXp = Utility::loadDouble(data, position);
    timeSinceCombat = Utility::loadInt(data, position);
    outOfCombatHealing = Utility::loadBool(data, position);
}

double Player::useDelay(Item* item) {
    Weapon* weapon = dynamic_cast<Weapon*> (item);
    double d = 0;
    if (weapon) {
        d = weapon->useDelay;
    } else {
        d = 1;
    }
    return d * (1 - ((double) abilities[iAGI] / maxAbilities[iAGI]));

}

void Player::gainXp(double amount) {
    xp += amount;
    double l = nextLevelXp;
    while (xp >= l) {
        xp -= l;
        abilityPoints += 1;
        level += 1;
        setNextLevelXp();
        Verbalizer::leveledUp(this);
    }
}

void Player::setActiveWeapon(Weapon* newWeapon) {
    if (newWeapon) {
        for (int i = 0; i < abilityCount; i++) {
            if (newWeapon->minimumAbilities[i] > abilities[i]) {
                return;
            }
        }
    }

    Alive::setActiveWeapon(newWeapon);
}
