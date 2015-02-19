//
//  MenuGame.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UiMenuGame__
#define __Underneath__UiMenuGame__

#include "Menu.h"
#include "Geometry.h"
#include "World.h"
#include "Level.h"
#include "Player.h"

namespace Ui {

    class MenuGame : public Menu {
    public:

        Point2 gameArea;
        Point2 borderSize = Point2(24, 4);

        MenuGame(string worldName, Abilities<int> playerAbilities);

        ~MenuGame();

        bool init(string worldName, Abilities<int> playerAbilities);

        bool openUi();
        void closeUi();
        void handleInput(int in);
        void update();

        void render(double displayTime);

        void setGameAreaSize();

        void viewUpdate();
        void drawTileAt(Point2 p);
        void arrowMove(Point2 p);
		
        double timePassed = 0;
        Point2 viewPos;
        Point2 viewMoveSpeed;

        bool initSuccess;

        World* currentWorld;
        yesNo* saveAnswer = new yesNo(aUndefined);
        int* useItem = new int(-1);

        Point2 targetPosition = Point2Zero;


        int modePlayerControl = 0;
        int modeAdjustBorder = 1;
        int modeInterectChoose = 2;
        int modeSelectTarget = 3;
        int mode = modePlayerControl;

    };
}

#endif /* defined(__Underneath__UiMenuGame__) */
