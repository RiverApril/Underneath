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

    class MenuGame : public Menu{
    public:

        int GAME_WIDTH = 0;
        int GAME_HEIGHT = 0;

        MenuGame();

        ~MenuGame();

        void openUi();
        void handleInput(int in);
        void update();

        void setGameAreaSize();

        void viewMove(int x, int y, bool cursor);
        void drawTileAt(int i, int j);

        bool paused = false;
        bool updateView;
        Geometry::Point2* viewPos;
        Geometry::Point2* viewMoveSpeed;
        Geometry::Point2* cursorPosition;
        bool cursorMode = false;
        bool playerMode = true;
        int cursorTick = 0;
        Level* currentLevel;
        Player* currentPlayer;

    };
}

#endif /* defined(__Underneath__UiMenuGame__) */
