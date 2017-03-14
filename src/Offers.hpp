//
//  Offers.hpp
//  Underneath
//
//  Created by Braeden Atlee on 3/14/17.
//  Copyright (c) 2017 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__Offers__
#define __Underneath__Offers__

#include "Global.hpp"

namespace Ui {
    class MenuGame;
}
class EntityPlayer;

namespace Offers{

    class Offer{
    public:

        Offer(vector<string> dialog, function<void(Offer*, EntityPlayer*, bool)> onAnswer){
            this->dialog = dialog;
            this->onAnswer = onAnswer;
        }

        ~Offer(){}

        void presentOffering(Ui::MenuGame* menuGame);

        int index;
        bool onlyOnce = true;
        vector<string> dialog;
        function<void(Offer*, EntityPlayer*, bool)> onAnswer;
        
        //Must be saved:
        bool usedUp = false;

    };

    void initOffers();
    void addOffer(Offer* offer);

    extern vector<Offer*> offers;
}

#endif //__Underneath__Offers__
