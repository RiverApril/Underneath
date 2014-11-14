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
#include "AiEntity.h"
#include "Math.h"
#include "Utility.h"


namespace Ui {


    MenuGame::MenuGame(string worldName) : Menu(false) {

        move(0, 0);
        clrtobot();
        mvaddstr(0, 0, "Loading...");

        if(!init(worldName)){
            changeMenu(new MenuMain());
            return;
        }

        viewPos = new Point2(0, 0);

        viewMoveSpeed = new Point2(0, 0);

        viewUpdate();
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

        currentLevel = currentWorld->currentLevel;
        currentPlayer = currentWorld->currentPlayer;

        updateView = true;

        return true;
    }

    MenuGame::~MenuGame() {
        delete viewPos;
        delete viewMoveSpeed;
        currentWorld = nullptr;
    }

    void MenuGame::openUi(Menu* oldMenu) {

        setGameAreaSize();


        viewPos = new Point2(0, 0);
        viewMoveSpeed = new Point2(2, 1);


        updateView = true;
        refresh();
    }

    void MenuGame::closeUi(Menu* newMenu){
        if(!newMenu->isTemp){
            delete this;
        }
    }

    void MenuGame::setGameAreaSize() {
        gameArea.x = terminalSize.x - borderSize.x;
        gameArea.y = terminalSize.y - borderSize.y;


        updateView = true;
    }

    void MenuGame::viewUpdate() {
        if(mode == modePlayerControl && currentPlayer != nullptr) {
            viewPos->x = currentPlayer->getPos().x-gameArea.x/2;
            viewPos->y = currentPlayer->getPos().y-gameArea.y/2;
        }
        updateView = true;
    }

    void MenuGame::drawTileAt(Point2 p) {
        bool inView = false;
        if(currentLevel != nullptr){
            if(currentPlayer != nullptr) {
                if(currentLevel->canSee(currentPlayer->getPos(), p, currentPlayer->getViewDistance())) {
                    currentLevel->setExplored(p, true);
                    inView = true;
                }
                if(!currentLevel->getExplored(p)) {
                    Ui::setColor(C_DARK_BLACK);
                    addch(' ');
                    return;
                }
            }
            Tile* tempTile = currentLevel->tileAt(p);

            if(currentLevel->inRange(p)) {
                if(currentLevel->getDisplayEntity(p) != nullptr) {

                    if(inView) {
                        if(currentPlayer == currentLevel->getDisplayEntity(p) && mode == modeInterectChoose){
							Ui::setColor(currentLevel->getDisplayEntity(p)->getBgColorCode(), currentLevel->getDisplayEntity(p)->getFgColorCode());
                        }else{
                        	Ui::setColor(currentLevel->getDisplayEntity(p)->getFgColorCode(), currentLevel->getDisplayEntity(p)->getBgColorCode());
                        }
                        addch(currentLevel->getDisplayEntity(p)->getIcon(p, (int)tick, currentLevel));
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
        if(mode  == modeInterectChoose){

            if(currentPlayer->interact(currentLevel, currentPlayer->getPos()+p)){
                timePassed = true;
            }
            mode = modePlayerControl;

        }else if(mode == modeAdjustBorder){
            borderSize -= p;
            setGameAreaSize();
        } else if(mode == modePlayerControl && currentPlayer != nullptr && currentLevel != nullptr){
            if(currentPlayer->moveRelative(p, currentLevel)){
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

            case 27: //Escape

                WorldLoader::save(currentWorld);
                changeMenu(new MenuMain());
                break;

            case KEY_UP:
            case 'w':
                arrowMove(Point2Up);
                break;

            case KEY_DOWN:
            case 's':
                arrowMove(Point2Down);
                break;

            case KEY_LEFT:
            case 'a':
                arrowMove(Point2Left);
                break;

            case KEY_RIGHT:
            case 'd':
                arrowMove(Point2Right);
                break;

            case 'i':
                changeMenu(new MenuTempInv(currentPlayer));
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

            case KEY_ENTER:
            case 13:
            case '\n':
                if(!consoleInputMode){
                    consoleInputMode = true;
                }
                break;

            default:
                break;
        }
        if(currentPlayer != nullptr && currentLevel != nullptr){
            switch (in) {

                case 'g':
                    if(mode == modeInterectChoose){
                    	if(currentPlayer->interact(currentLevel, currentPlayer->getPos())){
                        	timePassed = true;
                    	}
                        mode = modePlayerControl;
                    }else{
                        mode = modeInterectChoose;
                        debug("Interect Mode");
                    }
                    break;

                case 'r':{
                    Point2 p = currentLevel->findRandomWithoutFlag(tileFlagSolid);
                    if(currentLevel->canPathTo(currentPlayer->getPos(), p)){
                    	if(currentPlayer->moveAbsalute(p, currentLevel)){
                      	  timePassed = true;
                    	}
                    }
                    break;
                }

                case 'h':
                    timePassed = true;
                    break;

                case 'o':

                    currentPlayer->hurt(1);
                    break;

                case 'p':
                    
                    currentPlayer->heal(1);
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
            if(currentLevel->update((int)tick, viewPos)){
                updateView = true;
            }
        }

        viewUpdate();
        move(0, 0);
        clrtobot();
        for(int j=0; j<gameArea.y; j++) {
            move(j, 0);
            for(int i=0; i<gameArea.x; i++) {
                drawTileAt(Point2(viewPos->x+i, viewPos->y+j));
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
        Point2 p = *viewPos;
        if(currentPlayer!=nullptr){
            p = currentPlayer->getPos();
        }
        //mvprintw(gameArea.y, 0, "%d, %d  ms: %.2f  fps: %d  e: %d  %s  t: %d", p.x, p.y, ms, fps, currentLevel->entityCount(), updateView?"draw":"", tick);

        if(currentPlayer != nullptr){
            mvprintw(0, gameArea.x+1, "%s", currentPlayer->getName().c_str());

            mvprintw(1, gameArea.x+1, "%s %s", "Species", "Class");

            const int hp = currentPlayer->getHp();
            const int maxHp = currentPlayer->getMaxHp();
            const int mp = currentPlayer->getMp();
            const int maxMp = currentPlayer->getMaxMp();
            
            move(2, gameArea.x+1);
            
            clrtoeol();
            
            Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
            mvprintw(2, gameArea.x+1, "HP: %d/%d", hp, maxHp);
            addch(' ');
            printw("%s", Utility::makeBar(hp, maxHp, terminalSize.x - getcurx(stdscr) - 2).c_str());

            Ui::setColor(C_DARK_CYAN);
            mvprintw(3, gameArea.x+1, "MP: %d/%d", mp, maxMp);
            addch(' ');
            printw("%s", Utility::makeBar(mp, maxMp, terminalSize.x - getcurx(stdscr) - 2).c_str());
        }
        
        Ui::setColor(C_WHITE);
        
        mvprintw(5, gameArea.x+1, "Time: %u", currentWorld->worldTime);
        //mvprintw(11, gameArea.x+1, "Tick: %d", tick);

        if(currentPlayer->getActiveWeapon() != nullptr){
        	mvprintw(7, gameArea.x+1, "W: %s(%d)", currentPlayer->getActiveWeapon()->name.c_str(), currentPlayer->getActiveWeapon()->baseDamage);
        }

        for(int i=0;i<currentPlayer->effects.size();i++){
            Effect eff = currentPlayer->effects[i];
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
        
        if(currentLevel != nullptr){
            mvprintw(gameArea.y, 0, "%d, %d  e: %d", p.x, p.y, currentLevel->entityCount());
        }
        
        
        updateView = false;
        timePassed = false;

        printConsole(gameArea.y+1);
        
        refresh();
        
        
    }

}
