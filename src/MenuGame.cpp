//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.h"
#include "MenuMain.h"
#include "MenuTempInv.h"
#include "MenuTempYesNo.h"
#include "AiEntity.h"
#include "Math.h"
#include "Utility.h"


namespace Ui {


    MenuGame::MenuGame(string worldName) : Menu() {

        printConsoleByDefault = false;

        move(0, 0);
        clrtobot();
        mvaddstr(0, 0, "Building Level...");
        refresh();

        initSuccess = init(worldName);

        viewPos = Point2Zero;
        viewMoveSpeed = Point2(2, 1);

        /*if(currentWorld->currentLevel != nullptr){
        	currentWorld->currentLevel->setAndUnsetDisplayEntities();
        }*/

        viewUpdate();
    }

    MenuGame::~MenuGame() {
        delete currentWorld;
        delete saveAnswer;
    }

    bool MenuGame::init(string worldName){

        if(worldName.size() == 0){
            return false;
        }

        if(WorldLoader::exists(worldName)){
            currentWorld = WorldLoader::load(worldName);
            if(currentWorld == nullptr){
                currentWorld = WorldLoader::create(worldName);
            }
        }else{
            currentWorld = WorldLoader::create(worldName);
        }
        if(currentWorld == nullptr){
            return false;
        }

        updateView = true;

        return true;
    }

    bool MenuGame::openUi() {
        if(*saveAnswer == aYes){
            WorldLoader::save(currentWorld);
            *saveAnswer = aUndefined;
            closeThisMenu();
            return false;
        }else if(*saveAnswer == aNo){
            *saveAnswer = aUndefined;
            closeThisMenu();
            return false;
        }

        if(!initSuccess){
            closeThisMenu();
        }

        setGameAreaSize();


        updateView = true;
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


        updateView = true;
    }

    void MenuGame::viewUpdate() {
        if(mode == modePlayerControl) {
            
            if(currentWorld->currentPlayer != nullptr){
                viewPos = currentWorld->currentPlayer->pos - (gameArea/2);
            }
        }
        updateView = true;
    }

    void MenuGame::drawTileAt(Point2 p) {
        bool inView = false;
        if(currentWorld->currentLevel != nullptr){
            if(currentWorld->currentPlayer != nullptr) {
                if(currentWorld->currentLevel->canSee(currentWorld->currentPlayer->pos, p, currentWorld->currentPlayer->viewDistance)) {
                    currentWorld->currentLevel->setExplored(p, true);
                    inView = true;
                }
            }
            if(!currentWorld->currentLevel->getExplored(p)) {
                Ui::setColor(C_DARK_BLACK);
                addch(' ');
                return;
            }
            Tile* tempTile = currentWorld->currentLevel->tileAt(p);

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
							Ui::setColor(e->getBgColorCode(), e->getFgColorCode());
                        }else{
                        	Ui::setColor(e->getFgColorCode(), e->getBgColorCode());
                        }
                        addch(e->getIcon(p, (int)tick, currentWorld->currentLevel));
                        return;
                    }

                }
            }
            Ui::setColor(tempTile->getFgColor(inView), tempTile->getBgColor(inView));
            addch(tempTile->getIcon());
        }else{
            Ui::setColor(C_LIGHT_MAGENTA);
            addch('?');
        }
    }

    void MenuGame::arrowMove(Point2 p){
        if(mode == modeInterectChoose){

            if(currentWorld->currentPlayer->interact(currentWorld->currentLevel, currentWorld->currentPlayer->pos+p, false)){
                timePassed = true;
            }
            mode = modePlayerControl;

        }else if(mode == modeAdjustBorder){
            borderSize -= p;
            setGameAreaSize();
        } else if(mode == modePlayerControl && currentWorld->currentPlayer != nullptr && currentWorld->currentLevel != nullptr){
            if(currentWorld->currentPlayer->moveRelative(p, currentWorld->currentLevel)){
                timePassed = true;
            }
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
                    closeThisMenu();
                }else{
                    openMenu(new MenuTempYesNo("Do you want to save '"+currentWorld->name+"' ?", saveAnswer, true));
                }
                break;

            case KEY_UP:
                arrowMove(Point2Up);
                break;

            case KEY_DOWN:
                arrowMove(Point2Down);
                break;

            case KEY_LEFT:
                arrowMove(Point2Left);
                break;

            case KEY_RIGHT:
                arrowMove(Point2Right);
                break;

            case 'i':
            case '\t':
                openMenu(new MenuTempInv(currentWorld->currentPlayer, currentWorld));
                break;

            case '1':
                mode = modePlayerControl;
                break;

            case '2':
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

                case 'g':
                    if(mode == modeInterectChoose){
                    	if(currentWorld->currentPlayer->interact(currentWorld->currentLevel, currentWorld->currentPlayer->pos, false)){
                        	timePassed = true;
                    	}
                        mode = modePlayerControl;
                    }else{
                        mode = modeInterectChoose;
                    }
                    break;

                case 'r':{
                    Point2 p = currentWorld->currentLevel->findRandomWithoutFlag(tileFlagSolid);
                    //if(currentWorld->currentLevel->canPathTo(currentWorld->currentPlayer->pos, p)){
                    	if(currentWorld->currentPlayer->moveAbsalute(p, currentWorld->currentLevel)){
                      	  timePassed = true;
                    	}
                    //}
                    break;
                }

                case 'h':
                    timePassed = true;
                    break;

                case 'o':
                    currentWorld->currentPlayer->hurt(1);
                    break;

                case 'p':
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

        if(timePassed) {
            currentWorld->worldTime++;
            if(currentWorld->currentLevel->update((int)tick, viewPos)){
                updateView = true;
            }
        }

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
        
        if(currentWorld->currentPlayer != nullptr){
            p = currentWorld->currentPlayer->pos;
            mvprintw(0, gameArea.x+1, "%s", currentWorld->currentPlayer->getName().c_str());

            mvprintw(1, gameArea.x+1, "%s %s", "Species", "Class");

            const int hp = currentWorld->currentPlayer->getHp();
            const int maxHp = currentWorld->currentPlayer->getMaxHp();
            const int mp = currentWorld->currentPlayer->getMp();
            const int maxMp = currentWorld->currentPlayer->getMaxMp();
            
            move(2, gameArea.x+1);
            
            clrtoeol();
            
            Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
            mvprintw(2, gameArea.x+1, "HP: %d/%d", hp, maxHp);
            addch(' ');
            printw("%s", DrawingUtility::makeBar(hp, maxHp, terminalSize.x - getcurx(stdscr) - 2).c_str());

            Ui::setColor(C_DARK_CYAN);
            mvprintw(3, gameArea.x+1, "MP: %d/%d", mp, maxMp);
            addch(' ');
            printw("%s", DrawingUtility::makeBar(mp, maxMp, terminalSize.x - getcurx(stdscr) - 2).c_str());

            if(currentWorld->currentPlayer->getActiveWeapon() != nullptr){
                mvprintw(7, gameArea.x+1, "W: %s(%d)", currentWorld->currentPlayer->getActiveWeapon()->name.c_str(), currentWorld->currentPlayer->getActiveWeapon()->baseDamage);
            }

            for(int i=0;i<currentWorld->currentPlayer->effects.size();i++){
                Effect eff = currentWorld->currentPlayer->effects[i];
                string name = "EFFECT";
                Ui::color color = C_LIGHT_GREEN;
                switch (eff.eId) {
                    case effBleed:
                        name = "Bleed";
                        color = C_LIGHT_RED;
                        break;

                    case effFire:
                        name = "Fire";
                        color = C_LIGHT_RED;
                        break;
                }
                setColor(color);
                mvprintw(9+i, gameArea.x+1, (name+(eff.power==1?"":("("+std::to_string(eff.power)+")"))+": %d").c_str(), eff.power);
            }
            
        }else{
            printCenterOffset(gameArea.y/2, -(borderSize.x/2), "Game Over");
        }
        
        Ui::setColor(C_WHITE);
        
        mvprintw(5, gameArea.x+1, "Time: %u", currentWorld->worldTime);
        //mvprintw(11, gameArea.x+1, "Tick: %d", tick);
        
        if(currentWorld->currentLevel != nullptr){
            mvprintw(gameArea.y, 0, "%d, %d  e: %d  p: %s", p.x, p.y, currentWorld->currentLevel->entityCount(), (currentWorld->currentPlayer == nullptr)?"null":"not null");
        }
        
        
        updateView = false;
        timePassed = false;

        printConsole(gameArea.y+1);
        
        refresh();
        
        
    }

}
