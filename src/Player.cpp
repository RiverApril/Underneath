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



Player::Player() : Player("", ' ', Point2Zero, Ui::C_WHITE, Abilities<int>()){

}

Player::Player(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities) : Alive(name, icon, startPos, colorCode) {
    abilities = startAbilities;
    viewDistance = 14;
    updateVariablesForAbilities();
    for(int i=0;i<abilityCount;i++){
        setNextLevelXp();
    }
    hp = maxHp;
    mp = maxMp;
}

Player::~Player() {

}

bool Player::update(double deltaTime, double time, Level* level) {
    if(dead){
        level->currentWorld->currentPlayer = nullptr;
    }
    if(timeSinceHurt > 10){
        if(!outOfCombatHealing){
            lastHealTime = time;
            lastManaTime = time;
            healDelay = .5;
            manaDelay = .5;
            outOfCombatHealing = true;
        }
    }else{
        updateVariablesForAbilities();
        outOfCombatHealing = false;
    }
    timeSinceHurt += deltaTime;

    return Alive::update(deltaTime, time, level);
}

double Player::moveAbsalute(Point2 p, Level* level){
    if(tryToMoveAbsalute(p, level)){
        return moveDelay;
    }else{
        return interact(level, p, true, activeWeapon);
    }
    return 0;
}

double Player::moveRelative(Point2 p, Level* level) {
    return moveAbsalute(p+pos, level);
}

double Player::interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item){
    int tid = level->indexAt(posToInteract);
    for(Entity* e : level->entityList){
        if(e->uniqueId != uniqueId){
            if(e->pos == posToInteract){
                if(!needToBeSolid || e->isSolid()){
                    double d = interactWithEntity(level, e, posToInteract, item);
                    if(d>0){
                        return d;
                    }
                }
            }
        }
    }
    return interactWithTile(level, tid, posToInteract, item);
}

double Player::interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item){

    if(distanceSquared(posOfTile, pos) <= 1){

        if(Tiles::tileList[tid]->hasFlag(tileFlagHasTileEntity)){

            for(TileEntity* te : level->tileEntityList){
                //debugf("te's pos: %s", te->pos.toString().c_str());
                if(te->pos == posOfTile){
                    //debugf("te type id: %d", te->getTileEntityTypeId());
                    if(tid == Tiles::tileStairDown->getIndex() || tid == Tiles::tileStairUp->getIndex()){
                        TEStair* s = dynamic_cast<TEStair*>(te);
                        if(s){
                            //debugf("s->levelname: %s", s->levelName.c_str());
                            WorldLoader::changeLevel(level->currentWorld, s->pos, s->levelName);
                            return interactDelay;
                        }
                    }else if(tid == Tiles::tileChest->getIndex()){
                        level->menuGame->openMenu(new Ui::MenuChest(dynamic_cast<TEChest*>(te), this, level->currentWorld));
                    }
                }
            }

        }else if(tid == Tiles::tileOpenDoor->getIndex()){
            if(posOfTile != pos){
                level->setTile(posOfTile, Tiles::tileDoor);
                return interactDelay;
            }
        }else if(Tiles::tileList[tid]->hasFlag(tileFlagDoor)){
            level->setTile(posOfTile, Tiles::tileOpenDoor);
            return interactDelay;
        }
    }

    return 0;
}

double Player::interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item){

    if(e->removed){
        return 0;
    }

    Alive* a = dynamic_cast<Alive*>(e);
    if(a){
        if(item != nullptr){
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            Ranged* ranged = dynamic_cast<Ranged*>(item);
            Spell* spell = dynamic_cast<Spell*>(item);

            if(ranged){
                if(distanceSquared(pos, posOfEntity) > ranged->range*ranged->range){
                    console(Ui::colorCode(C_CODE_LIGHT_RED)+"Out of range!");
                    return 0;
                }
            }

            double x = 1;
            if(weapon){
                if(weapon->damageType == damMelee){
                    x = (double)abilities[iSTR] / (double)maxAbilities[iSTR];
                }else if(weapon->damageType == damRanged){
                    x = (double)abilities[iDEX] / (double)maxAbilities[iDEX];
                }else if(weapon->damageType == damMagic){
                    x = (double)abilities[iINT] / (double)maxAbilities[iINT];
                }
            }

            if(spell){
                if(mp >= spell->manaCost){
                    mp -= spell->manaCost;

                    double d = a->hurt(spell, level->currentWorld->worldTime, x+1);
                    Verbalizer::attack(this, a, spell, d);
                    //consolef("Dealt %.2f damage.", d);

                    return useDelay(item);
                }
                console(Ui::colorCode(C_CODE_LIGHT_RED)+"Not enough mana.");
                return 0;
            }

            if(weapon){

                double d = a->hurt(weapon, level->currentWorld->worldTime, x+1);
                Verbalizer::attack(this, a, weapon, d);
                //consolef("Dealt %.2f damage.", d);

                return useDelay(item);
            }

            return 0;
        }else{
            console(Ui::colorCode(C_CODE_LIGHT_RED)+"No item equiped.");
            return 0;
        }
    }

    if(distanceSquared(posOfEntity, pos) <= 1){

        ItemEntity* ie = dynamic_cast<ItemEntity*>(e);

        if(ie){
            Item* iei = ie->getItem();
            if(iei != nullptr){
                //debug("Picking up item: "+iei->getExtendedName());
                if(addItem(iei)){
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

double Player::waitUntilHealed(){
    //TODO
    debug("Not yet done, it just waits 5 time. It should wait until fully healed.");
    return waitDelay;
}


Player* Player::cloneUnsafe(Player* oldE, Player* newE){

    Alive::cloneUnsafe(oldE, newE);

    newE->abilities = oldE->abilities;
    newE->abilityPoints = oldE->abilityPoints;
    newE->level = oldE->level;
    newE->xp = oldE->xp;
    newE->nextLevelXp = oldE->nextLevelXp;
    newE->timeSinceHurt = oldE->timeSinceHurt;
    newE->outOfCombatHealing = oldE->outOfCombatHealing;

    return newE;
}

int Player::getEntityTypeId(){
    return ENTITY_TYPE_PLAYER;
}

void Player::save(vector<unsigned char>* data){
    Alive::save(data);
    abilities.save(data);
    FileUtility::saveInt(data, abilityPoints);
    FileUtility::saveInt(data, level);
    FileUtility::saveDouble(data, xp);
    FileUtility::saveDouble(data, nextLevelXp);
    FileUtility::saveInt(data, timeSinceHurt);
    FileUtility::saveBool(data, outOfCombatHealing);

}

void Player::load(unsigned char* data, int* position){
    Alive::load(data, position);
    abilities.load(data, position);
    abilityPoints = FileUtility::loadInt(data, position);
    level = FileUtility::loadInt(data, position);
    xp = FileUtility::loadDouble(data, position);
    nextLevelXp = FileUtility::loadDouble(data, position);
    timeSinceHurt = FileUtility::loadInt(data, position);
    outOfCombatHealing = FileUtility::loadBool(data, position);
}

double Player::useDelay(Item* item){
    Weapon* weapon = dynamic_cast<Weapon*>(item);
    double d = 0;
    if(weapon){
        d = weapon->useDelay;
    }else{
        d = 1;
    }
    return d * (1-((double)abilities[iAGI] / maxAbilities[iAGI]));

}

void Player::gainXp(double amount){
    xp += amount;
    double l = nextLevelXp;
    while(xp >= l){
        xp -= l;
        abilityPoints += 1;
        level += 1;
        setNextLevelXp();
        Verbalizer::leveledUp(this);
    }
}


