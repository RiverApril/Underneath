//
//  MenuPlayerSetup.h
//  Underneath
//
//  Created by Braeden Atlee on 12/15/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__MenuPlayerSetup__
#define __Underneath__MenuPlayerSetup__

#include "Menu.h"
#include "Abilities.h"

using namespace AbilityDetails;

namespace Ui {
    class MenuPlayerSetup : public Menu {
    public:

        MenuPlayerSetup(string name) : Menu() {
            this->name = name;
            for(int i=0;i<abilityCount;i++){
                abilityPoints[i] = 0;
            }
        }

        ~MenuPlayerSetup() {

        }

        bool openUi();
        void closeUi();
        void handleInput(int in);
        void update();

        void renderAbility(int y, int xOffset, int sel, int points, string name);

        string name;

        int selection = 0;

        int abilityPoints[abilityCount];

        static const int totalPoints = abilityCount*2;
        int remainingPoints = totalPoints;
        const int pointMax = 6;

        int* addOne(int a[abilityCount]){
            for(int i=0;i<abilityCount;i++){
                a[i]++;
            }
            return a;
        }

    private:

        static const int selContinue = abilityCount+0;
        static const int selBack = abilityCount+1;

        static const int maxUiSelection = selBack;
    };
}

#endif /* defined(__Underneath__MenuPlayerSetup__) */
