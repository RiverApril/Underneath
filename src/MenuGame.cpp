//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.hpp"
#include "MenuMain.hpp"
#include "MenuInv.hpp"
#include "MenuStats.hpp"
#include "MenuYesNo.hpp"
#include "MenuDebug.hpp"
#include "MenuMessage.hpp"
#include "EntityAi.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "Controls.hpp"
#include "ItemRanged.hpp"
#include "ItemUtilitySpell.hpp"
#include "Settings.hpp"

#define currentPlayer currentWorld->currentPlayer
#define currentLevel currentWorld->currentLevel


namespace Ui {

    MenuGame::MenuGame(string worldName, Abilities<int> playerAbilities) : Menu() {

        printConsoleByDefault = false;

        move(0, 0);
        clrtobot();
        //refresh();

        initSuccess = init(worldName, playerAbilities);

        viewPos = Point2Zero;
        viewMoveSpeed = Point2(2, 1);

        /*if(currentLevel != nullptr){
                currentLevel->setAndUnsetDisplayEntities();
        }*/
        if (initSuccess) {
            currentLevel->menuGame = this;
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

    bool MenuGame::init(string worldName, Abilities<int> playerAbilities) {

        if (worldName.size() == 0) {
            return false;
        }

        if (WorldLoader::exists(worldName)) {
            currentWorld = WorldLoader::load(nullptr, worldName);
            if (currentWorld == nullptr) {
                currentWorld = WorldLoader::create(worldName, playerAbilities);
            }
        } else {
            currentWorld = WorldLoader::create(worldName, playerAbilities);
        }
        if (currentWorld == nullptr || currentLevel == nullptr) {
            return false;
        }

        fadeIn = fadeInMin;

        return true;
    }

    bool MenuGame::openUi() {
        debug("Open MenuGame Ui");

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
            if (currentPlayer->inventory[*useItem]->instantUse()) {
                timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, currentPlayer->inventory[*useItem]);
                int selected = *useItem;
                *useItem = -1;
                MenuInv* m = new MenuInv(currentPlayer, currentWorld, useItem);
                m->selected = selected;
                openMenu(m);
            } else{
                itemToBeUsedRange = 1000;
                itemToBeUsed = currentPlayer->inventory[*useItem];
                targetPosition = currentPlayer->pos;
                changeMode(modeSelectPosition);
                *useItem = -1;
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
        if(currentWorld != nullptr){
            if(currentLevel != nullptr){
                if (currentPlayer != nullptr) {
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
        Ui::Color fg = C_LIGHT_RED;
        Ui::Color bg = C_BLACK;
        char symbol = ' ';
        int attr = 0;


        bool inView = false;
        if (currentLevel != nullptr) {
            if (currentPlayer != nullptr) {
                if (currentLevel->canSee(p, currentPlayer->pos, currentPlayer->viewDistance) || Settings::seeEverything) {
                    inView = true;
                }
                if(inView){
                    currentLevel->setExplored(p, true);
                }
                if (!currentLevel->getExplored(p)) {
                    symbol = ' ';
                } else {
                    Tile* tempTile = currentLevel->tileAt(p);

                    Icon* tempIcon = tempTile->getIcon(inView);

                    fg = tempIcon->getFgColor(tick, p, currentLevel);
                    bg = tempIcon->getBgColor(tick, p, currentLevel);
                    symbol = tempIcon->getChar(tick, p, currentLevel);

                    if (currentLevel->inRange(p)) {
                        Entity* e = nullptr;
                        int d = INT16_MIN;
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

                            if (inView) {
                                if (currentPlayer == e && controlMode == modeSelectDirection) {
                                    fg = e->getBgColor(tick, p, currentLevel);
                                    bg = e->getFgColor(tick, p, currentLevel);
                                } else {
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
                        attr = A_BLINK;
                    }
                } else if (controlMode == modeSelectPosition) {
                    if (p == targetPosition) {
                        bg = C_LIGHT_GREEN;
                        attr = A_BLINK;
                    }

                }

                if(currentPlayer->hasEffect(effMemory)){
                    if(!inView){
                        bg = C_BLACK;
                        fg = C_BLACK;
                        symbol = ' ';
                    }
                }

                if(currentPlayer->hasEffect(effLSD)){
                    if(bg != C_BLACK){
                        bg = (((int)((currentWorld->worldTime+100)*2.7))+p.y-p.x) % 16;
                    }else{
                    	fg = (((int)((currentWorld->worldTime+100)*2.3))+p.x-p.y) % 16;
                    }
                }
                if(fadeIn < fadeInMax){
                    if(distanceSquared(p, currentPlayer->pos) > fadeIn*fadeIn){
                        bg = C_BLACK;
                        fg = C_BLACK;
                        symbol = ' ';
                    }
                }
            }
        } else {
            fg = C_LIGHT_MAGENTA;
            symbol = '?';
        }

        setColor(fg, bg, attr);
        addch(symbol);
    }

    void MenuGame::arrowMove(Point2 p) {
        if (controlMode == modeSelectPosition) {
            targetPosition += p;
        } else if (controlMode == modeSelectDirection) {

            Item* i = itemToBeUsed!=nullptr? itemToBeUsed : currentPlayer->getActiveItemWeapon();

            timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos + p, false, i);

            changeMode(modeEntityPlayerControl);

        } else if (controlMode == modeAdjustBorder) {
            borderSize -= p;
            setGameAreaSize();
        } else if (controlMode == modeEntityPlayerControl && currentPlayer != nullptr && currentLevel != nullptr) {

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
                changeMode(modeEntityPlayerControl);
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
            if (controlMode != modeEntityPlayerControl) {
                changeMode(modeEntityPlayerControl);
            } else {
                if (currentPlayer == nullptr) {
                    WorldLoader::deleteWorld(currentWorld->name);

                    delete currentWorld;
                    currentWorld = nullptr;

                    closeAllMenus();
                    return;
                } else {
                    openMenu(new MenuYesNo("Do you want to save '" + currentWorld->name + "' ?", saveAnswer, true));
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

        } else if (in == Key::inventory) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuInv(currentPlayer, currentWorld, useItem));
            }

        } else if (in == Key::statsMenu) {
            if (currentPlayer != nullptr) {
                openMenu(new MenuStats(currentPlayer, currentWorld));
            }

        } else if (in == Key::adjustConsole) {
            if (controlMode == modeAdjustBorder) {
                changeMode(modeEntityPlayerControl);
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

                changeMode(modeEntityPlayerControl);

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
                if (controlMode == modeSelectDirection) {

                    timePassed += currentPlayer->interact(currentLevel, currentPlayer->pos, false, itemToBeUsed);

                    itemToBeUsed = nullptr;
                    changeMode(modeEntityPlayerControl);
                } else if (controlMode == modeSelectPosition) {

                    ItemUtilitySpell* us = dynamic_cast<ItemUtilitySpell*>(itemToBeUsed);

                    timePassed += currentPlayer->interact(currentLevel, targetPosition, false, itemToBeUsed);

                    if(!(us && us->continuousUse)){
                        changeMode(modeEntityPlayerControl);
                    }

                } else {
                    ItemRanged* ranged = dynamic_cast<ItemRanged*> (wep);
                    if (ranged) {
                        changeMode(modeSelectPosition);
                        if (!currentLevel->canSee(currentPlayer->pos, targetPosition, ranged->range)) {
                            targetPosition = currentPlayer->pos;
                        }
                        itemToBeUsedRange = ranged->range;
                        itemToBeUsed = ranged;
                    } else {
                        changeMode(modeSelectDirection);
                        itemToBeUsed = wep;
                    }
                }
            } else if(in == Key::waitUntilHealed) {
                if(controlMode == modeEntityPlayerControl){
                    if(currentPlayer->getHp() < currentPlayer->getMaxHp() || currentPlayer->getMp() < currentPlayer->getMaxMp()){
                        unsigned char b = 1;
                        timeout(20);
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
                            MenuGame::update();
                            if (getch() != ERR) {
                                console("Wait canceled.");
                                break;
                            }
                        }
                        timeout(defaultTimeout);
                        if(currentPlayer->getHp() == currentPlayer->getMaxHp() && currentPlayer->getMp() == currentPlayer->getMaxMp()){
                            console("Fully healed.");
                        }
                    }else{
                        console("Already at full health and mana.");
                    }
                }

            } else if (in == Key::wait) {
                if(controlMode == modeEntityPlayerControl){
                    timeout(20);
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
                        MenuGame::update();
                        if (getch() != ERR) {
                            console("Stopped waiting.");
                            c = false;
                        }
                    }
                    timeout(defaultTimeout);
                }

            }  else if (in == Key::walk) {
                if(controlMode == modeEntityPlayerControl){
                    changeMode(modeSelectPosition);
                    itemToBeUsedRange = 1000;
                    targetPosition = currentPlayer->pos;
                } else if(controlMode == modeSelectPosition){
                    Point2 t = targetPosition;
                    changeMode(modeEntityPlayerControl);
                    vector<Point2> path = currentLevel->getPathTo(t, currentPlayer->pos, tileFlagPathable, tileFlagSolid);
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
                        timeout(20);
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
                            Point2 d = p-currentPlayer->pos;
                            if(abs(d.x) <= 1 && abs(d.y) <= 1){
                            	timePassed = currentPlayer->tryToMoveRelative(d, currentLevel);
                            }else{
                                console("Next Path fragment is too far.");
                                c = false;
                                break;
                            }
                            MenuGame::update();
                            if (getch() != ERR) {
                                console("Stopped walking.");
                                c = false;
                                break;
                            }
                        }
                        timeout(defaultTimeout);
                        Point2 d = targetPosition-currentPlayer->pos;
                        if(currentPlayer->pos == t){
                            console("You have arrived at your destination.");
                        }else if(abs(d.x) <= 1 && abs(d.y) <= 1){
                            console("Your destination is next to you.");
                        }
                    }else{
                        console("No expolored path found.");
                    }
                }

            } else if (in == Key::inspect) {
                if(controlMode == modeEntityPlayerControl){
                    changeMode(modeSelectPosition);
                    itemToBeUsedRange = 1000;
                    targetPosition = currentPlayer->pos;
                } else if(controlMode == modeSelectPosition){
                    changeMode(modeEntityPlayerControl);

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
                                        consolef("%s - HP:[%.0f/%.0f]", a->getName().c_str(), a->getHp(), a->getMaxHp());
                                        EntityAi* eai = dynamic_cast<EntityAi*>(a);
                                        if(eai){
                                            ItemWeapon* w = eai->getActiveItemWeapon();
                                            if(w){
                                                consolef("Weapon: %s  %.2f d/t (%.2f per %.2ft) ", damageTypeName(w->damageType).c_str(), w->baseDamage, w->useDelay, w->baseDamage / w->useDelay);
                                            }
                                        }
                                        for(Weakness w : a->weaknesses){
                                            if(w.multiplier > 1){
                                                consolef("%d%% Weakness to %s", (int)(1/w.multiplier*100), damageTypeName(w.damageType).c_str());
                                            }else{
                                                consolef("%d%% Resistance to %s", (int)((w.multiplier)*100), damageTypeName(w.damageType).c_str());
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
            } else if(Settings::cheatKeysEnabled){
                if (in == 'r') {
                    Point2 p = currentLevel->findRandomWithoutFlag(tileFlagSolid);
                    timePassed += currentPlayer->moveAbsalute(p, currentLevel, false);

                } else if (in == 'R') {
                    Point2 p = currentLevel->stairDownPos;
                    consolef("Stair Down: %s", p.toString().c_str());
                    //timePassed += currentPlayer->moveAbsalute(p, currentLevel);
                    currentPlayer->pos = p;

                } else if (in == '[') {
                    currentPlayer->hurt(damDebug, 1);

                } else if (in == ']') {
                    currentPlayer->heal(1);

                } else if (in == '{') {
                    currentPlayer->hurt(damDebug, 10);

                } else if (in == '}') {
                    currentPlayer->heal(10);

                } else if (in == 'D') {
                    openMenu(new MenuDebug(currentWorld));

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
            render(currentWorld->worldTime);

            currentWorld->worldLastTime = currentWorld->worldTime;
            timeSinceTimePassed = 0;
        }else{
            timeSinceTimePassed++;
        }

        if(currentPlayer){

            if(Settings::autoSave && timeSinceTimePassed == autoSaveTime){
                WorldLoader::save(currentWorld);
            }

            if(currentPlayer->leveledUp){
                openMenu(new MenuMessage({"Level up!", "", ""}));
                currentPlayer->leveledUp = false;
            }
        }

    }

    void MenuGame::render(double displayTime) {
        viewUpdate();
        move(0, 0);
        clrtobot();

        if(fadeIn < fadeInMax){
            fadeIn++;
        }

        for (int j = 0; j < gameArea.y; j++) {
            move(j, 0);
            for (int i = 0; i < gameArea.x; i++) {
                drawTileAt(Point2(viewPos.x + i, viewPos.y + j));
            }
        }
        setColor((controlMode == modeAdjustBorder) ? C_BLACK : C_WHITE,
                (controlMode == modeAdjustBorder) ? C_LIGHT_YELLOW : C_BLACK,
                (controlMode == modeAdjustBorder) ? A_BLINK : 0);

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

        if (currentPlayer != nullptr) {

            p = currentPlayer->pos;
            mvprintw(a, gameArea.x + 1, "%s", currentPlayer->getName().c_str());

            const int hp = Math::roundToInt(currentPlayer->getHp());
            const int maxHp = Math::roundToInt(currentPlayer->getMaxHp());
            const int mp = Math::roundToInt(currentPlayer->getMp());
            const int maxMp = Math::roundToInt(currentPlayer->getMaxMp());

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

            for (size_t i = 0; i < currentPlayer->effects.size(); i++) {
                Effect eff = currentPlayer->effects[i];
                setColor(effectColor(eff.eId, eff.meta));
                if(eff.eId == effBuffDefense){
                    a += printMultiLineString(a, gameArea.x + 1, formatString("%s %d%% for %.2ft", effectName(eff.eId, eff.meta).c_str(), (int)(eff.power*100), eff.timeLeft));
                }else if(eff.eId == effBuffAttack){
                    a += printMultiLineString(a, gameArea.x + 1, formatString("%s %c%d%% for %.2ft", effectName(eff.eId, eff.meta).c_str(), eff.power>0?'+':'-', (int)abs((eff.power*100)-100), eff.timeLeft));
                }else{
                    if(eff.power != 0){
                    	a += printMultiLineString(a, gameArea.x + 1, formatString("%s %.2f for %.2ft", effectName(eff.eId, eff.meta).c_str(), eff.power, eff.timeLeft));
                    }else{
                        a += printMultiLineString(a, gameArea.x + 1, formatString("%s for %.2ft", effectName(eff.eId, eff.meta).c_str(), eff.timeLeft));
                    }
                }
            }

            Ui::setColor(C_WHITE);

            //mvprintw(a++, gameArea.x + 1, "Tick [%c]: %s", spinyIcon->getChar(tick, Point2Neg1, currentLevel), Utility::toString((int)tick, 62).c_str());

            mvprintw(a++, gameArea.x + 1, "%d:%02d:%02d:%02d", (((int)displayTime)/60/60/24), (((int)displayTime)/60/60)%(24), (((int)displayTime)/60)%60, ((int)displayTime)%60);
            //mvprintw(a++, gameArea.x + 1, "Time: %s", Utility::intToRomanNumerals((int) displayTime).c_str());

            if(Settings::debugMode){
            	mvprintw(a++, gameArea.x + 1, "Time: %.2f", displayTime);
            	mvprintw(a++, gameArea.x+1, "Tick: %d", tick);
            }

            if(currentPlayer->abilityPoints > 0){
                a++;
                printMultiLineString(a++, gameArea.x+1, "Unspent skill points");
                printMultiLineString(a++, gameArea.x+1, formatString("Press [%s]", keyDisplayName(Key::statsMenu).c_str()).c_str());
            }

            a++;

            if(Settings::autoSave && timeSinceTimePassed > autoSaveTime && timeSinceTimePassed < autoSaveTime+10){
                printMultiLineString(a++, gameArea.x+1, "Auto saving...");
                a++;
            }

            //move(a, gameArea.x + 1);
            //clrtoeol();

            nearestEntities = currentLevel->getAllVisableEntities(currentPlayer->pos, currentPlayer->viewDistance, currentPlayer, true);

            //consolef("Nearest Entity Count: %d", nearestEntities.size());

            for (size_t i = 0; i < nearestEntities.size() && a < terminalSize.y; i++) {
                Entity* nearestEntity = nearestEntities[i];
                if (nearestEntity != nullptr) {
                    Ui::setColor(C_WHITE);

                    a += printMultiLineColoredString(a, gameArea.x + 1, formatString("[&%c%c&%c] %s ", cc(nearestEntity->getFgColor(tick, p, currentLevel), nearestEntity->getBgColor(tick, p, currentLevel)), nearestEntity->getChar(tick, nearestEntity->pos, currentLevel), cc(C_WHITE), nearestEntity->getName().c_str()) );


                    EntityAlive* alive = dynamic_cast<EntityAlive*> (nearestEntity);
                    if (alive) {
                        const int hp = Math::roundToInt(alive->getHp());
                        const int maxHp = Math::roundToInt(alive->getMaxHp());

                        a += printMultiLineColoredString(a, gameArea.x + 2, formatString("HP: &%c%s&%c ", cc((hp < (maxHp / 3 * 2)) ? ((hp < (maxHp / 3)) ? C_LIGHT_RED : C_LIGHT_YELLOW) : C_LIGHT_GREEN), Utility::makeBar(hp, maxHp, (terminalSize.x - (gameArea.x + 1) - 5)).c_str(), cc(C_WHITE)) );
                    }
                    a++;
                }

            }


        } else {
            //printCenterOffset(gameArea.y / 2, -(borderSize.x / 2), "G a m e   O v e r");
            //printCenterOffset(gameArea.y / 2 + 3, -(borderSize.x / 2), "Press Escape to return to the Main Menu.");

            Art* a = Arts::getArt(Arts::artGameOver);
            a->printCenter((gameArea.y / 2) - ((int)a->lines.size()/2), -(borderSize.x / 2));
            printCenterOffset(gameArea.y / 2 + 3 + ((int)a->lines.size()/2), -(borderSize.x / 2), "Press Escape to return to the Main Menu.");
        }

        if(currentLevel != nullptr){
            //currentLevel->menuGame = this;

            if (Settings::debugMode) {
                Ui::setColor(C_LIGHT_GRAY);
                mvprintw(gameArea.y, 0, "%d, %d [%dx%d] e:%d te:%d p:%s %s(%d)", p.x, p.y, currentLevel->getSize().x, currentLevel->getSize().y, currentLevel->entityCount(), currentLevel->tileEntityList.size(), (currentPlayer == nullptr) ? "0" : "1", currentLevel->getName().c_str(), currentLevel->getDifficulty());
            }
        }



        printConsole(gameArea.y + 1);

        //refresh();
    }

    void MenuGame::changeMode(int newMode) {
        if (newMode == controlMode) {
            return;
        }
        switch (controlMode) {
            case modeSelectDirection:
            case modeSelectPosition:
            case modeSelectEntity:
                if(!targetPosition.inRange(viewPos, viewPos+gameArea)){
                    targetPosition = viewPos+(gameArea/2);
                }
                itemToBeUsedRange = -1;
                itemToBeUsed = nullptr;
                targetEntity = nullptr;
                break;

            default:
                break;
        }
        controlMode = newMode;
    }

}
