//
//  UiMenuGame.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "UiMenuGame.h"
#include "AiEntity.h"
#include "Math.h"


namespace Ui {


MenuGame::MenuGame() : Menu((1000/60)) {
    currentLevel = new Level();
    currentLevel->generate(static_cast<unsigned int>(time(NULL)));

    currentPlayer = new Player("PlayerNameHere", "@", '@', Point2Zero, C_WHITE);
    currentLevel->newEntity(currentPlayer);


    viewPos = new Point2(0, 0);
    viewMoveSpeed = new Point2(0, 0);

    move(0, 0);
    clrtobot();
    mvaddstr(0, 0, "Loading...");
}

MenuGame::~MenuGame() {
    delete currentLevel;
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

void MenuGame::viewMove(Point2 p) {
    if(playerMode && currentPlayer != nullptr) {
        currentPlayer->move(p, currentLevel);
        viewPos->x = currentPlayer->getPos()->x-GAME_WIDTH/2;
        viewPos->y = currentPlayer->getPos()->y-GAME_HEIGHT/2;
    }

    if(viewPos->x < 0) {
        viewPos->x = 0;
    } else if(viewPos->x > currentLevel->WIDTH-GAME_WIDTH) {
        viewPos->x = currentLevel->WIDTH-GAME_WIDTH;
    }
    if(viewPos->y < 0) {
        viewPos->y = 0;
    } else if(viewPos->y > currentLevel->HEIGHT-GAME_HEIGHT) {
        viewPos->y = currentLevel->HEIGHT-GAME_HEIGHT;
    }
    updateView = true;
}

void MenuGame::drawTileAt(Point2 p) {
    bool inView = false;
    if(playerMode && currentPlayer != nullptr) {
        if(currentLevel->canSee(*currentPlayer->getPos(), p, currentPlayer->getViewDistance())) {
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
        if(currentLevel->displayEntityGrid[p.x][p.y] != nullptr) {


            if(inView) {
                Ui::setColor(currentLevel->displayEntityGrid[p.x][p.y]->getColorCode());
                if(unicodeSupport) {
                    addstr(currentLevel->displayEntityGrid[p.x][p.y]->getIcon(p, tick, currentLevel).c_str());
                } else {
                    addch(currentLevel->displayEntityGrid[p.x][p.y]->getIconAlt(p, tick, currentLevel));
                }
                return;
            }

        }
    }
    Ui::setColor(tempTile->getColorCode(inView));
    if(unicodeSupport) {
        addstr(tempTile->getIcon().c_str());
    } else {
        addch(tempTile->getIconAlt());
    }
}

void MenuGame::handleInput(int in) {
    switch (in) {
        case KEY_UP:
        case 'w':
            viewMove(Point2Up);
            break;

        case KEY_DOWN:
        case 's':
            viewMove(Point2Down);
            break;

        case KEY_LEFT:
        case 'a':
            viewMove(Point2Left);
            break;

        case KEY_RIGHT:
        case 'd':
            viewMove(Point2Right);
            break;

        case 'W':
            viewMove(Point2Up*5);
            break;

        case 'S':
            viewMove(Point2Down*5);
            break;

        case 'A':
            viewMove(Point2Left*5);
            break;

        case 'D':
            viewMove(Point2Right*5);
            break;

        case 'z':
            debugMode = true;
            updateView = true;
            break;

        case 'x':
            currentLevel->newEntity(new AiEntity(AiType::aiMoveRandom, "A", 'A', (*viewPos)+Point2(GAME_WIDTH/2, GAME_HEIGHT/2), Ui::C_DARK_RED));
            break;

        case 'c':
            currentLevel->newEntity(new AiEntity(AiType::aiMoveRandom, "B", 'B', (*viewPos)+Point2(GAME_WIDTH/2, GAME_HEIGHT/2), Ui::C_LIGHT_GREEN));
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
		p = *currentPlayer->getPos();
    }
    mvprintw(TERMINAL_HEIGHT-1, 0, "%d, %d  ms: %.2f  fps: %d  e: %d  %s", p.x, p.y, ms, fps, currentLevel->entityCount(), updateView?"draw":"");
    debugMessage = "";
    updateView = false;
    refresh();


}

}
