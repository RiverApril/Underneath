//
//  UiMenuGame.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UiMenuGame__
#define __Underneath__UiMenuGame__

#include "Ui.h"
#include "Geometry.h"
#include "Level.h"
#include "Player.h"

namespace Ui {

class MenuGame : public Menu {
public:

    int GAME_WIDTH = 0;
    int GAME_HEIGHT = 0;

    MenuGame();

    ~MenuGame();

    void openUi();
    void handleInput(int in);
    void update();

    void setGameAreaSize();

    void viewMove(Point2 p);
    void drawTileAt(Point2 p);

    bool paused = false;
    bool updateView;
    Point2* viewPos;
    Point2* viewMoveSpeed;
    bool playerMode = true;
    bool debugMode = true;
    Level* currentLevel;
    Player* currentPlayer;

};
}

#endif /* defined(__Underneath__UiMenuGame__) */
