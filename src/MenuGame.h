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

        MenuGame(std::string worldName);

        ~MenuGame();

        bool init(std::string worldName);

        void openUi();
        void handleInput(int in);
        void update();

        void setGameAreaSize();

        void viewUpdate();
        void drawTileAt(Point2 p);

        bool paused = false;
        bool updateView;
        Point2* viewPos;
        Point2* viewMoveSpeed;
        bool playerMode = true;
        World* currentWorld;
        Level* currentLevel;
        Player* currentPlayer;

    };
}

#endif /* defined(__Underneath__UiMenuGame__) */
