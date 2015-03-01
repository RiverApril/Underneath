//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.h"
#include "MenuMain.h"
#include "MenuInv.h"
#include "MenuStats.h"
#include "MenuYesNo.h"
#include "MenuDebug.h"
#include "AiEntity.h"
#include "Math.h"
#include "Utility.h"
#include "Controls.h"
#include "Ranged.h"


namespace Ui {


    MenuGame::MenuGame(string worldName, Abilities<int> playerAbilities) : Menu() {

        printConsoleByDefault = false;

        move(0, 0);
        clrtobot();
        setColor(C_WHITE, C_BLACK);
        mvaddstr(0, 0, "Building Level...");
        refresh();

        initSuccess = init(worldName, playerAbilities);

        viewPos = Point2Zero;
        viewMoveSpeed = Point2(2, 1);

        /*if(currentWorld->currentLevel != nullptr){
        	currentWorld->currentLevel->setAndUnsetDisplayEntities();
        }*/
        if(initSuccess){
            currentWorld->currentLevel->menuGame = this;
        }

        viewUpdate();


    }

    MenuGame::~MenuGame() {
        delete currentWorld;
        delete saveAnswer;
        delete useItem;
    }

    bool MenuGame::init(string worldName, Abilities<int> playerAbilities){

        if(worldName.size() == 0){
            return false;
        }

        if(WorldLoader::exists(worldName)){
            currentWorld = WorldLoader::load(worldName);
            if(currentWorld == nullptr){
                currentWorld = WorldLoader::create(worldName, playerAbilities);
            }
        }else{
            currentWorld = WorldLoader::create(worldName, playerAbilities);
        }
        if(currentWorld == nullptr){
            return false;
        }

        return true;
    }

    bool MenuGame::openUi() {
        debug("Open MenuGame Ui");

        if(*saveAnswer == aYes){

            WorldLoader::save(currentWorld);
            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        }else if(*saveAnswer == aNo){

            delete currentWorld;
            currentWorld = nullptr;

            *saveAnswer = aUndefined;
            closeAllMenus();
            return false;
        }

        if(*useItem != -1){
            mode = modeInterectChoose;
        }

        if(!initSuccess){
            closeThisMenu();
        }

        setGameAreaSize();

        move(0, 0);
        clrtobot();
        refresh();
        return true;
    }

    void MenuGame::closeUi(){

    }

    void MenuGame::setGameAreaSize() {
        gameArea.x = terminalSize.x - borderSize.x;
        gameArea.y = terminalSize.y - borderSize.y;
    }

    void MenuGame::viewUpdate() {
        if(mode == modePlayerControl) {
            
            if(currentWorld->currentPlayer != nullptr){
                viewPos = currentWorld->currentPlayer->pos - (gameArea/2);
            }
        }
    }

    void MenuGame::drawTileAt(Point2 p) {
        Ui::Color fg = C_LIGHT_RED;
        Ui::Color bg = C_BLACK;
        char symbol = ' ';
        int attr = 0;


        bool inView = false;
        if(currentWorld->currentLevel != nullptr){
            if(currentWorld->currentPlayer != nullptr) {
                if(currentWorld->currentLevel->canSee(currentWorld->currentPlayer->pos, p, currentWorld->currentPlayer->viewDistance, true)) {
                    currentWorld->currentLevel->setExplored(p, true);
                    inView = true;
                }if(!currentWorld->currentLevel->getExplored(p)) {
                    symbol = ' ';
                }else{
                    Tile* tempTile = currentWorld->currentLevel->tileAt(p);

                    fg = tempTile->getFgColor(inView);
                    bg = tempTile->getBgColor(inView);
                    symbol = tempTile->getIcon();

                    if(currentWorld->currentLevel->inRange(p)) {
                        Entity* e = nullptr;
                        int d = INT16_MIN;
                        for(Entity* ei : currentWorld->currentLevel->entityList){
                            if(ei->pos == p){
                                int dd = ei->getRenderDepth();
                                if(d < dd){
                                    e = ei;
                                    d = dd;
                                }
                            }
                        }
                        if(e != nullptr) {

                            if(inView) {
                                if(currentWorld->currentPlayer == e && mode == modeInterectChoose){
                                    fg = e->getBgColorCode();
                                    bg = e->getFgColorCode();
                                }else{
                                    fg = e->getFgColorCode();
                                    bg = e->getBgColorCode();
                                }
                                symbol = e->getIcon(p, tick, currentWorld->currentLevel);
                            }

                        }
                    }
                }
                Ranged* ranged = dynamic_cast<Ranged*>(currentWorld->currentPlayer->getActiveWeapon());
                if(ranged){
                    if(mode == modeSelectTarget){
                        if(p == targetPosition){
                            bg = C_LIGHT_GREEN;
                            attr = A_BLINK;
                        }
                        if(!currentWorld->currentLevel->canSee(currentWorld->currentPlayer->pos, p, ranged->range, false) && inView) {
                            if(p == targetPosition){
                                bg = C_LIGHT_RED;
                            }
                            if(bg == C_BLACK){
                                bg = C_DARK_RED;
                            }
                        }

                    }
                }

            }
        }else{
            fg = C_LIGHT_MAGENTA;
            symbol = '?';
        }

        setColor(fg, bg, attr);
        addch(symbol);
    }

    void MenuGame::arrowMove(Point2 p){
        if(mode == modeSelectTarget){
            targetPosition += p;
        }if(mode == modeInterectChoose){

            Item* i = *useItem!=-1?(currentWorld->currentPlayer->inventory[*useItem]):currentWorld->currentPlayer->getActiveWeapon();
            
            timePassed += currentWorld->currentPlayer->interact(currentWorld->currentLevel, currentWorld->currentPlayer->pos+p, false, i);

            *useItem = -1;

            mode = modePlayerControl;

        }else if(mode == modeAdjustBorder){
            borderSize -= p;
            setGameAreaSize();
        } else if(mode == modePlayerControl && currentWorld->currentPlayer != nullptr && currentWorld->currentLevel != nullptr){

            timePassed += currentWorld->currentPlayer->moveRelative(p, currentWorld->currentLevel);
            
        }
    }

    void MenuGame::handleInput(int in) {
        switch (in) {
            case KEY_RESIZE:
                setGameAreaSize();
                break;

            case ERR:
                break;

            case KEY_ESCAPE: //Escape
                if(currentWorld->currentPlayer == nullptr){
                    WorldLoader::deleteWorld(currentWorld->name);
                    
                    delete currentWorld;
                    currentWorld = nullptr;

                    closeAllMenus();
                }else{
                    openMenu(new MenuYesNo("Do you want to save '"+currentWorld->name+"' ?", saveAnswer, true));
                }
                break;

            case Key::moveUp:
                arrowMove(Point2Up);
                break;

            case Key::moveDown:
                arrowMove(Point2Down);
                break;

            case Key::moveLeft:
                arrowMove(Point2Left);
                break;

            case Key::moveRight:
                arrowMove(Point2Right);
                break;

            case Key::inventory:
                if(currentWorld->currentPlayer != nullptr){
                	openMenu(new MenuInv(currentWorld->currentPlayer, currentWorld, useItem));
                }
                break;

            case Key::stats:
                if(currentWorld->currentPlayer != nullptr){
                	openMenu(new MenuStats(currentWorld->currentPlayer, currentWorld));
                }
                break;

            case 'd':
                if(currentWorld->currentPlayer != nullptr){
                    openMenu(new MenuDebug(currentWorld));
                }
                break;

            case '\\':
                if(mode == modeAdjustBorder){
                    mode = modePlayerControl;
                }else{
                    mode = modeAdjustBorder;
                }
                break;
                
            case '\n':
                if(!consoleInputMode){
                    consoleInputMode = true;
                }
                break;

            default:
                break;
        }
        if(currentWorld != nullptr && currentWorld->currentPlayer != nullptr && currentWorld->currentLevel != nullptr){
            switch (in) {

                case Key::interact:
                    if(mode == modeInterectChoose){

                        timePassed += currentWorld->currentPlayer->interact(currentWorld->currentLevel, currentWorld->currentPlayer->pos, false, currentWorld->currentPlayer->getActiveWeapon());

                        mode = modePlayerControl;
                    }else if(mode == modeSelectTarget){

                        timePassed += currentWorld->currentPlayer->interact(currentWorld->currentLevel, targetPosition, false, currentWorld->currentPlayer->getActiveWeapon());

                        mode = modePlayerControl;
                    }else{
                        Ranged* ranged = dynamic_cast<Ranged*>(currentWorld->currentPlayer->getActiveWeapon());
                        if(ranged){
                            mode = modeSelectTarget;
                            if(!currentWorld->currentLevel->canSee(currentWorld->currentPlayer->pos, targetPosition, ranged->range, false)){
                            	targetPosition = currentWorld->currentPlayer->pos;
                            }
                        }else{
                        	mode = modeInterectChoose;
                        }
                    }
                    break;

                case 'r':{
                    Point2 p = currentWorld->currentLevel->findRandomWithoutFlag(tileFlagSolid);
                    timePassed += currentWorld->currentPlayer->moveAbsalute(p, currentWorld->currentLevel);
                    break;
                }

                case 'h':
                    timePassed += currentWorld->currentPlayer->waitUntilHealed();
                    break;

                case '[':
                    currentWorld->currentPlayer->hurt(1);
                    break;

                case ']':
                    currentWorld->currentPlayer->heal(1);
                    break;



                default:
                    break;
            }
        }
    }

    void MenuGame::update() {

        if(currentWorld==nullptr){
            return;
        }

        render(currentWorld->worldTime);

        if(timePassed > 0) {
            currentWorld->worldTime += timePassed;
            timePassed = 0;
            currentWorld->currentLevel->update(currentWorld->worldTime, viewPos);
            render(currentWorld->worldTime);
        }
        
    }

    void MenuGame::render(double displayTime) {
        viewUpdate();
        move(0, 0);
        clrtobot();
        for(int j=0; j<gameArea.y; j++) {
            move(j, 0);
            for(int i=0; i<gameArea.x; i++) {
                drawTileAt(Point2(viewPos.x+i, viewPos.y+j));
            }
        }
        setColor((mode == modeAdjustBorder)?C_BLACK:C_WHITE,
                 (mode == modeAdjustBorder)?C_LIGHT_YELLOW:C_BLACK,
                 (mode == modeAdjustBorder)?A_BLINK:0);

        for(int j=0; j<gameArea.y; j++) {
            mvaddch(j, gameArea.x, '|');
        }
        for(int i=0; i<terminalSize.x; i++) {
            mvaddch(gameArea.y, i, '-');
        }
        mvaddch(gameArea.y, gameArea.x, '+');

        Ui::setColor(C_WHITE);
        Point2 p = viewPos;

        int a = 0;

        if(currentWorld->currentPlayer != nullptr){

            p = currentWorld->currentPlayer->pos;
            mvprintw(a, gameArea.x+1, "%s", currentWorld->currentPlayer->getName().c_str());

            const int hp = Math::roundToInt(currentWorld->currentPlayer->getHp());
            const int maxHp = Math::roundToInt(currentWorld->currentPlayer->getMaxHp());
            const int mp = Math::roundToInt(currentWorld->currentPlayer->getMp());
            const int maxMp = Math::roundToInt(currentWorld->currentPlayer->getMaxMp());

            a++;

            move(a, gameArea.x+1);
            clrtoeol();

            mvprintw(a, gameArea.x+1, "HP: %d/%d", hp, maxHp);
            Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
            printw(" %s", StringUtility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
            Ui::setColor(C_WHITE);

            a++;

            move(a, gameArea.x+1);
            clrtoeol();

            mvprintw(a, gameArea.x+1, "MP: %d/%d", mp, maxMp);
            Ui::setColor(C_LIGHT_BLUE);
            printw(" %s", StringUtility::makeBar(mp, maxMp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
            Ui::setColor(C_WHITE);

            a++;

            for(size_t i=0;i<currentWorld->currentPlayer->effects.size();i++){
                Effect eff = currentWorld->currentPlayer->effects[i];
                string name = "EFFECT";
                Ui::Color color = C_LIGHT_GREEN;
                switch (eff.eId) {
                    case effBleed:
                        name = "Bleed";
                        color = C_LIGHT_RED;
                        break;

                    case effFire:
                        name = "Fire";
                        color = C_LIGHT_YELLOW;
                        break;

                    case effRegen:
                        name = "Regen";
                        color = C_LIGHT_GREEN;
                        break;
                }
                setColor(color);
                mvprintw(a, gameArea.x+1, (name+" %s: %.2f").c_str(), ParsingUtility::intToRomanNumerals((int)eff.power).c_str(), eff.timeEnd-currentWorld->worldTime);
                a++;
            }

            a++;

            Ui::setColor(C_WHITE);

            mvprintw(a, gameArea.x+1, "Time: %.2f", displayTime);
            a++;
            mvprintw(a, gameArea.x+1, "Time: %s", ParsingUtility::intToRomanNumerals((int)displayTime).c_str());
            //mvprintw(11, gameArea.x+1, "Tick: %d", tick);

            a++;
            a++;

            move(a, gameArea.x+1);
            clrtoeol();

            vector<Entity*> nearestEntities = currentWorld->currentLevel->getAllVisableEntitiesSortedByNearest(currentWorld->currentPlayer->pos, currentWorld->currentPlayer->viewDistance, currentWorld->currentPlayer);

            for(size_t i=0; i<nearestEntities.size() && a<terminalSize.y; i++){
                Entity* nearestEntity = nearestEntities[i];
                if(nearestEntity != nullptr){
                    Ui::setColor(C_WHITE);
                    mvprintw(a, gameArea.x+1, "%s [", nearestEntity->getName().c_str());
                    Ui::setColor(nearestEntity->getFgColorCode(), nearestEntity->getBgColorCode());
                    printw("%c", nearestEntity->getIcon(nearestEntity->pos, currentWorld->worldTime, currentWorld->currentLevel));
                    Ui::setColor(C_WHITE);
                    printw("]");
                    a++;
                    AiEntity* aiEntity = dynamic_cast<AiEntity*>(nearestEntity);
                    if(aiEntity){
                        const int hp = Math::roundToInt(aiEntity->getHp());
                        const int maxHp = Math::roundToInt(aiEntity->getMaxHp());
                        mvprintw(a, gameArea.x+1, " HP: %d/%d", hp, maxHp);
                        Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
                        printw(" %s", StringUtility::makeBar(hp, maxHp, (terminalSize.x - getcurx(stdscr) - 2)).c_str());
                        Ui::setColor(C_WHITE);
                        a++;
                    }
                    a++;
                }

            }


        }else{
            printCenterOffset(gameArea.y/2, -(borderSize.x/2), "Game Over");
        }


        if(currentWorld->currentLevel != nullptr){
            Ui::setColor(C_DARK_GRAY);
            mvprintw(gameArea.y, 0, "%d, %d  e: %d  te: %d  p: %s   %s", p.x, p.y, currentWorld->currentLevel->entityCount(), currentWorld->currentLevel->tileEntityList.size(), (currentWorld->currentPlayer == nullptr)?"null":"not null", currentWorld->currentLevel->getName().c_str());
        }


        
        printConsole(gameArea.y+1);
        
        refresh();
    }

}
