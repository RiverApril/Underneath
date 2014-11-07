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


    MenuGame::MenuGame(std::string worldName) : Menu(false) {

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
        delete currentWorld;
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
            if(currentPlayer != nullptr) {
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

    void MenuGame::arrowMove(int x, int y){
        if(mode == modeAdjustBorder){
            gameArea.x += x;
            gameArea.y += y;
        } else if(mode == modePlayerControl && currentPlayer != nullptr && currentLevel != nullptr){
            if(currentPlayer->moveRelative(Point2(x, y), currentLevel)){
                timePassed = true;
            }
            viewUpdate();
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
                arrowMove(0, -1);
                break;

            case KEY_DOWN:
            case 's':
                arrowMove(0, 1);
                break;

            case KEY_LEFT:
            case 'a':
                arrowMove(-1, 0);
                break;

            case KEY_RIGHT:
            case 'd':
                arrowMove(1, 0);
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
                if(mode == modeConsoleInput){
                    mode = modePlayerControl;
                    execute(input);
                    input = "";
                }else{
                	mode = modeConsoleInput;
                }
                break;

            default:
                break;
        }
        if(mode == modeConsoleInput){
            switch (in) {
                case KEY_BACKSPACE:
                case 8: //Backspace
                case 127: //Delete
                    if(input.length() > 0){
                        input = input.substr(0, input.length()-1);
                    }
                    break;
                    
                default:
                    if((in>=32 && in<=126)){
                        input += in;
                    }
                    break;
            }
        }
        if(currentPlayer != nullptr && currentLevel != nullptr){
            switch (in) {

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
            time++;
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
        setColor((mode == modeAdjustBorder)?C_LIGHT_YELLOW:C_WHITE, (mode == modeAdjustBorder)?A_BLINK:0);
        for(int j=0; j<terminalSize.y; j++) {
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
            
            move(2, gameArea.x+1);
            
            clrtoeol();
            
            Ui::setColor((hp<(maxHp/3*2))?((hp<(maxHp/3))?C_LIGHT_RED:C_LIGHT_YELLOW):C_LIGHT_GREEN);
            mvprintw(2, gameArea.x+1, "HP: %d/%d", hp, maxHp);
            mvprintw(3, gameArea.x+2, "%s",currentPlayer->getHpBar(borderSize.x-3).c_str());
        }
        
        Ui::setColor(C_WHITE);
        
        mvprintw(10, gameArea.x+1, "Time: %d", time);
        mvprintw(11, gameArea.x+1, "Tick: %d", tick);
        
        if(currentLevel != nullptr){
            mvprintw(gameArea.y, 0, "%d, %d  e: %d", p.x, p.y, currentLevel->entityCount());
        }
        
        
        updateView = false;
        timePassed = false;

        printConsole(gameArea.y+1, terminalSize.y-2);

        if(mode == modeConsoleInput){
            Ui::setColor(C_DARK_GREEN);
        	mvprintw(terminalSize.y-1, 0, "> %s", input.c_str());
            Ui::setColor(C_LIGHT_GREEN, A_BLINK);
            mvprintw(terminalSize.y-1, 2+(int)input.length(), "_");
        }
        
        refresh();
        
        
    }


    bool MenuGame::execute(std::string commandRaw){
        std::vector<std::string> arguments;
        int lastI = 0;
        for(int i=0;i<commandRaw.length();i++){
            if(commandRaw[i] == ' '){
                arguments.push_back(commandRaw.substr(lastI, i-lastI));
                lastI = i+1;
            }
        }
        arguments.push_back(commandRaw.substr(lastI));

        for(int i=0;i<arguments.size();i++){
            //debug(arguments[i]);
        }
        return false;
    }

}
