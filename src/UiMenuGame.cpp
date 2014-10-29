//
//  UiMenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "UiMenuGame.h"
#include "UiMenuMain.h"
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
    }

    bool MenuGame::init(std::string worldName){
        
        if(worldName.size() == 0){
            return false;
        }

        if(WorldLoader::exists(worldName)){
            if(!WorldLoader::load(worldName)){
                return false;
            }
        }else{
            if(!WorldLoader::create(worldName)){
                return false;
            }
        }

        currentWorld = WorldLoader::loadedWorld;
        currentLevel = currentWorld->currentLevel;
        currentPlayer = currentWorld->currentPlayer;

        return true;
    }

    MenuGame::~MenuGame() {
        delete currentLevel;
        delete viewPos;
        delete viewMoveSpeed;
        delete currentLevel;
        delete currentPlayer;
    }

    void MenuGame::openUi() {
        paused = false;

        setGameAreaSize();


        viewPos = new Point2(0, 0);
        viewMoveSpeed = new Point2(2, 1);


        refresh();
    }

    void MenuGame::setGameAreaSize() {
        const int gameWidthMin = 8;
        const int edgeHeightMin = 2;
        const int edgeWidthPreferred = 20;
        GAME_WIDTH = max(TERMINAL_WIDTH - edgeWidthPreferred, gameWidthMin);
        GAME_HEIGHT = max(TERMINAL_HEIGHT - edgeHeightMin, edgeHeightMin);


        updateView = true;
    }

    void MenuGame::viewUpdate() {
        if(playerMode && currentPlayer != nullptr) {
            viewPos->x = currentPlayer->getPos().x-GAME_WIDTH/2;
            viewPos->y = currentPlayer->getPos().y-GAME_HEIGHT/2;
        }

        if(viewPos->x < 0) {
            viewPos->x = 0;
        } else if(viewPos->x > currentLevel->getSize().x-GAME_WIDTH) {
            viewPos->x = currentLevel->getSize().x-GAME_WIDTH;
        }
        if(viewPos->y < 0) {
            viewPos->y = 0;
        } else if(viewPos->y > currentLevel->getSize().y-GAME_HEIGHT) {
            viewPos->y = currentLevel->getSize().y-GAME_HEIGHT;
        }
        updateView = true;
    }

    void MenuGame::drawTileAt(Point2 p) {
        bool inView = false;
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
                    addch(currentLevel->getDisplayEntity(p)->getIcon(p, tick, currentLevel));
                    return;
                }

            }
        }
        Ui::setColor(tempTile->getColorCode(inView));
        addch(tempTile->getIcon());
    }

    void MenuGame::handleInput(int in) {
        switch (in) {
            case KEY_UP:
            case 'w':
                currentPlayer->move(Point2Up, currentLevel);
                viewUpdate();
                break;

            case KEY_DOWN:
            case 's':
                currentPlayer->move(Point2Down, currentLevel);
                viewUpdate();
                break;

            case KEY_LEFT:
            case 'a':
                currentPlayer->move(Point2Left, currentLevel);
                viewUpdate();
                break;

            case KEY_RIGHT:
            case 'd':
                currentPlayer->move(Point2Right, currentLevel);
                viewUpdate();
                break;

            case 'g':
                currentPlayer->use(currentLevel);
                break;

            case 'x':
                currentLevel->newEntity(new AiEntity(AiType::aiMoveRandom, 'A', (*viewPos)+Point2(GAME_WIDTH/2, GAME_HEIGHT/2), Ui::C_DARK_RED));
                break;

            case 'c':
                currentLevel->newEntity(new AiEntity(AiType::aiMoveRandom, 'B', (*viewPos)+Point2(GAME_WIDTH/2, GAME_HEIGHT/2), Ui::C_LIGHT_GREEN));
                break;

            case ' ':
                paused = !paused;
                break;

            case KEY_RESIZE:
                setGameAreaSize();
                break;

            case ERR:
                break;

            case 27: //Escape
                changeMenu(new MenuMain());
                break;

            default:
                break;
        }
    }

    void MenuGame::update() {

        if(currentWorld==nullptr){
            return;
        }

        if(!paused) {
            updateView = currentLevel->update(tick, viewPos) || updateView;
        } else {
            debugMessage+="Paused ";
        }

        if(updateView) {
            move(0, 0);
            clrtobot();
            for(int j=0; j<GAME_HEIGHT; j++) {
                move(j, 0);
                for(int i=0; i<GAME_WIDTH; i++) {
                    drawTileAt(Point2(viewPos->x+i, viewPos->y+j));
                }
            }
        }

        Ui::setColor(C_WHITE);
        if(debugMessage.length() > 0) {
            move(TERMINAL_HEIGHT-2, 0);
            clrtobot();
            mvprintw(TERMINAL_HEIGHT-2, 0, debugMessage.c_str());
        }
        Point2 p = *viewPos;
        if(currentPlayer!=nullptr){
            p = currentPlayer->getPos();
        }
        mvprintw(TERMINAL_HEIGHT-1, 0, "%d, %d  ms: %.2f  fps: %d  e: %d  %s", p.x, p.y, ms, fps, currentLevel->entityCount(), updateView?"draw":"");
        debugMessage = "";
        updateView = false;
        refresh();
        
        
    }
    
}
