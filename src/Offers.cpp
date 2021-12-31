//
//  Offers.cpp
//  Underneath
//
//  Created by Emily Atlee on 3/14/17.
//  Copyright (c) 2017 Emily Atlee. All rights reserved.
//

#include "Offers.hpp"
#include "EntityPlayer.hpp"
#include "MenuGame.hpp"
#include "MenuDialog.hpp"
#include "MenuMessage.hpp"


void Offer::presentOffering(Ui::MenuGame* menuGame) {
    if(this->usedUp){
        menuGame->openMenu(new Ui::MenuMessage("The alter breaks apart as soon as you touch it."));
        return;
    }
    
    menuGame->rumble = 125;
    while(menuGame->rumble > 25){
        menuGame->render(menuGame->currentWorld->worldTime);
        refresh();
    }
    
    vector<string> message = {"*You hear a voice from deep below you...*",
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

void Offers::addOffer(Offer* offer){
    offer->index = (int)list.size();
    list.push_back(offer);
}

Offers::Offers(){
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
