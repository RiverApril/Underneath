//
//  MenuGame.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UiMenuGame__
#define __Underneath__UiMenuGame__

#include "Ui.h"
#include "Geometry.h"
#include "World.h"
#include "Level.h"
#include "Player.h"

namespace Ui {

    class MenuGame : public Menu {
    public:

        Point2 gameArea;
        Point2 borderSize = Point2(24, 4);

        MenuGame(std::string worldName);

        ~MenuGame();

        bool init(std::string worldName);

        void openUi(Menu* oldMenu);
        void closeUi(Menu* newMenu);
        void handleInput(int in);
        void update();

        void setGameAreaSize();

        void viewUpdate();
        void drawTileAt(Point2 p);
        void arrowMove(int x, int y);

        bool execute(std::string commandRaw);
		
        bool updateView;
        bool timePassed;
        Point2* viewPos;
        Point2* viewMoveSpeed;

        World* currentWorld;
        Level* currentLevel;
        Player* currentPlayer;
        std::string input = "";

        int modePlayerControl = 0;
        int modeAdjustBorder = 1;
        int modeConsoleInput = 2;
        int mode = modePlayerControl;

    };
}

#endif /* defined(__Underneath__UiMenuGame__) */
