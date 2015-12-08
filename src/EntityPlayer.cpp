//
//  EntityPlayer.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/18/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "EntityPlayer.hpp"
#include "Level.hpp"
#include "EntityItem.hpp"
#include "EntityShop.hpp"
#include "MenuChest.hpp"
#include "Verbalizer.hpp"
#include "ItemPotion.hpp"
#include "ItemUtilitySpell.hpp"
#include "ItemSpecial.hpp"
#include "EnemyGenerator.hpp"
#include "EntityTimeActivated.hpp"
#include "Icon.hpp"
#include "MenuShop.hpp"
#include "Animator.hpp"
#include "Settings.hpp"

EntityPlayer::EntityPlayer() : EntityPlayer("", ' ', Point2Zero, C_WHITE, Abilities<int>()) {

}

EntityPlayer::EntityPlayer(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities) : EntityAlive(name, icon, startPos, colorCode) {
    abilities = startAbilities;
    viewDistance = 14;
    updateVariablesForAbilities();
    for (int i = 0; i < abilityCount; i++) {
        setNextLevelXp();
    }
    hp = maxHp;
    mp = maxMp;
}

EntityPlayer::~EntityPlayer() {

}

bool EntityPlayer::update(double deltaTime, double time, Level* level) {
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

    return EntityAlive::update(deltaTime, time, level);
}

double EntityPlayer::moveAbsalute(Point2 p, Level* level, bool canInteract) {
    if (tryToMoveAbsalute(p, level)) {
        return moveDelay;
    } else if(canInteract) {
        return interact(level, p, true, getActiveItemWeapon());
    }
    return 0;
}

double EntityPlayer::moveRelative(Point2 p, Level* level) {
    return moveAbsalute(p + pos, level, true);
}

double EntityPlayer::useItemOnOther(Item* itemToUse, Item* itemToBeUsedOn){
    ItemSpecial* is = dynamic_cast<ItemSpecial*>(itemToUse);
    if(is){
        ItemEquipable* ie = dynamic_cast<ItemEquipable*>(itemToBeUsedOn);
        if(ie){
            if(ie->durability > -1){
                int rep = repairToolPower();
                ie->durability += rep;
                consolef("Repaired item %d durability points.", rep);
                if (itemToUse->qty == 1) {
                    removeItem(itemToUse, true);
                } else {
                    itemToUse->qty -= 1;
                }
                return interactDelay;
            }
        }
    }
    return 0;
}

int EntityPlayer::repairToolPower(){
    return (abilities[iSTR] * 10) + (abilities[iDEX] * 15) + 100;
}

double EntityPlayer::interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item) {



    if (item == nullptr || containsItem(item)) {


        ItemPotion* p = dynamic_cast<ItemPotion*> (item);
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

        ItemUtilitySpell* s = dynamic_cast<ItemUtilitySpell*> (item);
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
                            consolef("&%cFailed to interact", Ui::cc(C_LIGHT_RED));
                            return 0;
                        }
                        break;
                    case spellRelocate:
                        if (posToInteract == pos) {
                            posToInteract = level->findRandomWithoutFlag(tileFlagSolid);
                        }
                        if (!moveAbsalute(posToInteract, level, false)) {
                            hurt(damSuffocation, maxHp * 2);
                        }
                        break;
                    case spellBarrier:{
                        bool success = false;
                        for(int i=-1;i<=1;i++){
                            for(int j=-1;j<=1;j++){
                                Point2 p = posToInteract+Point2(i, j);
                                if(level->tileAt(p)->hasFlag(tileFlagReplaceable)){
                                    if(level->firstEntityHere(p) == nullptr){
                                        level->setTile(p, Tiles::tileBreakable);
                                        success = true;
                                    }
                                }
                            }
                        }
                        if(!success){
                            consolef("&%cAll tiles are ocupied", Ui::cc(C_LIGHT_RED));
                            return 0;
                        }
                        /*if(level->tileAt(posToInteract)->hasFlag(tileFlagReplaceable)){
                            if(level->firstEntityHere(posToInteract) == nullptr){
                                level->setTile(posToInteract, Tiles::tileBreakable);
                            }else{
                                consolef("&%cTile ocupied by entity", Ui::cc(C_LIGHT_RED));
                                return 0;
                            }
                        }else{
                            consolef("&%cTile ocupied", Ui::cc(C_LIGHT_RED));
                            return 0;
                        }*/
                        break;
                    }
                    case spellDebugPlaceWall:
                        level->setTile(posToInteract, Tiles::tileWall);
                        break;
                    case spellDebugPlaceFloor:
                        level->setTile(posToInteract, Tiles::tileFloor);
                        break;
                    case spellDebugPlaceGoblin:{
                        EntityAi* e = EnemyGenerator::makeEntity(EnemyGenerator::goblinScout, level->getDifficulty());
                        e->pos = posToInteract;
                        level->newEntity(e);
                        break;
                    }
                    case spellDebugPlaceShop:{
                        EntityShop* e = new EntityShop("Shop keeper", aiNone, 'S', posToInteract, C_LIGHT_MAGENTA, 100);
                        //e->addItems(ItemGenerator::createRandLoots(level->getDifficulty(), , 0, 10, 10, 10, 2));
                        level->newEntity(e);
                        break;
                    }
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



        for (Entity* e : level->entityList) {
            if (e->uniqueId != uniqueId) { //Don't interact with yourself yet.
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
        int tid = level->indexAt(posToInteract);
        double delay = interactWithTile(level, tid, posToInteract, item); //Interact with tile
        if(delay > 0){
            return delay;
        }else{
            //If there is no tile to interact with, try interacting with yourself.
            if(posToInteract == pos){
            	return interactWithEntity(level, this, posToInteract, item);
            }
        }
    }
    return 0;
}

double EntityPlayer::interactWithTile(Level* level, int tid, Point2 posOfTile, Item* item) {

    bool use = true;

    ItemWeapon* weapon = dynamic_cast<ItemWeapon*> (item);
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
                                TEChest* tec = dynamic_cast<TEChest*> (te);
                                if(tec->lootProfileIndex != -1){
                                    tec->addItems(ItemGenerator::makeLoot(tec->lootProfileIndex, level->getDifficulty(), (rand()%500), 10, 15, 2));
                                    tec->lootProfileIndex = -1;
                                }
                                level->currentWorld->menuGame->openMenu(new Ui::MenuChest(tec, this, level->currentWorld));
                            }
                            return interactDelay;
                        }

                        case TILE_ENTITY_TYPE_MIMIC:
                        {
                            EntityAi* e = EnemyGenerator::makeEntity(EnemyGenerator::mimic, level->getDifficulty());

                            //Tile* t = Tiles::getTile(tid);

                            ////Icon* was saying it was private, no idea why.
                            //auto ti = t->getIcon(true);

                            //e->defaultIcon = 'M';//ti->getChar(0, posOfTile, level);
                            //e->fgColor = ti->getFgColor(0, posOfTile, level);
                            //e->bgColor = ti->getBgColor(0, posOfTile, level);

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
                                int delay = interactDelay;
                                WorldLoader::changeLevel(level->currentWorld, s->pos, s->levelName);
                                //level no longer the currentLevel
                                return delay;
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
        } else if (tid == Tiles::tileLockedDoor->getIndex()) {
            if(containsItemEqualingExceptQty(ItemGenerator::smallKey->original(), 1)){
            	level->setTile(posOfTile, Tiles::tileOpenDoor);
                consolef("Opened the door using up a &%cKey&%c.", Ui::cc(C_LIGHT_MAGENTA), Ui::cc(C_WHITE));
            	return interactDelay;
            }else{
                consolef("This door requires a &%cKey&%c.", Ui::cc(C_LIGHT_MAGENTA), Ui::cc(C_WHITE));
            }
        } else if (tid == Tiles::tileBreakable->getIndex()) {
            level->setTile(posOfTile, Tiles::tileRubble);
            return interactDelay;
        } else if (tid == Tiles::tileCrate->getIndex()) {

            if(rand()%3 == 0){

                vector<Item*> items;

                if(rand()%3 == 0){
                    items = ItemGenerator::makeLoot(ItemGenerator::lootProfileCrate, level->getDifficulty(), (rand()%50), 1, 1, 10);
                }else{
                    items.push_back(ItemGenerator::makeCoins((rand() % 30) + 1));
                }

                for (Item* i : items) {
                    level->newEntity(new EntityItem(Item::clone(i), posOfTile));
                }
            }
            level->setTile(posOfTile, Tiles::tileFloor);
            return interactDelay;
        }
    }

    return 0;
}

double EntityPlayer::calcDamageMultiplier(ItemWeapon* weapon) {
    double x = 1;
    if (weapon) {
        if (weapon->weaponType == wepMelee) {
            x = strMult;
        } else if (weapon->weaponType == wepRanged) {
            x = dexMult;
        } else if (weapon->weaponType == wepMagic) {
            x = intMult;
        }
        x *= getAttackMultiplierFromEffectsAndArmor(weapon->damageType);
    }
    if(weapon->durability < 0){
        x /= 2;
    }
    return x;
}

double EntityPlayer::interactWithEntity(Level* level, Entity* e, Point2 posOfEntity, Item* item) {

    if (e->removed) {
        return 0;
    }

    if (distanceSquared(posOfEntity, pos) <= 1) {

        EntityItem* ie = dynamic_cast<EntityItem*> (e);
        EntityShop* is = dynamic_cast<EntityShop*> (e);

        if (ie) {
            Item* iei = ie->getItem();
            if (iei != nullptr) {
                if (addItem(iei)) {
                    ie->setItemToNull();
                    level->removeEntity(e, true);
                    return interactDelay;
                }
            }
            level->removeEntity(e, true);
            return 0;
        }else if(is){
            level->currentWorld->menuGame->openMenu(new Ui::MenuShop(is, this, level->currentWorld));
            return interactDelay;
        }

    }

    EntityAlive* a = dynamic_cast<EntityAlive*> (e);
    if (a && e->uniqueId != uniqueId) {
        if (item != nullptr) {
            ItemWeapon* weapon = dynamic_cast<ItemWeapon*> (item);
            ItemRanged* ranged = dynamic_cast<ItemRanged*> (item);
            ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*> (item);

            if (ranged) {
                if (distanceSquared(pos, posOfEntity) > ranged->range * ranged->range) {
                    consolef("&%cOut of range!", Ui::cc(C_LIGHT_RED));
                    return 0;
                } else if (!level->canSee(pos, posOfEntity, viewDistance)) {
                    consolef("&%cNo line of sight!", Ui::cc(C_LIGHT_RED));
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

                    BasicIcon* icon = new BasicIcon('*', damageTypeColor(weapon->damageType), C_BLACK);
                    Animator::renderRangedAttack(pos, posOfEntity, icon, level, 8);
                    delete icon;

                    return useDelay(item);
                }
                consolef("&%cNot enough mana!", Ui::cc(C_LIGHT_RED));
                return 0;
            }

            if (weapon) {
                timeSinceCombat = 0;

                if(weapon->durability > -1){
                    if(weapon->durability <= 0){
                        consolef("&%cYour weapon is broken!", Ui::cc(C_LIGHT_RED));
                    }else if(weapon->durability < 16){
                        consolef("&%cYour weapon is almost broken!", Ui::cc(C_LIGHT_RED));
                    }
                    if(weapon->durability > 0){
                        weapon->durability--;
                    }
                }

                double d = a->hurt(weapon, x);
                Verbalizer::attack(this, a, weapon, d);

                BasicIcon* icon = new BasicIcon('*', damageTypeColor(weapon->damageType), C_BLACK);
                Animator::renderRangedAttack(pos, posOfEntity, icon, level, 1);
                delete icon;

                return useDelay(item);
            }

            return 0;
        } else {
            consolef("&%cNo item equiped!", Ui::cc(C_LIGHT_RED));
            return 0;
        }
    }

    EntityTimeActivated* eti = dynamic_cast<EntityTimeActivated*>(e);
    if(eti){
        eti->activate(level);
        return interactDelay;
    }

    return 0;
}

EntityPlayer* EntityPlayer::cloneUnsafe(EntityPlayer* oldE, EntityPlayer* newE) {

    EntityAlive::cloneUnsafe(oldE, newE);

    newE->abilities = oldE->abilities;
    newE->abilityPoints = oldE->abilityPoints;
    newE->level = oldE->level;
    newE->xp = oldE->xp;
    newE->nextLevelXp = oldE->nextLevelXp;
    newE->timeSinceCombat = oldE->timeSinceCombat;
    newE->outOfCombatHealing = oldE->outOfCombatHealing;

    forVector(oldE->inventory, i) {
        for(pair<EquipSlot, ItemEquipable*> p : oldE->equipedItems){
            if (oldE->inventory[i] == p.second) {
                newE->equipedItems[p.first] = dynamic_cast<ItemEquipable*> (newE->inventory[i]);
            }
        }
    }

    newE->updateVariablesForAbilities();

    return newE;
}

int EntityPlayer::getEntityTypeId() {
    return ENTITY_TYPE_PLAYER;
}

void EntityPlayer::save(vector<unsigned char>* data) {
    EntityAlive::save(data);
    abilities.save(data);
    Utility::saveInt(data, abilityPoints);
    Utility::saveInt(data, level);
    Utility::saveDouble(data, xp);
    Utility::saveDouble(data, nextLevelXp);
    Utility::saveInt(data, timeSinceCombat);
    Utility::saveBool(data, outOfCombatHealing);

    Utility::saveInt(data, (int)equipedItems.size());

    for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
    	forVector(inventory, i) {
            if (inventory[i] == p.second) {
                Utility::saveInt(data, p.first);
                Utility::saveInt(data, i);
                break;
            }
        }
    }

}

void EntityPlayer::load(vector<unsigned char>* data, int* position) {
    EntityAlive::load(data, position);
    abilities.load(data, position);
    abilityPoints = Utility::loadInt(data, position);
    level = Utility::loadInt(data, position);
    xp = Utility::loadDouble(data, position);
    nextLevelXp = Utility::loadDouble(data, position);
    timeSinceCombat = Utility::loadInt(data, position);
    outOfCombatHealing = Utility::loadBool(data, position);

    int size = Utility::loadInt(data, position);

    for(int i=0;i<size;i++){
        int slot = Utility::loadInt(data, position);
        int index = Utility::loadInt(data, position);
        equipedItems[slot] = dynamic_cast<ItemEquipable*>(inventory[index]);
    }
	
    updateVariablesForAbilities();

}

double EntityPlayer::useDelay(Item* item) {
    ItemWeapon* weapon = dynamic_cast<ItemWeapon*> (item);
    double d = 0;
    if (weapon) {
        d = weapon->useDelay;
    } else {
        d = 1;
    }
    return d * interactDelay*10;

}

void EntityPlayer::gainXp(double amount) {
    xp += amount;
    double l = nextLevelXp;
    while (xp >= l) {
        xp -= l;
        abilityPoints += 1;
        level += 1;
        setNextLevelXp();
        Verbalizer::leveledUp(this);
        leveledUp = true;
    }
}

bool EntityPlayer::removeItem(Item* item, bool deleteItem) {
    if (item) {
        for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
            if (item == p.second) {
                equipItem(nullptr, p.first);
            }
        }
    }
    return Inventory::removeItem(item, deleteItem);
}

bool EntityPlayer::equipItem(ItemEquipable* newItem, bool forceDefaultSlot){
    if(newItem){
        vector<EquipSlot> vs = newItem->getViableSlots();
        if(vs.size() > 0){
            if(!forceDefaultSlot){
                for(EquipSlot slot : vs){
                    if(equipedItems[slot] == nullptr){
                        return equipItem(newItem, slot);
                    }
                }
            }
            return equipItem(newItem, vs[0]);
        }
    }
    return false;
}

bool EntityPlayer::equipItem(ItemEquipable* newItem, EquipSlot slot){

    try{

        bool alreadyHave = false;

        for (Item* ie : inventory) {
            if (ie == newItem) {
                alreadyHave = true;
                break;
            }
        }

        if(newItem){

            if(!alreadyHave){
                addItem(newItem);
                equipedItems[slot] = newItem;
            }

            if(slot == 0 || slot > slotMAX){
                consolef("Slot out of range: %d", slot);
                throw false;
            }

            for (int i = 0; i < abilityCount; i++) {
                if (newItem->minimumAbilities[i] > abilities[i]) {
                    consolef("Ability %s too high: %d", abilityAbr[i].c_str(), newItem->minimumAbilities[i]);
                    throw false;
                }
            }

            if(!newItem->canBeEquipedHere(slot)){
                consolef("Cannot be equipped here: %d", slot);
                throw false;
            }

            for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
                if(p.second){
                    if(p.second->blocksSlot(slot, p.first)){
                        equipedItems[p.first] = nullptr;
                    }
                }
            }



            if(alreadyHave){
                equipedItems[slot] = dynamic_cast<ItemEquipable*> (newItem);
                throw true;
            }else{
                throw false;
            }

        } else {
            equipedItems[slot] = nullptr;
            throw true;
        }
    }catch(bool changed){
        return changed;
    }

}

void EntityPlayer::setActiveItemWeapon(ItemWeapon* newItemWeapon) {
    equipItem(newItemWeapon);
}

double EntityPlayer::hurt(DamageType damageType, double amount, double damageMultiplier) {
    timeSinceCombat = 0;
    if(Settings::godMode){
        return 0;
    }

    damageMultiplier *= 1.0 - getDefenseMultiplierFromArmor(damageType, true);

    return EntityAlive::hurt(damageType, amount, damageMultiplier);
}

double EntityPlayer::hurt(ItemWeapon* w, double damageMultiplier) {
    timeSinceCombat = 0;
    if(Settings::godMode){
        return 0;
    }

    damageMultiplier *= 1.0 - getDefenseMultiplierFromArmor(w->damageType, true);

    return EntityAlive::hurt(w, damageMultiplier);
}

void EntityPlayer::updateVariablesForAbilities() {
    moveDelay = max(1*pow(0.98, abilities[iSPD]), 0.0001);
    if (outOfCombatHealing) {
        healDelay = max(.5*pow(0.96, abilities[iCON]), 0.0001);
        manaDelay = max(.5*pow(0.96, abilities[iWIS]), 0.0001);
    } else {
        healDelay = max(20*pow(0.96, abilities[iCON]), 0.0001);
        manaDelay = max(20*pow(0.96, abilities[iWIS]), 0.0001);
    }
    interactDelay = max(.1*pow(0.96, abilities[iSPD]), 0.0001);

    maxHp = 100 + (abilities[iCON] * 5);
    maxMp = 0 + (abilities[iWIS] * 5);

    dodgeChance = max(1*pow(0.995, abilities[iAGI]), 0.0001);

    strMult = 1 + (((double) abilities[iSTR]/10.0));
    dexMult = 1 + (((double) abilities[iDEX]/10.0));
    intMult = 1 + (((double) abilities[iINT]/10.0));
}

int EntityPlayer::xpForLevel(int l){
    l+=1;
    return (pow(l, 2)+(5*l)+20);
}

double EntityPlayer::getDefenseMultiplierFromArmor(DamageType damType, bool reduceDurability){

    if(reduceDurability){
        if (rand() > (RAND_MAX * dodgeChance)) {
            return 1;
        }
    }

    double def = 0;

    for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
        if(p.second){
            ItemArmor* armor = dynamic_cast<ItemArmor*>(p.second);
            if(armor){
                for(Defense d : armor->defenses){
                    if(d.damageType == damType || d.damageType == -1){
                        if(reduceDurability){
                            if(armor->durability > -1){
                                if(armor->durability == 0){
                                    consolef("&%cA peice of your armor is broken!", Ui::cc(C_LIGHT_RED));
                                }else if(armor->durability < 16){
                                    consolef("&%cA peice of your armor is almost broken!", Ui::cc(C_LIGHT_RED));
                                }
                                if(armor->durability > 0){
                                    armor->durability--;
                                }
                            }
                        }
                        if(armor->durability > 0){
                            def += d.amount;
                        }else{
                            def += d.amount / 2;
                        }
                    }
                }
            }
        }
    }

    return def;
}

double EntityPlayer::getAttackMultiplierFromEffectsAndArmor(DamageType damType){
    double d = 1;
    for(Effect eff : effects){
        if(eff.eId == effBuffAttack){
            if((int)eff.meta == damType){
                d += eff.power;
            }
        }
    }
    for(pair<EquipSlot, ItemEquipable*> p : equipedItems){
        ItemArmor* a = dynamic_cast<ItemArmor*>(p.second);
        if(a){
            for(Enchantment e : a->enchantments){
                if(e.effectId == effBuffAttack){
                    if((int)e.meta == damType){
                        d += e.power;
                    }
                }
            }
        }
    }
    return d;
}
