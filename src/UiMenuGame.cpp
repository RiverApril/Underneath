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

    currentPlayer = new Player("PlayerNameHere", "@", '@', Geometry::Point2Zero, C_WHITE);
    currentLevel->newEntity(currentPlayer);


    viewPos = new Geometry::Point2(0, 0);
    viewMoveSpeed = new Geometry::Point2(0, 0);
    cursorPosition = new Geometry::Point2(0, 0);

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


    viewPos = new Geometry::Point2(0, 0);
    viewMoveSpeed = new Geometry::Point2(2, 1);
    cursorPosition = new Geometry::Point2(GAME_WIDTH/2, GAME_HEIGHT/2);


    refresh();
}

void MenuGame::setGameAreaSize() {
    const int gameWidthMin = 8;
    const int edgeHeightMin = 2;
    const int edgeWidthPreferred = 20;
    GAME_WIDTH = Math::max(TERMINAL_WIDTH - edgeWidthPreferred, gameWidthMin);
    GAME_HEIGHT = Math::max(TERMINAL_HEIGHT - edgeHeightMin, edgeHeightMin);


    updateView = true;
}

void MenuGame::viewMove(int x, int y, bool cursor) {
    if(playerMode && currentPlayer != nullptr) {
        currentPlayer->move(x, y, currentLevel);
        viewPos->x = currentPlayer->getPos()->x-GAME_WIDTH/2;
        viewPos->y = currentPlayer->getPos()->y-GAME_HEIGHT/2;
    } else {
        if(cursor) {
            cursorPosition->x+=x;
            cursorPosition->y+=y;
            if(cursorPosition->x < 0) {
                cursorPosition->x = 0;
                viewMove(x, y, false);
            } else if(cursorPosition->x >= GAME_WIDTH) {
                cursorPosition->x = GAME_WIDTH-1;
                viewMove(x, y, false);
            }
            if(cursorPosition->y < 0) {
                cursorPosition->y = 0;
                viewMove(x, y, false);
            } else if(cursorPosition->y >= GAME_HEIGHT) {
                cursorPosition->y = GAME_HEIGHT-1;
                viewMove(x, y, false);
            }
        } else {
            viewPos->x+=viewMoveSpeed->x*x;
            viewPos->y+=viewMoveSpeed->y*y;
        }
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

    cursorTick = 0;
    updateView = true;
}

void MenuGame::drawTileAt(int i, int j) {
    bool inView = false;
    if(playerMode && currentPlayer != nullptr) {
        if(currentLevel->isInView(i, j, currentPlayer)) {
            currentLevel->setExplored(i, j, true);
            inView = true;
        }
        if(!currentLevel->getExplored(i, j)) {
            Ui::setColor(C_BLACK);
            addch(' ');
            return;
        }
    }
    Tile* tempTile = currentLevel->tileAt(i, j);

    if(currentLevel->inRange(i, j)) {
        if(currentLevel->displayEntityGrid[i][j] != nullptr) {


            if(inView) {
                Ui::setColor(currentLevel->displayEntityGrid[i][j]->getColorCode());
                if(unicodeSupport) {
                    addstr(currentLevel->displayEntityGrid[i][j]->getIcon(i, j, tick, currentLevel).c_str());
                } else {
                    addch(currentLevel->displayEntityGrid[i][j]->getIconAlt(i, j, tick, currentLevel));
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
        viewMove(0, -1, cursorMode);
        break;

    case KEY_DOWN:
    case 's':
        viewMove(0, 1, cursorMode);
        break;

    case KEY_LEFT:
    case 'a':
        viewMove(-1, 0, cursorMode);
        break;

    case KEY_RIGHT:
    case 'd':
        viewMove(1, 0, cursorMode);
        break;

    case 'W':
        viewMove(0, -5, cursorMode);
        break;

    case 'S':
        viewMove(0, 5, cursorMode);
        break;

    case 'A':
        viewMove(-5, 0, cursorMode);
        break;

    case 'D':
        viewMove(5, 0, cursorMode);
        break;

    case 'z':
        cursorMode = !cursorMode;
        updateView = true;
        break;

    case 'x':
        currentLevel->newEntity(new AiEntity(AiType::aiMoveRandom, "A", 'A', new Geometry::Point2(cursorPosition->x+viewPos->x, cursorPosition->y+viewPos->y), Ui::C_DARK_RED));
        break;

    case 'c':
        currentLevel->newEntity(new AiEntity(AiType::aiMoveRandom, "B", 'B', new Geometry::Point2(cursorPosition->x+viewPos->x, cursorPosition->y+viewPos->y), Ui::C_LIGHT_GREEN));
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
                drawTileAt(viewPos->x+i, viewPos->y+j);
            }
        }
    }

    if(cursorMode) {
        debugMessage+="Cursor Mode ";
        move(cursorPosition->y, cursorPosition->x);
        if(cursorTick%30<20) {
            addch('#');
        } else {
            drawTileAt(cursorPosition->x+viewPos->x, cursorPosition->y+viewPos->y);
        }
    }
    cursorTick++;

    Ui::setColor(C_WHITE);
    if(debugMessage.length() > 0) {
        move(TERMINAL_HEIGHT-2, 0);
        clrtobot();
        mvprintw(TERMINAL_HEIGHT-2, 0, debugMessage.c_str());
    }
    mvprintw(TERMINAL_HEIGHT-1, 0, "%d, %d  ms: %.2f  fps: %d  e: %d  %s", cursorPosition->x+viewPos->x, cursorPosition->y+viewPos->y, ms, fps, currentLevel->entityCount(), updateView?"draw":"");
    debugMessage = "";
    updateView = false;
    refresh();


}

}
