//
//  Offers.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/14/17.
//  Copyright (c) 2017 Braeden Atlee. All rights reserved.
//

#include "Offers.hpp"
#include "EntityPlayer.hpp"
#include "MenuGame.hpp"
#include "MenuDialog.hpp"
#include "MenuMessage.hpp"

namespace Offers{

    vector<Offer*> offers;


    void Offer::presentOffering(Ui::MenuGame* menuGame) {
        if(this->usedUp){
            menuGame->openMenu(new Ui::MenuMessage("The alter breaks apart as soon as you touch it."));
            return;
        }
        
        menuGame->rumble = 50;
        timeout(fastTimeout);
        while(menuGame->rumble > 0){
            menuGame->render(menuGame->currentWorld->worldTime);
            refresh();
        }
        timeout(defaultTimeout);
        
        vector<string> message = {"*You hear a deep voice from deep below you...*",
                                  "I see great potentoial in you, I offer you a proposal...", ""};
        message.insert(message.end(), dialog.begin(), dialog.end());
        menuGame->openMenu(new Ui::MenuDialog(message, {"Accept", "Decline"}, [this, menuGame](Ui::MenuDialog* menu, int result){
            this->onAnswer(this, menuGame->currentWorld->currentPlayer, result==0);
            menu->closeThisMenu();
            if(this->onlyOnce && result==0){
                this->usedUp = true;
            }
        }, false));
    }

    void addOffer(Offer* offer){
        offer->index = (int)offers.size();
        offers.push_back(offer);
    }

    void initOffers(){
        addOffer(new Offer({"I will merge your health and mana pools forever.", "However, your maximum reserves will be cut in half."},
                           [](Offer* offer, EntityPlayer* player, bool accepted){
            if(accepted){
                player->changeSpecial(Special::specialHalfHpMp, true);
                player->changeSpecial(Special::specialPoolHpMp, true);
            }
        }));
        addOffer(new Offer({"I will disable your ability to heal over time.", "However, you will absorb an enemy's maximum health when killed."},
                           [](Offer* offer, EntityPlayer* player, bool accepted){
            if(accepted){
               player->changeSpecial(Special::specialDisableRegen, true);
               player->changeSpecial(Special::specialHealOnKills, true);
            }
       }));
    }

}
