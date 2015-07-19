//
//  MenuGame.h
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__UiMenuGame__
#define __Underneath__UiMenuGame__

#include "Menu.hpp"
#include "Geometry.hpp"
#include "World.hpp"
#include "Level.hpp"
#include "EntityPlayer.hpp"

namespace Ui {

    class MenuGame : public Menu {
    public:

        Point2 gameArea;
        Point2 borderSize = Point2(24, 4);

        MenuGame(string worldName, Abilities<int> playerAbilities);

        ~MenuGame();

        Icon* spinyIcon;

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
        Entity* targetEntity = nullptr;

        double itemToBeUsedRange = -1;
        Item* itemToBeUsed = nullptr;

        vector<Entity*> nearestEntities;

#define modeEntityPlayerControl 0
#define modeAdjustBorder 1
#define modeSelectDirection 2
#define modeSelectPosition 3
#define modeSelectEntity 4
        int controlMode = modeEntityPlayerControl;

        void changeMode(int newMode);

    };
}

#endif /* defined(__Underneath__UiMenuGame__) */
