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
#include "MenuDialog.hpp"
#include "MenuChest.hpp"
#include "Verbalizer.hpp"
#include "ItemPotion.hpp"
#include "ItemUtilitySpell.hpp"
#include "ItemSpecial.hpp"
#include "EnemyGenerator.hpp"
#include "EntityExplosive.hpp"
#include "Icon.hpp"
#include "MenuShop.hpp"
#include "Animator.hpp"
#include "Settings.hpp"
#include "ItemAreaOfEffectWeapon.hpp"
#include "EntityMulti.hpp"
#include "TEAlter.hpp"
#include "Offers.hpp"

EntityPlayer::EntityPlayer() : EntityPlayer("", ' ', Point2Zero, C_WHITE, Abilities<int>()) {

}

EntityPlayer::EntityPlayer(string name, char icon, Point2 startPos, Ui::Color colorCode, Abilities<int> startAbilities) : EntityAlive(name, icon, startPos, colorCode) {
    abilities = startAbilities;
    viewDistance = 14;
    updateVariablesForAbilities();
    for (int i = 0; i < abilityCount; i++) {
        setNextLevelXp();
    }
    setHpAndMpToMax();
}

EntityPlayer::~EntityPlayer() {

}

bool EntityPlayer::update(double deltaTime, double time, Level* level) {
    if (dead) {
        level->currentWorld->currentPlayer = nullptr;
    }

    timeSinceCombat += deltaTime;

    enchTick += deltaTime;
    if(enchTick >= 1){
        enchTick -= 1;
        for(pair<EquipSlot, Item*> p : equipedItems){
            ItemArmor* a = dynamic_cast<ItemArmor*>(p.second);
            ItemWeapon* w = dynamic_cast<ItemWeapon*>(p.second);
            if(a){
                for(Enchantment e : a->enchantments){
                    if(e.style == eStyle_onTick_SelfEff && (rand() % e.chance) == 0){
                        addEffect(e.effect);
                    }
                }
            }else if(w){
                for(Enchantment e : w->enchantments){
                    if(e.style == eStyle_onTick_SelfEff && (rand() % e.chance) == 0){
                        addEffect(e.effect);
                    }
                }
            }
        }
    }

    return EntityAlive::update(deltaTime, time, level);
}

double EntityPlayer::moveAbsalute(Point2 p, Level* level, bool canInteract, bool force) {
    if(hasEffect(effStun)){
        console("You are stunned! You can only wait.");
        return 1;
    }
    if (tryToMoveAbsalute(p, level, force)) {
        return moveDelay;
    } else if(canInteract) {
        return interact(level, p, true, getActiveItemWeapon(), false);
    }
    return 0;
}

double EntityPlayer::moveRelative(Point2 p, Level* level) {
    return moveAbsalute(p + pos, level, true);
}

double EntityPlayer::useItemOnOther(Item* itemToUse, Item* itemToBeUsedOn){
    /*ItemSpecial* is = dynamic_cast<ItemSpecial*>(itemToUse);
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
    }*/
    return 0;
}

int EntityPlayer::repairToolPower(){
    return (abilities[iSTR] * 10) + (abilities[iDEX] * 15) + 100;
}

double EntityPlayer::interact(Level* level, Point2 posToInteract, bool needToBeSolid, Item* item, bool okayToInteractWithSelf) {
    
    
    if(hasEffect(effStun)){
        console("You are stunned! You can only wait.");
        return 1;
    }


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

        ItemExplosive* ita = dynamic_cast<ItemExplosive*>(item);
        if(ita){
            level->newEntity(new EntityExplosive(ita, pos, posToInteract, 1, uniqueId));
            if (item->qty == 1) {
                removeItem(item, false);
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
            } else if (getMp() >= s->manaCost) {
                changeMp(-s->manaCost);
                use = 1;
            }
            if (use) {
                switch (s->spellEffect) {
                    case spellRemoteUse:
                        if (!interact(level, posToInteract, false, nullptr, false)) {
                            consolef("&%cFailed to interact", Ui::cc(C_LIGHT_RED));
                            return 0;
                        }
                        break;
                    case spellRelocate:
                        if (posToInteract == pos) {
                            posToInteract = level->findRandomWithoutFlag(getSolidity());
                        }
                        
                        moveAbsalute(posToInteract, level, false, true);
                        break;
                    case spellBarrier:{
                        bool success = false;
                        for(int i=-1;i<=1;i++){
                            for(int j=-1;j<=1;j++){
                                Point2 p = posToInteract+Point2(i, j);
                                if(level->tileAt(p)->hasAllOfFlags(tileFlagReplaceable)){
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
                        EntityShop* e = new EntityShop("Merchant", aiNone, 'M', posToInteract, C_LIGHT_MAGENTA, 100);
                        e->addItems(ItemGenerator::makeLoot(ItemGenerator::lootProfileShop, level->getDifficulty(), (rand()%9000)+1000, 10, 20, 2));
                        e->addItem(ItemGenerator::makeCoins(1000));
                        level->newEntity(e);
                        break;
                    }
                    case spellDebugPlaceBoss:{
                        Entity* e = EnemyGenerator::makeBossVenom(posToInteract, level->getDifficulty());
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
                if (!needToBeSolid || (bool)(e->getSolidity() & getSolidity())) {
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
            if(posToInteract == pos && okayToInteractWithSelf){
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
        double dis = distanceSquared(posOfTile, pos);
        if (dis > 1) {
            ItemAreaOfEffectWeapon* aoe = dynamic_cast<ItemAreaOfEffectWeapon*>(item);
            if(aoe){
                use = false;
                if(aoe->range * aoe->range < dis){
                    consolef("&%cOut of range!", Ui::cc(C_LIGHT_RED));
                    return 0;
                } else if(!level->canSee(pos, posOfTile, aoe->range)){
                    consolef("&%cNo line of sight!", Ui::cc(C_LIGHT_RED));
                    return 0;
                } else if(getMp() < aoe->manaCost){
                    consolef("&%cNot enough mana!", Ui::cc(C_LIGHT_RED));
                    return 0;
                } else {
                    changeMp(-aoe->manaCost);
                    BasicIcon* icon = new BasicIcon('*', damageTypeColor(weapon->damageType), C_BLACK);
                    Animator::renderRangedAttack(pos, posOfTile, icon, level, 4);
                    Animator::renderExposion(posOfTile, aoe->radius, level, 1, aoe->damageType);
                    level->explode(posOfTile, aoe->radius, aoe->baseDamage, false, aoe->damageType);
                    return useDelay(aoe);
                }
            }
        }
    }

    if (use) {
        if (Tiles::tileList[tid]->hasAllOfFlags(tileFlagHasTileEntity)) {

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
                                    int l = abilities[iLUK];
                                    tec->addItems(ItemGenerator::makeLoot(tec->lootProfileIndex, level->getDifficulty(), (rand()%500), 5, 10+l, 5));
                                    tec->lootProfileIndex = -1;
                                }
                                level->currentWorld->menuGame->openMenu(new Ui::MenuChest(tec, this, level->currentWorld));
                            }
                            return interactDelay;
                        }

                        case TILE_ENTITY_TYPE_STAIR:
                        {
                            TEStair* s = dynamic_cast<TEStair*> (te);
                            if (s) {
                                int delay = interactDelay;
                                World* cw = level->currentWorld;
                                WorldLoader::changeLevel(level->currentWorld, s->pos, s->levelName);
                                cw->menuGame->levelHasChanged();
                                //level no longer the currentLevel
                                return delay;
                            }
                        }
                            
                        case TILE_ENTITY_TYPE_ALTER:
                        {
                            if(tid == Tiles::tileAlter->getIndex()){
                                TEAlter* tea = dynamic_cast<TEAlter*>(te);
                                if(tea->offerIndex != -1){
                                    level->currentWorld->offers->list[tea->offerIndex]->presentOffering(level->currentWorld->menuGame);
                                    level->removeTileEntity(tea);
                                    level->setTile(posOfTile, Tiles::tileBrokenAlter);
                                }
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
        } else if (Tiles::tileList[tid]->hasAllOfFlags(tileFlagDoor)) {

            if(Tiles::tileList[tid]->hasAllOfFlags(tileFlagMonsterSpawningDoor)){

                EnemyGenerator::setIntervals(level->getDifficulty());

                int aa = 0;
                int ee = 0;
                int tt = 0;

                do{
                    tt = 0;
                    aa++;
                    Utility::spreadUnexploredTileExecute(level, posOfTile, Tiles::tileFloor->getIndex(), [level, &tt, &ee](int x, int y){
                        tt++;
                        if(rand() % 30 == 0){
                            EntityAi* e = EnemyGenerator::makeRandomEntity(level->getDifficulty());
                            e->pos = Point2(x, y);
                            level->newEntity(e);
                            ee++;
                        }
                    });
                }while(ee < max(1, (int)pow(tt, 1/6)) && aa < 20 && tt > 0);
                
                if(rand() % 50 == 0){
                    
                    EntityAi* e = EnemyGenerator::makeEntity(EnemyGenerator::mimic, level->getDifficulty());
                    
                    e->pos = posOfTile;

                    level->newEntity(e);
                    level->setTile(posOfTile, Tiles::tileFloor);
                    consolef("Ahh! That's no door!");
                }

                if(Tiles::tileList[tid]->getIndex() == Tiles::tileSecretDoor->getIndex()){
                    consolef("You found a secret door!");
                }

            }

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

                int l = abilities[iLUK];

                if(rand()%3 == 0){
                    items = ItemGenerator::makeLoot(ItemGenerator::lootProfileCrate, level->getDifficulty(), (rand()%(50+l*4)), 1, 1, max(1, 10-(l)));
                }else{
                    items.push_back(ItemGenerator::makeCoins((rand() % (30+l*2)) + 1));
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
        x *= getAttackMultiplierFromEffectsAndEquips(weapon->damageType);
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
            

            vector<string> message;
            if(rand()%5 == 0){
                message = {"It's dangerous to go alone", "", "", "...but I'm not coming with you."};
            }else{
                message = {"Welcome! I'm glad I found someone down here.", "Would you like to trade wares?"};
            }
            
            level->currentWorld->menuGame->openMenu(new Ui::MenuDialog(message, {"Trade", "No thanks"}, [this, e, level](Ui::MenuDialog* menu, int result){
                if(result == 0){
                    menu->openMenu(new Ui::MenuShop(dynamic_cast<EntityShop*>(e), this, level->currentWorld));
                }else{
                    menu->closeThisMenu();
                }
                //result 1 is cancel
                //result -1 is escape
                //these will just exit
            }, true));
            
            return interactDelay;
        }

    }

    EntityAlive* a = dynamic_cast<EntityAlive*> (e);
    if (a/* && (e->uniqueId != uniqueId)*/) {
        if (item != nullptr) {
            ItemWeapon* weapon = dynamic_cast<ItemWeapon*> (item);
            ItemRanged* ranged = dynamic_cast<ItemRanged*> (item);
            ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*> (item);
            ItemAreaOfEffectWeapon* aoe = dynamic_cast<ItemAreaOfEffectWeapon*> (item);

            if(aoe){
                return 0;
            }

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

            if(rand()%max(2, 100-abilities[iLUK])==0){
                x *= 2;
                consolef("&%cCritical!", Ui::cc(C_LIGHT_GREEN));
            }

            if (spell) {
                if (getMp() >= spell->manaCost) {
                    changeMp(-spell->manaCost);

                    timeSinceCombat = 0;
                    double d = a->hurt(level, spell, x + 1);
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

                double ud = useDelay(item);

                /*if(weapon->durability > -1){
                    if(weapon->durability <= 0){
                        consolef("&%cYour weapon is broken!", Ui::cc(C_LIGHT_RED));
                    }else if(weapon->durability < 16){
                        consolef("&%cYour weapon is almost broken!", Ui::cc(C_LIGHT_RED));
                    }
                    if(weapon->durability > 0){
                        weapon->durability--;
                    }
                }*/

                double d = a->hurt(level, weapon, x);
                Verbalizer::attack(this, a, weapon, d);

                if(ranged){
                    switch (ranged->rangedType) {
                        case rangedUnlimited:
                            break;

                        case rangedOneUse:
                            removeItemQty(1, ranged, true);
                            break;
                        case rangedOneUseRecoverable:
                            if(distanceSquared(posOfEntity, pos) > 1){
                                level->newEntity(new EntityItem(Item::clone(ranged, 1), posOfEntity));
                                removeItemQty(1, ranged, true);
                            }
                            break;
                    }
                    BasicIcon* icon = new BasicIcon('*', damageTypeColor(weapon->damageType), C_BLACK);
                    Animator::renderRangedAttack(pos, posOfEntity, icon, level, 1);
                    delete icon;

                }

                return ud;
            }

            return 0;
        } else {
            consolef("&%cNo item equiped!", Ui::cc(C_LIGHT_RED));
            return 0;
        }
    }

    EntityExplosive* eti = dynamic_cast<EntityExplosive*>(e);
    if(eti){
        eti->activate(level);
        return interactDelay;
    }

    return 0;
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
    Utility::saveDouble(data, enchTick);

    int size = 0;
    for(pair<EquipSlot, Item*> p : equipedItems){
        if(p.second){
            size++;
        }
    }

    Utility::saveInt(data, size);

    for(pair<EquipSlot, Item*> p : equipedItems){
        if(p.second){
            forVector(inventory, i) {
                if (inventory[i] == p.second) {
                    Utility::saveInt(data, p.first);
                    Utility::saveInt(data, i);
                    break;
                }
            }
        }
    }
    
    for(Item* item : favItems){
        if(item){
            bool found = false;
            forVector(inventory, i) {
                if (inventory[i] == item) {
                    Utility::saveInt(data, i);
                    found = true;
                    break;
                }
            }
            if(!found){
                Utility::saveInt(data, -1);
            }
        }else{
            Utility::saveInt(data, -1);
        }
    }
    
    for(int i = 0; i < specialCOUNT; i++){
        Utility::saveBool(data, specials[i]);
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
    enchTick = Utility::loadDouble(data, position);

    int size = Utility::loadInt(data, position);

    for(int i=0;i<size;i++){
        EquipSlot slot = (EquipSlot)Utility::loadInt(data, position);
        int index = Utility::loadInt(data, position);
        if(index != -1){
            equipedItems[slot] = inventory[index];
        }
    }
    
    for(int i=0;i<10;i++){
        int index = Utility::loadInt(data, position);
        if(index != -1){
            favItems[i] = inventory[index];
        }
    }
    
    for(int i = 0; i < specialCOUNT; i++){
        specials[i] = Utility::loadBool(data, position);
    }
	
    updateVariablesForAbilities();

}

double EntityPlayer::useDelay(Item* item) {
    if(!item){
    	return interactDelay;
    }
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
        for(pair<EquipSlot, Item*> p : equipedItems){
            if (item == p.second) {
                equipItem(nullptr, p.first);
            }
        }
        for(int i=0;i<10;i++){
            if (item == favItems[i]) {
                setFav(nullptr, i);
            }
        }
    }
    return Inventory::removeItem(item, deleteItem);
}

bool EntityPlayer::equipItem(ItemEquipable* newItem){
    return equipItemWithIgnoreList(newItem, {});
}

bool EntityPlayer::equipItemWithIgnoreList(ItemEquipable* newItem, vector<EquipSlot> ignoreSlots){
    if(newItem){
        vector<EquipSlot> vs = newItem->getViableSlots();
        while(ignoreSlots.size() > 0){
            size_t i;
            for(i = 0; i<vs.size(); i++){
                if(vs[i] == ignoreSlots[0]){
                    vs.erase(vs.begin()+i);
                    break;
                }
            }
            ignoreSlots.erase(ignoreSlots.begin());
        }
        if(vs.size() > 0){
            EquipSlot currentSlot = slotNone;
            for(EquipSlot slot : vs){
                if(newItem == equipedItems[slot]){
                    currentSlot = slot;
                    break;
                }
            }
            if(currentSlot == slotNone){
                for(EquipSlot slot : vs){
                    if(!equipedItems[slot]){
                        return equipItem(newItem, slot);
                    }
                }
                return equipItem(newItem, vs[vs.size()-1]);
            }else{
                if(currentSlot == vs[0]){
                    return equipItem(nullptr, currentSlot);
                }else{
                    EquipSlot x = slotNone;
                    for(int i=(int)vs.size()-1;i>=0;i--){
                        if(equipedItems[vs[i]] && vs[i] != currentSlot){
                            x = vs[i];
                            break;
                        }
                    }
                    equipItem(equipedItems[x], currentSlot);
                    return equipItem(newItem, x);
                }
            }
        }
    }
    return false;
}

bool EntityPlayer::equipItem(Item* newItem, EquipSlot slot){

    try{

        if(newItem){
            if(getSlot(newItem) == slot){
                equipItem(nullptr, slot);
                return true;
            }
        	equipItem(nullptr, getSlot(newItem));
        }

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

            if(!newItem->canBeEquipedHere(slot)){
                consolef("Cannot be equipped here: %d", slot);
                throw false;
            }

            if(slot == slotNone || slot >= slotQty){
                consolef("Slot out of range: %d", slot);
                throw false;
            }
            ItemEquipable* eqp = dynamic_cast<ItemEquipable*>(newItem);
            if(eqp){

                for (int i = 0; i < abilityCount; i++) {
                    if (eqp->minimumAbilities[i] > abilities[i]) {
                        consolef("Required ability %s too high: %d", abilityAbr[i].c_str(), eqp->minimumAbilities[i]);
                        throw false;
                    }
                }

                for(pair<EquipSlot, Item*> p : equipedItems){
                    if(p.second){
                        ItemEquipable* eqpSec = dynamic_cast<ItemEquipable*>(p.second);
                        if(eqpSec){
                            if(eqpSec->blocksSlot(slot, p.first)){
                                equipedItems[p.first] = nullptr;
                            }
                        }
                    }
                }

            }



            if(alreadyHave){
                equipedItems[slot] = newItem;
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
    equipItem(newItemWeapon, slotWep1);
}

double EntityPlayer::hurt(Level* level, DamageType damageType, double amount, double damageMultiplier) {
    damageMultiplier = hurtMiddleStep(level, damageType, damageMultiplier);

    return EntityAlive::hurt(level, damageType, amount, damageMultiplier);
}

double EntityPlayer::hurt(Level* level, ItemWeapon* w, double damageMultiplier) {
    damageMultiplier = hurtMiddleStep(level, w->damageType, damageMultiplier);

    return EntityAlive::hurt(level, w, damageMultiplier);
}

double EntityPlayer::hurtMiddleStep(Level* level, DamageType damageType, double damageMultiplier){
    timeSinceCombat = 0;
    if(Settings::godMode){
        return 0;
    }

    damageMultiplier *= getRecivedDamageMultiplierFromArmorAndEquips(damageType);

    for(pair<EquipSlot, Item*> p : equipedItems){
        if(p.second){

        }
    }

    return damageMultiplier;
}

void EntityPlayer::updateVariablesForAbilities() {
    moveDelay = max(1*pow(0.98, abilities[iAGI]), 0.0001);
    healBase = specials[specialDisableRegen] ? 0 : 1 + (abilities[iCON]*0.01);
    healManaBase = specials[specialDisableRegen] ? 0 : 1 + (abilities[iWIS]*0.01);
    interactDelay = max(.1*pow(0.99, abilities[iAGI]), 0.0001);

    setMaxHpAndMp((specials[specialHalfHpMp] ? .5 : 1) * (100 + (abilities[iCON] * 5)),
                  (specials[specialHalfHpMp] ? .5 : 1) * (0 + (abilities[iWIS] * 5)));

    dodgeChance = max(1*pow(0.995, abilities[iAGI]), 0.0001);

    strMult = 1 + (((double) abilities[iSTR]*0.075));
    dexMult = 1 + (((double) abilities[iDEX]*0.075));
    intMult = 1 + (((double) abilities[iINT]*0.075));
    for(Effect eff : effects){
        if(eff.eId == effMultiMoveDelay){
            moveDelay *= eff.power;
        }
    }
    
    setPoolHpMp(specials[specialPoolHpMp]);
}

int EntityPlayer::xpForLevel(int l){
    l+=1;
    return (pow(l, 2)+(5*l)+20);
}

double EntityPlayer::getRecivedDamageMultiplierFromArmorAndEquips(DamageType damType){

    double def = 1;

    for(pair<EquipSlot, Item*> p : equipedItems){
        if(p.second){
            ItemArmor* armor = dynamic_cast<ItemArmor*>(p.second);
            if(armor){
                for(Defense d : armor->defenses){
                    if(d.damageType == damType || d.damageType == damNone){
                        def *= (1.0-d.amount);
                    }
                }
            }
        }
    }

    for(pair<EquipSlot, Item*> p : equipedItems){
        ItemArmor* a = dynamic_cast<ItemArmor*>(p.second);
        ItemWeapon* w = dynamic_cast<ItemWeapon*>(p.second);
        if(a){
            for(Enchantment e : a->enchantments){
                if(e.style == eStyle_EnemyToSelf_SelfEff && e.effect.eId == effMultRecivedDamage){
                    if((int)e.effect.meta == damType || e.effect.meta == damNone){
                        def *= e.effect.power;
                    }
                }
            }
        }else if(w){
            for(Enchantment e : w->enchantments){
                if(e.style == eStyle_EnemyToSelf_SelfEff && e.effect.eId == effMultRecivedDamage){
                    if((int)e.effect.meta == damType || e.effect.meta == damNone){
                        def *= e.effect.power;
                    }
                }
            }
        }
    }

    return def;
}

double EntityPlayer::getAttackMultiplierFromEffectsAndEquips(DamageType damType){
    double d = 1;
    for(Effect eff : effects){
        if(eff.eId == effMultRecivedDamage){
            if((int)eff.meta == damType || eff.meta == damNone){
                d *= eff.power;
            }
        }
    }
    for(pair<EquipSlot, Item*> p : equipedItems){
        ItemArmor* a = dynamic_cast<ItemArmor*>(p.second);
        ItemWeapon* w = dynamic_cast<ItemWeapon*>(p.second);
        if(a){
            for(Enchantment e : a->enchantments){
                if(e.style == eStyle_SelfToEnemy_SelfEff && e.effect.eId == effMultRecivedDamage){
                    if((int)e.effect.meta == damType || e.effect.meta == damNone){
                        d *= e.effect.power;
                    }
                }
            }
        }else if(w){
            for(Enchantment e : w->enchantments){
                if(e.style == eStyle_SelfToEnemy_SelfEff && e.effect.eId == effMultRecivedDamage){
                    if((int)e.effect.meta == damType || e.effect.meta == damNone){
                        d *= e.effect.power;
                    }
                }
            }
        }
    }
    return d;
}

Item* EntityPlayer::getFav(int fav){
    return favItems[fav];
}

void EntityPlayer::setFav(Item* item, int fav){
    for(int i=0;i<10;i++){
        if(getFav(i) == item){
            favItems[i] = nullptr;
        }
    }
    favItems[fav] = item;
}

void EntityPlayer::effectsChanged(){
    updateVariablesForAbilities();
}

void EntityPlayer::changeSpecial(Special sp, bool enable){
    if(enable == specials[sp]){
        return;
    }
    specials[sp] = enable;
    updateVariablesForAbilities();
}



