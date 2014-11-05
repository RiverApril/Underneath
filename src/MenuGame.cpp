//
//  MenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuGame.h"
#include "MenuMain.h"
#include "AiEntity.h"
#include "Math.h"


namespace Ui {


    MenuGame::MenuGame(std::string worldName) : Menu((1000/60)) {

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

    bool MenuGame::init(std::string worldName){

        if(worldName.size() == 0){
            return false;
        }

        if(currentWorld != nullptr){
            delete currentWorld;
            currentWorld = nullptr;
        }

        if(WorldLoader::exists(worldName)){
            debug(worldName+" exists");
            debug("loading");
            currentWorld = WorldLoader::load(worldName);
            if(currentWorld == nullptr){
                debug("load failed, creating new");
                currentWorld = WorldLoader::create(worldName);
            }
        }else{
            debug(worldName+" does not exist, creating new");
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
        delete currentWorld;
        currentWorld = nullptr;
    }

    void MenuGame::openUi() {

        setGameAreaSize();


        viewPos = new Point2(0, 0);
        viewMoveSpeed = new Point2(2, 1);


        updateView = true;
        refresh();
    }

    void MenuGame::setGameAreaSize() {
        gameArea.x = terminalSize.x - 24;
        gameArea.y = terminalSize.y - 2;


        updateView = true;
    }

    void MenuGame::viewUpdate() {
        if(playerMode && currentPlayer != nullptr) {
            viewPos->x = currentPlayer->getPos().x-gameArea.x/2;
            viewPos->y = currentPlayer->getPos().y-gameArea.y/2;
        }

        /*if(viewPos->x < 0) {
         viewPos->x = 0;
         } else if(viewPos->x > currentLevel->getSize().x-gameArea.x) {
         viewPos->x = currentLevel->getSize().x-gameArea.x;
         }
         if(viewPos->y < 0) {
         viewPos->y = 0;
         } else if(viewPos->y > currentLevel->getSize().y-gameArea.y) {
         viewPos->y = currentLevel->getSize().y-gameArea.y;
         }*/
        updateView = true;
    }

    void MenuGame::drawTileAt(Point2 p) {
        bool inView = false;
        if(currentLevel != nullptr){
            if(playerMode && currentPlayer != nullptr) {
                if(currentLevel->canSee(currentPlayer->getPos(), p, currentPlayer->getViewDistance())) {
                    currentLevel->setExplored(p, true);
                    inView = true;
                }
                if(!currentLevel->getExplored(p)) {
                    Ui::setColor(C_BLACK);
                    addch(' ');
                    return;
                }
            }
            Tile* tempTile = currentLevel->tileAt(p);

            if(currentLevel->inRange(p)) {
                if(currentLevel->getDisplayEntity(p) != nullptr) {

                    if(inView) {
                        Ui::setColor(currentLevel->getDisplayEntity(p)->getColorCode());
                        addch(currentLevel->getDisplayEntity(p)->getIcon(p, (int)tick, currentLevel));
                        return;
                    }

                }
            }
            Ui::setColor(tempTile->getColorCode(inView));
            addch(tempTile->getIcon());
        }else{
            Ui::setColor(C_LIGHT_MAGENTA);
            addch('?');
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

            default:
                break;
        }
        if(currentPlayer != nullptr && currentLevel != nullptr){
            switch (in) {
                case KEY_UP:
                case 'w':
                    if(currentPlayer->moveRelative(Point2Up, currentLevel)){
                        timePassed = true;
                    }
                    viewUpdate();
                    break;

                case KEY_DOWN:
                case 's':
                    if(currentPlayer->moveRelative(Point2Down, currentLevel)){
                        timePassed = true;
                    }
                    viewUpdate();
                    break;

                case KEY_LEFT:
                case 'a':
                    if(currentPlayer->moveRelative(Point2Left, currentLevel)){
                        timePassed = true;
                    }
                    viewUpdate();
                    break;

                case KEY_RIGHT:
                case 'd':
                    if(currentPlayer->moveRelative(Point2Right, currentLevel)){
                        timePassed = true;
                    }
                    viewUpdate();
                    break;

                case 'g':
                    if(currentPlayer->use(currentLevel)){
                        timePassed = true;
                    }
                    break;

                case 'r':
                    if(currentPlayer->moveExact(currentLevel->findRandomOfType(tileFloor->getIndex()), currentLevel)){
                        timePassed = true;
                    }
                    viewUpdate();
                    break;

                case 'R':{
                    Point2 p;
                    int i = rand()%currentLevel->entityList.size();
                    p = currentLevel->entityList[i]->getPos();
                    if(currentPlayer->moveExact(p, currentLevel)){
                        timePassed = true;
                    }
                    break;
                }

                case 'o':
                    currentPlayer->hurt(1);
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
            time++;
            if(currentLevel->update((int)tick, viewPos)){
                updateView = true;
            }
        }

        if(updateView) {
            viewUpdate();
            move(0, 0);
            clrtobot();
            for(int j=0; j<gameArea.y; j++) {
                move(j, 0);
                for(int i=0; i<gameArea.x; i++) {
                    drawTileAt(Point2(viewPos->x+i, viewPos->y+j));
                }
            }
            setColor(C_WHITE);
            for(int j=0; j<terminalSize.y; j++) {
                mvaddch(j, gameArea.x, '|');
            }
            for(int i=0; i<terminalSize.x; i++) {
                mvaddch(gameArea.y, i, '-');
            }
            mvaddch(gameArea.y, gameArea.x, '+');
        }

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
            
            move(2, gameArea.x+1);
            
            clrtoeol();
            
            Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
            mvprintw(2, gameArea.x+1, "HP: %d/%d %s", hp, maxHp, currentPlayer->getHpBar(10).c_str());
        }
        
        Ui::setColor(C_WHITE);
        
        mvprintw(10, gameArea.x+1, "Time: %d", time);
        mvprintw(11, gameArea.x+1, "Tick: %d", tick);
        
        if(currentLevel != nullptr){
            mvprintw(gameArea.y, 0, "%d, %d  e: %d", p.x, p.y, currentLevel->entityCount());
        }
        
        
        updateView = false;
        timePassed = false;
        
        refresh();
        
        
    }
    
}
