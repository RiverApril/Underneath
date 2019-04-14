//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.hpp"
#include "MenuMain.hpp"
#include "MenuEscape.hpp"
#include "MenuInv.hpp"
#include "MenuStats.hpp"
#include "MenuYesNo.hpp"
#include "MenuDebug.hpp"
#include "MenuMessage.hpp"
#include "MenuUseInInv.hpp"
#include "EntityAi.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Controls.hpp"
#include "ItemRanged.hpp"
#include "ItemUtilitySpell.hpp"
#include "Settings.hpp"
#include "Audio.hpp"
#include "EntityMultiSub.hpp"

#define currentPlayer (currentWorld->currentPlayer)
#define currentLevel (currentWorld->currentLevel)


namespace Ui {

    MenuGame::MenuGame(string worldName, Abilities<int> playerAbilities) : Menu() {

        printConsoleByDefault = false;

        //move(0, 0);
        //clrtobot();
        //refresh();

        initSuccess = init(worldName, playerAbilities);

        viewPos = Point2Zero;
        viewMoveSpeed = Point2(2, 1);

        /*if(currentLevel != nullptr){
                currentLevel->setAndUnsetDisplayEntities();
        }*/
        if (initSuccess) {
            currentWorld->menuGame = this;
        }else{
            openMenu(new MenuMessage(formatString("Unable to load world: \"%s\"", worldName.c_str())));
        }

        viewUpdate();

        spinyIcon = new AnimatedIcon({'-', '\\', '|', '/'}, 1, C_WHITE, C_BLACK);

    }

    MenuGame::~MenuGame() {
        delete currentWorld;
        delete saveAnswer;
        delete useItem;
        delete spinyIcon;
    }

    void MenuGame::levelHasChanged(){
        
        fadeIn = fadeInMin;

        if(currentLevel->getDifficulty() == 0){
            Audio::setBgMusic(Audio::soundMenu);
        }else{
            Audio::setBgMusic(Audio::soundTheme);
        }

    }

    bool MenuGame::init(string worldName, Abilities<int> playerAbilities) {

        debug("MenuGame::init()");

        if (worldName.size() == 0) {
            return false;
        }

        if (WorldLoader::exists(worldName)) {
            currentWorld = WorldLoader::load(nullptr, worldName);
            debug("Attempting to load world...");
            if (currentWorld == nullptr) {
                debug("Failed to load world, creating a new one...");
                currentWorld = WorldLoader::create(worldName, playerAbilities);
            }
        } else {
            debug("Creating new world...");
            currentWorld = WorldLoader::create(worldName, playerAbilities);
        }
        if (currentWorld == nullptr || currentLevel == nullptr) {
            return false;
        }
        
        levelHasChanged();


        return true;
    }

    bool MenuGame::openUi() {
        debug("Open MenuGame Ui");

        if(reopenInventory){
            reopenInventory = false;
            openMenu(new MenuInv(currentWorld, useItem));
            return true;
        }

        if (*saveAnswer == aYes) {

            WorldLoader::save(currentWorld);
            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        } else if (*saveAnswer == aNo) {

            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        }

        if (*useItem != -1) {
            UseType use = currentPlayer->inventory[*useItem]->getUseType();
            if (use == useInstant) {
                timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, currentPlayer->inventory[*useItem], false);
                int selected = *useItem;
                *useItem = -1;
                MenuInv* m = new MenuInv(currentWorld, useItem);
                m->selected = selected;
                openMenu(m);
            } else if(use == useInWorld){
                itemToBeUsed = currentPlayer->inventory[*useItem];
                if (!currentLevel->canSee(currentPlayer->pos, targetPosition, currentPlayer->viewDistance)) {
                    targetPosition = currentPlayer->pos;
                }
                changeMode(modeSelectPosition);
                *useItem = -1;
            } else if(use == useInInventory){
                MenuUseInInv* m = new MenuUseInInv(currentPlayer, currentWorld, *useItem);
                *useItem = -1;
                openMenu(m);
                reopenInventory = true;
            }
        }

        if (!initSuccess) {
            closeThisMenu();
            return false;
        }

        setGameAreaSize();

        move(0, 0);
        clrtobot();
        //refresh();
        return true;
    }

    void MenuGame::closeUi() {

    }

    void MenuGame::setGameAreaSize() {
        gameArea.x = terminalSize.x - borderSize.x;
        gameArea.y = terminalSize.y - borderSize.y;
    }

    void MenuGame::viewUpdate() {
        if(currentWorld){
            if(currentLevel){
                if (currentPlayer) {
                    if(controlMode == modeSelectPosition){
                        viewPos = targetPosition;
                    }else{
                        viewPos = currentPlayer->pos;
                    }
                    viewPos -= (gameArea / 2);
                }
            }
        }
    }

    void MenuGame::drawTileAt(Point2 p) {
        if(rumble > 0){
            p.x += rumbleX;
            p.y += rumbleY;
        }
        Ui::Color fg = C_LIGHT_RED;
        Ui::Color bg = C_BLACK;
        char symbol = ' ';

        bool inView = false;
        if (currentLevel) {
            if(currentPlayer && !currentPlayer->dead){
                if ((currentLevel->canSee(p, currentPlayer->pos, currentPlayer->viewDistance))) {
                    inView = true;
                }
            }else{
                inView = false;
            }
            if(inView){
                currentLevel->setExplored(p, true);
            }
            if (!currentLevel->getExplored(p) && !Settings::seeEverything) {
                symbol = ' ';
            } else {
                Tile* tempTile = currentLevel->tileAt(p);

                Icon* tempIcon = tempTile->getIcon(inView);

                fg = tempIcon->getFgColor(tick, p, currentLevel);
                bg = tempIcon->getBgColor(tick, p, currentLevel);
                symbol = tempIcon->getChar(tick, p, currentLevel);

                if (currentLevel->inRange(p)) {
                    Entity* e = nullptr;
                    int d = -1000000;
                    for (Entity* ei : currentLevel->entityList) {
                        if (ei) {
                            if (!ei->removed) {
                                if (ei->pos == p) {
                                    int dd = ei->getRenderDepth();
                                    if (d < dd) {
                                        e = ei;
                                        d = dd;
                                    }
                                }
                            }
                        }
                    }
                    if (e) {

                        if (inView || Settings::seeEverything) {
                            /*if (controlMode == modeSelectDirection && currentPlayer == e) {
                                fg = e->getBgColor(tick, p, currentLevel);
                                bg = e->getFgColor(tick, p, currentLevel);
                            } else */{
                                fg = e->getFgColor(tick, p, currentLevel);
                                bg = e->getBgColor(tick, p, currentLevel);
                            }

                            symbol = e->getChar(tick, p, currentLevel);

                        }

                    }
                }
            }
            if (controlMode == modeSelectEntity) {
                if (p == targetPosition) {
                    bg = C_LIGHT_BLUE;
                }
            } else if (controlMode == modeSelectPosition) {
                if (p == targetPosition) {
                    bg = C_LIGHT_GREEN;
                }

            }

            if(currentPlayer && currentPlayer->hasEffect(effMemory)){
                if(!inView && !Settings::seeEverything){
                    bg = C_BLACK;
                    fg = C_BLACK;
                    symbol = ' ';
                }
            }

            if(currentPlayer && currentPlayer->hasEffect(effLSD)){
                if(bg != C_BLACK){
                    bg = (((int)((currentWorld->worldTime+100)*2.7))+p.y-p.x) % 16;
                }else if(fg != C_BLACK && symbol != ' '){
                    fg = (((int)((currentWorld->worldTime+100)*2.3))+p.x-p.y) % 16;
                }
            }
            if(currentPlayer && fadeIn < fadeInMax){
                if(distanceSquared(p, currentPlayer->pos) > fadeIn*fadeIn){
                    bg = C_BLACK;
                    fg = C_BLACK;
                    symbol = ' ';
                }
            }
        } else {
            fg = C_LIGHT_MAGENTA;
            symbol = '?';
        }

        setColor(fg, bg);
        addch(symbol);
    }

    void MenuGame::arrowMove(Point2 p) {
        if (controlMode == modeSelectPosition) {
            targetPosition += p;
        }/* else if (controlMode == modeSelectDirection) {

            Item* i = itemToBeUsed!=nullptr? itemToBeUsed : currentPlayer->getActiveItemWeapon();

            timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos + p, false, i, false);

            changeMode(modePlayerControl);

        }*/ else if (controlMode == modeAdjustBorder) {
            borderSize -= p;
            setGameAreaSize();
        } else if (controlMode == modePlayerControl && currentPlayer != nullptr && currentLevel != nullptr) {

            timePassed += currentPlayer->moveRelative(p, currentLevel);

        } else if (controlMode == modeSelectEntity) {
            Point2 temp = Point2Zero;
            if (targetEntity) {
                temp = targetEntity->pos;
            } else {
                temp = currentPlayer->pos;
            }
            temp += p;
            vector<Entity*> list = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, {
                currentPlayer, targetEntity
            }, temp, p, true);
            if (list.size() > 0) {
                targetEntity = list[0];
                targetPosition = targetEntity->pos;
            } else if (!targetEntity) {
                changeMode(modePlayerControl);
            } else {
                targetPosition = targetEntity->pos;
            }
        }
    }

    void MenuGame::handleInput(int in) {
        if (in == ERR) {

        } else if (in == KEY_RESIZE) {
            setGameAreaSize();

        } else if (in == KEY_ESCAPE) {
            if (controlMode != modePlayerControl) {
                changeMode(modePlayerControl);
            } else {
                if (!currentPlayer || currentPlayer->dead) {
                    WorldLoader::deleteWorld(currentWorld->name);

                    delete currentWorld;
                    currentWorld = nullptr;

                    closeAllMenus();
                    return;
                } else {
                    openMenu(new MenuEscape(this));
                }
            }

        } else if (in == Key::moveUp) {
            arrowMove(Point2Up);

        } else if (in == Key::moveDown) {
            arrowMove(Point2Down);

        } else if (in == Key::moveLeft) {
            arrowMove(Point2Left);

        } else if (in == Key::moveRight) {
            arrowMove(Point2Right);

        } else if (in == Key::standStill) {
            arrowMove(Point2Zero);

        } else if (in == Key::inventory) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuInv(currentWorld, useItem));
            }

        } else if (in == Key::statsMenu) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuStats(currentPlayer, currentWorld));
            }

        } else if (in == Key::adjustConsole) {
            if (controlMode == modeAdjustBorder) {
                changeMode(modePlayerControl);
            } else {
                changeMode(modeAdjustBorder);
            }

        }

        if (currentWorld != nullptr && currentPlayer != nullptr && currentLevel != nullptr) {
            /*if (controlMode == modeSelectEntity && (((false) && in == 'f') || in == Key::interact)) {

                if (targetEntity) {
                    if (currentLevel->canSee(currentPlayer->pos, targetEntity->pos, currentPlayer->viewDistance, false)) {
                        timePassed += currentPlayer->interactWithEntity(currentLevel, targetEntity, targetEntity->pos, itemToBeUsed);
                    }
                }

                changeMode(modePlayerControl);

            } else if ((false) && in == 'f') {
                ItemRanged* ranged = dynamic_cast<ItemRanged*> (currentPlayer->getActiveItemWeapon());
                if (ranged && controlMode != modeSelectEntity) {
                    itemToBeUsedRange = ranged->range;
                    itemToBeUsed = ranged;
                    changeMode(modeSelectEntity);
                    if (!targetEntity) {
                        arrowMove(Point2Zero);
                    } else {
                        if (currentLevel->canSee(currentPlayer->pos, targetEntity->pos, currentPlayer->viewDistance, false)) {
                            targetPosition = targetEntity->pos;
                        } else {
                            targetPosition = Point2Zero;
                        }
                    }
                }

            } else*/

            if (in == Key::interact || in == Key::secondaryAttack) {
                ItemWeapon* wep = (in == Key::interact)?currentPlayer->getActiveItemWeapon():currentPlayer->getSecondaryItemWeapon();
                /*if (controlMode == modeSelectDirection) {

                    ItemWeapon* w = dynamic_cast<ItemWeapon*>(itemToBeUsed);

                    timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, itemToBeUsed, w?w->baseDamage==0:true);

                    itemToBeUsed = nullptr;
                    changeMode(modePlayerControl);
                } else*/ if (controlMode == modeSelectPosition) {

                    ItemUtilitySpell* us = dynamic_cast<ItemUtilitySpell*>(itemToBeUsed);
                    ItemWeapon* w = dynamic_cast<ItemWeapon*>(itemToBeUsed);
                    ItemRanged* ranged = dynamic_cast<ItemRanged*> (itemToBeUsed);
                    
                    double range = ranged ? ranged->range : 1.5; // just over sqrt(2) to satisfy float comparison
                    
                    if(us || currentLevel->canSee(currentPlayer->pos, targetPosition, range)){
                        timePassed += currentPlayer->interact(currentLevel, targetPosition, false, itemToBeUsed, w?w->baseDamage==0:true);
                    } else {
                        if(distanceSquared(currentPlayer->pos, targetPosition) > range){
                            consolef("&%cOut of range!", Ui::cc(C_LIGHT_RED));
                        }else{
                            consolef("&%cNo line of sight!", Ui::cc(C_LIGHT_RED));
                        }
                    }

                    if(!(us && us->continuousUse)){
                        changeMode(modePlayerControl);
                    }

                } else {
                    
                    ItemRanged* ranged = dynamic_cast<ItemRanged*> (wep);
                    changeMode(modeSelectPosition);
                    if (!ranged || !currentLevel->canSee(currentPlayer->pos, targetPosition, ranged->range)) {
                        targetPosition = currentPlayer->pos;
                    }
                    itemToBeUsed = wep;
                    
                    /*ItemRanged* ranged = dynamic_cast<ItemRanged*> (wep);
                    if (ranged) {
                        changeMode(modeSelectPosition);
                        if (!currentLevel->canSee(currentPlayer->pos, targetPosition, ranged->range)) {
                            targetPosition = currentPlayer->pos;
                        }
                        itemToBeUsed = ranged;
                    } else {
                        changeMode(modeSelectDirection);
                        itemToBeUsed = wep;
                    }*/
                }
            } else if(in == Key::instantInteract){
                timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, currentPlayer->getActiveItemWeapon(), false);
            }else if(in == Key::waitUntilHealed) {
                if(controlMode == modePlayerControl){
                    if(currentPlayer && (currentPlayer->getHp() < currentPlayer->getMaxHp() || currentPlayer->getMp() < currentPlayer->getMaxMp())){
                        unsigned char b = 1;
                        timeout(fastestTimeout);
                        while ((currentPlayer->getHp() < currentPlayer->getMaxHp() || currentPlayer->getMp() < currentPlayer->getMaxMp()) && b) {
                            vector<Entity*> nearest = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, false);
                            for (Entity* e : nearest) {
                                if (e->isHostile()) {
                                    b = 0;
                                    console("A hostile is near!");
                                    break;
                                }
                            }
                            for(Effect e : currentPlayer->effects){
                                if(e.eId == effDamage && e.power < 0){
                                    b = 0;
                                    console("You have an effect that is lowering your health!");
                                    break;
                                }
                            }
                            if (b == 1) {
                                b = 2;
                                console("Waiting until fully healed...");
                            }
                            timePassed = 1;
                            tick++;
                            MenuGame::update();
                            if(!currentPlayer){
                                timeout(defaultTimeout);
                                return;
                            }
                            if (getchSafe() != ERR) {
                                console("Wait canceled.");
                                break;
                            }
                            refresh();
                        }
                        timeout(defaultTimeout);
                        if(currentPlayer && currentPlayer->getHp() == currentPlayer->getMaxHp() && currentPlayer->getMp() == currentPlayer->getMaxMp()){
                            console("Fully healed.");
                        }
                    }else{
                        console("Already at full health and mana.");
                    }
                }

            } else if (in == Key::wait) {
                if(controlMode == modePlayerControl){
                    timeout(fastTimeout);
                    console("Waiting indefinitely. Press any key to stop waiting.");
                    bool c = true;
                    while (c) {
                        vector<Entity*> nearest = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, false);
                        for (Entity* e : nearest) {
                            if (e->isHostile()) {
                                console("A hostile is near!");
                                c = false;
                                break;
                            }
                        }
                        for(Effect e : currentPlayer->effects){
                            if(e.eId == effDamage && e.power < 0){
                                console("You have an effect that is lowering your health!");
                                c = false;
                                break;
                            }
                        }
                        timePassed = 1;
                        tick++;
                        MenuGame::update();
                        if(!currentPlayer){
                            timeout(defaultTimeout);
                            return;
                        }
                        if (getchSafe() != ERR) {
                            console("Stopped waiting.");
                            c = false;
                        }
                        refresh();
                    }
                    timeout(defaultTimeout);
                }

            } else if (in == Key::walk) {
                if(controlMode == modePlayerControl){
                    changeMode(modeSelectPosition);
                    if (!currentLevel->canSee(currentPlayer->pos, targetPosition, currentPlayer->viewDistance)) {
                        targetPosition = currentPlayer->pos;
                    }
                } else if(controlMode == modeSelectPosition){
                    Point2 t = targetPosition;
                    changeMode(modePlayerControl);
                    vector<Point2> path = currentLevel->getPathTo(t, currentPlayer->pos, tileFlagPathable, currentPlayer->getSolidity() | tileFlagHarmful, false, true);
                    path.push_back(t);
                    bool pathExplored = true;
                    if(!Settings::seeEverything){
                        for(Point2 p : path){
                            if(!currentLevel->getExplored(p)){
                                pathExplored = false;
                                break;
                            }
                        }
                    }
                    if(path.size() > 0 && pathExplored){
                        timeout(fastTimeout);
                        console("Walking to target position...");
                        bool c = true;
                        for(Point2 p : path) {
                            /*vector<Entity*> nearest = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, false);
                            for (Entity* e : nearest) {
                                if (e->isHostile()) {
                                    console("A hostile is near!");
                                    c = false;
                                    break;
                                }
                            }*/
                            vector<Entity*> nearest = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, false);
                            for (Entity* e : nearest) {
                                if (e->isHostile()) {
                                    console("A hostile is near!");
                                    c = false;
                                    break;
                                }
                            }
                            for(Effect e : currentPlayer->effects){
                                if(e.eId == effDamage && e.power > 0){
                                    console("You have an effect that is lowering your health!");
                                    c = false;
                                    break;
                                }
                            }
                            if(!c){
                                break;
                            }

                            Point2 d = p-currentPlayer->pos;
                            if(distanceSquared(d, Point2Zero) <= 1){
                                timePassed = currentPlayer->tryToMoveRelative(d, currentLevel);
                            }else{
                                console("Next Path fragment is too far.");
                                break;
                            }
                            tick++;
                            MenuGame::update();
                            if(!currentPlayer){
                                timeout(defaultTimeout);
                                return;
                            }
                            if (getchSafe() != ERR) {
                                console("Stopped walking.");
                                break;
                            }
                            refresh();
                        }
                        timeout(defaultTimeout);
                        Point2 d = targetPosition-currentPlayer->pos;
                        if(currentPlayer->pos == t){
                            console("You have arrived at your destination.");
                        }else if(distanceSquared(d, Point2Zero) <= 1){
                            console("Your destination is next to you.");
                        }
                    }else{
                        console("No expolored path found.");
                    }
                }

            }  else if (in == Key::explore) {
                if(controlMode == modePlayerControl){
                    timeout(fastTimeout);
                    console("Exploring... Press any key to stop.");
                    bool c = true;
                    int exploreTicks = -1;
                    while (c) {
                        exploreTicks++;
                        vector<Entity*> nearest = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, false);
                        for (Entity* e : nearest) {
                            if (e->isHostile()) {
                                console("A hostile is near!");
                                c = false;
                                break;
                            }
                        }
                        for(Effect e : currentPlayer->effects){
                            if(e.eId == effDamage && e.power > 0){
                                console("You have an effect that is lowering your health!");
                                c = false;
                                break;
                            }
                        }

                        vector<Point2> possibilityList;

                        Utility::executeGrid(currentPlayer->pos-currentPlayer->viewDistance*2, currentPlayer->pos+currentPlayer->viewDistance*2, [this, &possibilityList](int x, int y){
                            if(currentLevel->inRange(x, y) && !currentLevel->getExplored(x, y)){
                                int nearExplored = false;
                                Utility::execute4Around(x, y, [this, &nearExplored](int x, int y){
                                    if(currentLevel->getExplored(x, y) && currentLevel->tileAt(x, y)->hasAllOfFlags(tileFlagPathable)){
                                        nearExplored = true;
                                        return;
                                    }
                                });
                                if(nearExplored){
                                    possibilityList.push_back(Point2(x, y));
                                }
                            }
                        });
                        
                        Point2 next = Point2Neg1;
                        
                        vector<Point2> nexts;
                        
                        size_t count = 1000000;

                        vector<Point2> path;

                        for(Point2 p : possibilityList){
                            int actualLength;
                            path = currentLevel->getPathTo(p, currentPlayer->pos, tileFlagPathable, currentPlayer->solidity | tileFlagHarmful, true, true, tileFlagAll, 1, &actualLength);
                            if(actualLength > 0){
                                bool nn = true;
                                for(Point2 n : nexts){
                                    if(n == path[0]){
                                        nn = false;
                                        break;
                                    }
                                }
                                if(nn){
                                    nexts.push_back(path[0]);
                                }
                                if(next == Point2Neg1){
                                    next = path[0];
                                    count = actualLength;
                                }else{
                                    if(actualLength < count){
                                        next = path[0];
                                        count = actualLength;
                                    }
                                }
                            }
                        }
                        
                        if(next == currentPlayer->lastPos && exploreTicks > 0){
                            console("Whelp, that's the end of the line.");
                            c = false;
                            break;
                        }

                        if(next != Point2Neg1){
                            int pc = 0;
                            Utility::execute4Around(next.x, next.y, [this, &c, &pc](int x, int y){
                                if(c && currentLevel->tileAt(x, y)->hasAllOfFlags(tileFlagDoor | tileFlagPathable)){
                                    console("Oh a door");
                                    c = false;
                                }
                                if(currentLevel->tileAt(x, y)->hasAllOfFlags(tileFlagPathable)){
                                    pc++;
                                }
                            });
                            if(pc > 2 && nexts.size() > 1){
                                console("Where to go now?");
                                c = false;
                            }
                            timePassed = currentPlayer->moveAbsolute(next, currentLevel, false);
                            if(timePassed == 0){
                                console("Your path is blocked!");
                                c = false;
                            }
                        }else{
                            console("No unexplored area found nearby.");
                            c = false;
                        }
                        
                        MenuGame::update();
                        if(!currentPlayer){
                            timeout(defaultTimeout);
                            return;
                        }
                        if (getchSafe() != ERR) {
                            console("Stopped exploring.");
                            c = false;
                        }
                        refresh();
                    }
                    timeout(defaultTimeout);
                }

            } else if (in == Key::inspect) {
                if(controlMode == modePlayerControl){
                    changeMode(modeSelectPosition);
                    if (!currentLevel->canSee(currentPlayer->pos, targetPosition, currentPlayer->viewDistance)) {
                        targetPosition = currentPlayer->pos;
                    }
                } else if(controlMode == modeSelectPosition){
                    changeMode(modePlayerControl);

                    console("Inspecting...");

                    if(currentLevel->getExplored(targetPosition)){

                        console(Utility::capitalize(currentLevel->tileAt(targetPosition)->getName()));

                        if(currentLevel->canSee(currentPlayer->pos, targetPosition, currentPlayer->viewDistance)){
                            for(Entity* e : currentLevel->entityList){
                                if(!e->removed && e->pos == targetPosition){
                                    if(Settings::debugMode){
                                        console("Entity("+to_string(e->getEntityTypeId())+"): "+e->getName());
                                    }

                                    EntityAlive* a = dynamic_cast<EntityAlive*>(e);
                                    if(a){
                                        consolef("%s  HP:[%.0f/%.0f]", a->getName().c_str(), a->getHp(), a->getMaxHp());
                                        EntityAi* eai = dynamic_cast<EntityAi*>(a);
                                        if(eai){
                                            ItemWeapon* w = eai->getActiveItemWeapon();
                                            if(w){
                                                consolef("Weapon, %s (%s) %.2f d/t (%.2f per %.2ft) ", w->getName(false).c_str(), damageTypeName(w->damageType).c_str(), w->baseDamage / w->useDelay, w->baseDamage, w->useDelay);
                                            }
                                        }
                                        for(Weakness w : a->weaknesses){
                                            if(w.multiplier > 1){
                                                consolef("%d%% Weakness to %s", (int)((w.multiplier-1)*100), damageTypeName(w.damageType).c_str());
                                            }else if(w.multiplier < 1){
                                                consolef("%d%% Resistance to %s", (int)((1-w.multiplier)*100), damageTypeName(w.damageType).c_str());
                                            }
                                        }
                                    }

                                }
                            }
                            for(TileEntity* t : currentLevel->tileEntityList){
                                if(t->pos == targetPosition){
                                    if(Settings::debugMode){
                                        console("Tile Entity("+to_string(t->getTileEntityTypeId())+"): "+t->debugString());
                                    }
                                    switch (t->getTileEntityTypeId()) {
                                        case TILE_ENTITY_TYPE_STAIR:{
                                            TEStair* tes = dynamic_cast<TEStair*>(t);
                                            consolef("Staircase going &%c%s", cc(C_LIGHT_CYAN), tes->up?"Up":"Down");
                                            break;
                                        }

                                        default:
                                            break;
                                    }
                                }
                            }
                        }else{
                            console("Area not in view");
                        }
                    }else{
                        console("Area Unexplored.");
                    }
                }
            }else {
                int fav = -1;
                for(int i=0;i<10;i++){
                    if(in == Key::favs[i]){
                        fav = i;
                        break;
                    }
                }

                if(fav >= 0){
                    Item* it = currentPlayer->getFav(fav);
                    
                    if(it){
                        ItemEquipable* eqip = dynamic_cast<ItemEquipable*>(it);
                        if(eqip){
                            currentPlayer->equipItem(eqip);
                        }else{
                            UseType use = it->getUseType();
                            if (use == useInstant) {
                                timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, it, true);
                            } else if(use == useInWorld){
                                if(controlMode == modePlayerControl){
                                    itemToBeUsed = it;
                                    if (!currentLevel->canSee(currentPlayer->pos, targetPosition, currentPlayer->viewDistance)) {
                                        targetPosition = currentPlayer->pos;
                                    }
                                    changeMode(modeSelectPosition);
                                }else if(controlMode == modeSelectPosition){
                                    ItemWeapon* wep = dynamic_cast<ItemWeapon*>(it);
                                    timePassed += currentPlayer->interact(currentLevel, targetPosition, false, itemToBeUsed, wep?wep->baseDamage==0:true);
                                    changeMode(modePlayerControl);
                                }
                            } else if(use == useInInventory){
                                int i=0;
                                for(Item* item : currentPlayer->inventory){
                                    if(item == it){
                                        break;
                                    }
                                    i++;
                                }
                                MenuUseInInv* m = new MenuUseInInv(currentPlayer, currentWorld, i);
                                openMenu(m);
                            }
                        }
                    }
                }

                if(Settings::cheatKeysEnabled){
                    if (in == 'r') {
                        Point2 p = currentLevel->findRandomWithoutFlag(currentPlayer->getSolidity());
                        timePassed += currentPlayer->moveAbsolute(p, currentLevel, false);

                    } else if (in == 'R') {
                        Point2 p = currentLevel->stairDownPos;
                        consolef("Stair Down: %s", p.toString().c_str());
                        //timePassed += currentPlayer->moveAbsolute(p, currentLevel);
                        currentPlayer->pos = p;

                    } else if (in == '[') {
                        currentPlayer->hurt(currentLevel, damDebug, 1);

                    } else if (in == ']') {
                        currentPlayer->heal(1);

                    } else if (in == '{') {
                        currentPlayer->hurt(currentLevel, damDebug, 10);

                    } else if (in == '}') {
                        currentPlayer->heal(10);

                    } else if (in == 'D') {
                        openMenu(new MenuDebug());

                    } else if (in == 'P') {
                        currentPlayer->changeSpecial(specialPoolHpMp, !currentPlayer->specials[specialPoolHpMp]);
                        
                    } else if (in == 'O') {
                        rumble+=50;
                    }
                }
            }
        }
    }

    void MenuGame::update() {

        if (currentWorld == nullptr) {
            return;
        }

        render(currentWorld->worldTime);

        if (timePassed > 0) {
            currentWorld->worldTime += timePassed;
            timePassed = 0;
            currentLevel->update(currentWorld->worldTime - currentWorld->worldLastTime, currentWorld->worldTime, viewPos);
            //render(currentWorld->worldTime);

            currentWorld->worldLastTime = currentWorld->worldTime;
            timeSinceTimePassed = 0;
        }else{
            timeSinceTimePassed++;
        }

        if(currentPlayer){

            if(Settings::autoSave && timeSinceTimePassed == Settings::autoSaveDelay){
                WorldLoader::save(currentWorld);
            }

            if(currentPlayer->leveledUp){
                vector<string> message = Arts::getArt(Arts::artLevelUp)->printToVector();
                message.push_back("");
                message.push_back("");
                message.push_back("");
                openMenu(new MenuMessage(message));
                currentPlayer->leveledUp = false;
            }
        }else{
            timePassed += 1;
        }

    }

    void MenuGame::render(double displayTime) {
        viewUpdate();
        move(0, 0);
        clrtobot();
        
        printConsole(gameArea.y + 1);

        if(fadeIn < fadeInMax){
            fadeIn++;
        }

        if(rumble > 0){
            rumbleX = rand()%2;
            rumbleY = rand()%2;
            rumble--;
        }

        for (int j = 0; j < gameArea.y; j++) {
            move(j, 0);
            for (int i = 0; i < gameArea.x; i++) {
                drawTileAt(Point2(viewPos.x + i, viewPos.y + j));
            }
        }
        setColor((controlMode == modeAdjustBorder) ? C_BLACK : C_WHITE,
                (controlMode == modeAdjustBorder) ? C_LIGHT_YELLOW : C_BLACK);

        for (int j = 0; j < gameArea.y; j++) {
            mvaddch(j, gameArea.x, '|');
        }
        for (int i = 0; i < terminalSize.x; i++) {
            mvaddch(gameArea.y, i, '-');
        }
        mvaddch(gameArea.y, gameArea.x, '+');

        Ui::setColor(C_WHITE);
        Point2 p = viewPos;

        int a = 0;

        if (currentPlayer) {

            p = currentPlayer->pos;
            mvprintw(a, gameArea.x + 1, "%s", currentPlayer->getName().c_str());
            
            const int hp = roundToInt(currentPlayer->getHp());
            const int maxHp = roundToInt(currentPlayer->getMaxHp());
            
            if(currentPlayer->specials[specialPoolHpMp]){
                
                a++;
                
                move(a, gameArea.x + 1);
                clrtoeol();
                
                a += printMultiLineString(a, gameArea.x + 1, formatString("HP+MP: %d/%d", hp, maxHp));
                Ui::setColor((hp < (maxHp / 3 * 2)) ? ((hp < (maxHp / 3)) ? C_LIGHT_RED : C_LIGHT_YELLOW) : C_LIGHT_BLUE);
                printw(" %s", Utility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 1)).c_str());
                Ui::setColor(C_WHITE);
                
                move(a, gameArea.x + 1);
                clrtoeol();
                
            }else{

                const int mp = roundToInt(currentPlayer->getMp());
                const int maxMp = roundToInt(currentPlayer->getMaxMp());

                a++;

                move(a, gameArea.x + 1);
                clrtoeol();

                a += printMultiLineString(a, gameArea.x + 1, formatString("HP: %d/%d", hp, maxHp));
                Ui::setColor((hp < (maxHp / 3 * 2)) ? ((hp < (maxHp / 3)) ? C_LIGHT_RED : C_LIGHT_YELLOW) : C_LIGHT_GREEN);
                printw(" %s", Utility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 1)).c_str());
                Ui::setColor(C_WHITE);

                move(a, gameArea.x + 1);
                clrtoeol();

                if(maxMp > 0){
                    a += printMultiLineString(a, gameArea.x + 1, formatString("MP: %d/%d", mp, maxMp));
                    Ui::setColor(C_LIGHT_BLUE);
                    printw(" %s", Utility::makeBar(mp, maxMp, (terminalSize.x - getcurx(stdscr) - 1)).c_str());
                    Ui::setColor(C_WHITE);
                }
            }
            a += printMultiLineString(a, gameArea.x + 1, formatString("Lvl %d", currentPlayer->level));
            printw(" %s", Utility::makeBar(currentPlayer->xp, currentPlayer->nextLevelXp, (terminalSize.x - getcurx(stdscr) - 1)).c_str());

            for (size_t i = 0; i < currentPlayer->effects.size(); i++) {
                Effect eff = currentPlayer->effects[i];
                setColor(effectColor(eff.eId, eff.meta));
                if(eff.eId == effMultRecivedDamage || eff.eId == effMultAttack){
                    a += printMultiLineString(a, gameArea.x + 1, formatString("%s %d%% for %.2f%s", effectName(eff).c_str(), (int)(eff.power*100), eff.timeLeft, SYMBOL_TIME));
                }else{
                    if(eff.power != 0){
                    	a += printMultiLineString(a, gameArea.x + 1, formatString("%s %.2f for %.2f%s", effectName(eff).c_str(), eff.power, eff.timeLeft, SYMBOL_TIME));
                    }else{
                        a += printMultiLineString(a, gameArea.x + 1, formatString("%s for %.2f%s", effectName(eff).c_str(), eff.timeLeft, SYMBOL_TIME));
                    }
                }
            }

            Ui::setColor(C_WHITE);

            //mvprintw(a++, gameArea.x + 1, "Tick [%c]: %s", spinyIcon->getChar(tick, Point2Neg1, currentLevel), Utility::toString((int)tick, 62).c_str());
            
            string s = formatString("%d.%d", ((int)displayTime)%60, (int)(displayTime/100.0f));
            if(displayTime >= 60){
                s = formatString("%d:", (((int)displayTime)/60)%60) + s;
                if(displayTime >= 60*60){
                    s = formatString("%d:", (((int)displayTime)/60/60)%24) + s;
                    if(displayTime >= 60*60*24){
                        s = formatString("%d:", ((int)displayTime)/60/60/24) + s;
                    }
                }
            }
            printMultiLineString(a++, gameArea.x + 1, s);

            //printMultiLineString(a++, gameArea.x + 1, formatString("%d:%02d:%02d:%02d.%02d", (((int)displayTime)/60/60/24), (((int)displayTime)/60/60)%(24), (((int)displayTime)/60)%60, ((int)displayTime)%60, (int)(displayTime/100.0f)));
            //mvprintw(a++, gameArea.x + 1, "Time: %s", Utility::intToRomanNumerals((int) displayTime).c_str());

            if(Settings::debugMode){
            	mvprintw(a++, gameArea.x + 1, "Time: %.2f", displayTime);
                mvprintw(a++, gameArea.x + 1, "Tick: %d", tick);
            }
            mvprintw(a++, gameArea.x + 1, "%s", currentLevel->getName().c_str());

            if(currentPlayer->abilityPoints > 0){
                a++;
                printMultiLineString(a++, gameArea.x+1, "Unspent skill points");
                printMultiLineString(a++, gameArea.x+1, formatString("Press [%s]", keyDisplayName(Key::statsMenu).c_str()).c_str());
            }

            a++;

            if(Settings::autoSave && timeSinceTimePassed > Settings::autoSaveDelay && timeSinceTimePassed < Settings::autoSaveDelay+10){
                printMultiLineString(a++, gameArea.x+1, "Auto saving...");
                a++;
            }

            //move(a, gameArea.x + 1);
            //clrtoeol();

            nearestEntities = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, true);

            //consolef("Nearest Entity Count: %d", nearestEntities.size());

            for (size_t i = 0; i < nearestEntities.size() && a < terminalSize.y; i++) {
                Entity* nearestEntity = nearestEntities[i];
                if (nearestEntity && !nearestEntity->removed && !dynamic_cast<EntityMultiSub*>(nearestEntity)) {
                    Ui::setColor(C_WHITE);

                    a += printMultiLineColoredString(a, gameArea.x + 1, formatString("[&%c%c&%c] %s ", cc(nearestEntity->getFgColor(tick, p, currentLevel), nearestEntity->getBgColor(tick, p, currentLevel)), nearestEntity->getChar(tick, nearestEntity->pos, currentLevel), cc(C_WHITE), nearestEntity->getName().c_str()));


                    EntityAlive* alive = dynamic_cast<EntityAlive*> (nearestEntity);
                    if (alive) {
                        const int hp = roundToInt(alive->getHp());
                        const int maxHp = roundToInt(alive->getMaxHp());

                        a += printMultiLineColoredString(a, gameArea.x + 1, formatString("^HP: &%c%s&%c ", cc((hp < (maxHp / 3 * 2)) ? ((hp < (maxHp / 3)) ? C_LIGHT_RED : C_LIGHT_YELLOW) : C_LIGHT_GREEN), Utility::makeBar(hp, maxHp, (terminalSize.x - (gameArea.x + 1) - 5)).c_str(), cc(C_WHITE)));

                        for (size_t i = 0; i < alive->effects.size(); i++) {
                            Effect eff = alive->effects[i];
                            setColor(effectColor(eff.eId, eff.meta));
                            if(eff.eId == effMultRecivedDamage || eff.eId == effMultAttack){
                                a += printMultiLineString(a, gameArea.x + 1, formatString("%s %d%%", effectName(eff).c_str(), (int)(eff.power*100)));
                            }else{
                                if(eff.power != 0){
                                    a += printMultiLineString(a, gameArea.x + 1, formatString("%s %.2f", effectName(eff).c_str(), eff.power));
                                }else{
                                    a += printMultiLineString(a, gameArea.x + 1, formatString("%s", effectName(eff).c_str()));
                                }
                            }
                        }
                    }
                    //a++;



                }

            }
            
            char f[10];
            int fw[10];
            
            int ic = 0;

            for(int i=0;i<10;i++){
                if(currentPlayer->getFav(i)){
                    f[i] = 'I';
                    ic ++;
                }else{
                    f[i] = 'E';
                }
            }
            
            for(int i=0;i<10;i++){
                if(f[i] == 'I'){
                    Item* it = currentPlayer->getFav(i);
                    
                    int extra = 0;
                    EquipSlot slot = currentPlayer->getSlot(it);
                    if(slot != slotNone){ extra = 2; }
                    fw[i] = min((terminalSize.x - borderSize.x) / ic, (int)it->getName(it->qty!=1).size()+1+extra);
                }else{
                    fw[i] = 0;
                }
            }

            char dashes[] = "----------------------------------------------------------------";
            char spaces[] = "                                                                ";
            
            int x = 0;
            
            Ui::setColor(C_WHITE);
            
            for(int i=0;i<10;i++){
                Item* it = currentPlayer->getFav(i);
                if(it){
                    string name = keyDisplayName(Key::favs[i]);
                    mvprintw(gameArea.y-2, x, "+%.*s%.*s%.*s+", (fw[i]-name.length())/2, dashes, name.length(), name.c_str(), (((fw[i]-name.length())%2==1)?1:0)+(fw[i]-name.length())/2-1, dashes);
                    mvprintw(gameArea.y-1, x, "|%.*s|", fw[i]-1, spaces);
                    string s = it->getName(it->qty!=1);
                    EquipSlot slot = currentPlayer->getSlot(it);
                    if(slot != slotNone){
                        s = formatString("%c %s", ItemEquipable::equipSlotAbr(slot), s.c_str());
                    }
                    s = s.substr(0, fw[i]-1);
                    mvprintw(gameArea.y-1, (int)((x+fw[i]*.5)-((int)s.size()*.5))+1, "%.*s", fw[i]-1, s.c_str());
                    mvprintw(gameArea.y, x, "+%.*s+", fw[i]-1, dashes);
                }
                x += fw[i];
            }
            
            

            /*for(int i=1;i<=10;i++){
                Item* it = currentPlayer->getEquiped(slotFav1+i-1);
                if(it){
                    //string s = it->getName(it->qty!=1)+"   ";
                    //string ss = s.substr((tick/5)%(s.size()-3), 3);
                    mvprintw(gameArea.y-2, (i-1)*4, "+-%d-+", i==10?0:i);
                	mvprintw(gameArea.y-1, (i-1)*4, "|%3s|", (it->getName(it->qty!=1).substr(0, 3)).c_str());
                	mvprintw(gameArea.y, (i-1)*4, "+---+");
                }
            }*/

        }

        if(!currentPlayer || currentPlayer->dead){
            //printCenterOffset(gameArea.y / 2, -(borderSize.x / 2), "G a m e   O v e r");
            //printCenterOffset(gameArea.y / 2 + 3, -(borderSize.x / 2), "Press Escape to return to the Main Menu.");

            Art* a = Arts::getArt(Arts::artGameOver);
            setColor(C_WHITE, C_BLACK);
            a->printCenter((gameArea.y / 2) - ((int)a->lines.size()/2), -(borderSize.x / 2));
            printCenterOffset(gameArea.y / 2 + 3 + ((int)a->lines.size()/2), -(borderSize.x / 2), "Press Escape to return to the Main Menu.");
        }

        if(currentLevel){
            //currentLevel->menuGame = this;

            if (Settings::debugMode) {
                Ui::setColor(C_LIGHT_GRAY);
                mvprintw(gameArea.y, 0, "%d, %d [%dx%d] e:%d te:%d p:%s %s(%d)", p.x, p.y, currentLevel->getSize().x, currentLevel->getSize().y, currentLevel->entityCount(), currentLevel->tileEntityList.size(), (currentPlayer == nullptr) ? "0" : "1", currentLevel->getName().c_str(), currentLevel->getDifficulty());
            }
        }




        //refresh();
    }

    void MenuGame::changeMode(int newMode) {
        if (newMode == controlMode) {
            return;
        }
        switch (controlMode) {
            //case modeSelectDirection:
            case modeSelectPosition:
            case modeSelectEntity:
                if(!targetPosition.inRange(viewPos, viewPos+gameArea)){
                    targetPosition = viewPos+(gameArea/2);
                }
                itemToBeUsed = nullptr;
                targetEntity = nullptr;
                
                break;

            default:
                break;
        }
        controlMode = newMode;
    }

}
