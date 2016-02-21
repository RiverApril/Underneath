//
//  MenuInv.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuInv.hpp"
#include "Math.hpp"
#include "EntityItem.hpp"
#include "Controls.hpp"
#include "Utility.hpp"
#include "Art.hpp"

namespace Ui {

    MenuInv::MenuInv(World* w, int* useItem) : Menu() {
        this->currentWorld = w;
        this->useItem = useItem;
        *useItem = -1;
    }

    void MenuInv::handleInput(int in) {
        if (in == Key::uiUp) {
            selected--;
        } else if (in == Key::uiDown) {
            selected++;
        } else if (in == Key::drop) {
            if ((int)currentWorld->currentPlayer->inventory.size() > 0 && selected < (int)currentWorld->currentPlayer->inventory.size()) {
                Item* drop;
                if (currentWorld->currentPlayer->inventory[selected]->qty == 1) {
                    drop = currentWorld->currentPlayer->inventory[selected];
                    currentWorld->currentPlayer->removeItem(currentWorld->currentPlayer->inventory[selected], false);
                } else {
                    currentWorld->currentPlayer->inventory[selected]->qty -= 1;
                    drop = Item::clone(currentWorld->currentPlayer->inventory[selected]);
                    drop->qty = 1;
                }
                currentWorld->currentLevel->newEntity(new EntityItem(drop, currentWorld->currentPlayer->pos));
            }
        } else if (in == Key::equip) {
            ItemEquipable* equipable = dynamic_cast<ItemEquipable*> (currentWorld->currentPlayer->inventory[selected]);
            if (equipable) {
                if(!currentWorld->currentPlayer->equipItem(equipable)){
                    flashImportantInfo = flashTimeMax;
                }
            }else{
                *useItem = selected;
                closeThisMenu();
            }
        } else if (in == Key::secondaryAttack) {
            ItemEquipable* equipable = dynamic_cast<ItemEquipable*> (currentWorld->currentPlayer->inventory[selected]);
            if (equipable) {
                if(equipable->canBeEquipedHere(slotWep2)){
                    if(!currentWorld->currentPlayer->equipItem(equipable, slotWep2)){
                        flashImportantInfo = flashTimeMax;
                    }
                }
            }
        } else if (in == Key::interact) {
            *useItem = selected;
            closeThisMenu();
            return;
        } else if (in == KEY_ESCAPE || in == Key::inventory) {
            closeThisMenu();
            return;
        }else if (in == Key::sortInv){
            sort(currentWorld->currentPlayer->inventory.begin(), currentWorld->currentPlayer->inventory.end(), comparePointer<Item>);
            return;
        }else{

            EquipSlot slot = slotNone;
            if(in == Key::fav1){
                slot = slotFav1;
            }else if(in == Key::fav2){
                slot = slotFav2;
            }else if(in == Key::fav3){
                slot = slotFav3;
            }else if(in == Key::fav4){
                slot = slotFav4;
            }else if(in == Key::fav5){
                slot = slotFav5;
            }else if(in == Key::fav6){
                slot = slotFav6;
            }else if(in == Key::fav7){
                slot = slotFav7;
            }else if(in == Key::fav8){
                slot = slotFav8;
            }else if(in == Key::fav9){
                slot = slotFav9;
            }else if(in == Key::fav0){
                slot = slotFav0;
            }

            if(slot >= 0){
                currentWorld->currentPlayer->equipItem(currentWorld->currentPlayer->inventory[selected], slot);
            }
        }
    }

    void MenuInv::update() {
        if (selected < 0) {
            selected = (int) currentWorld->currentPlayer->inventory.size() - 1;
        }
        if (selected >= (int) currentWorld->currentPlayer->inventory.size()) {
            selected = 0;
        }

        if(currentWorld->currentPlayer){
			Ui::drawInventory(currentWorld, currentWorld->currentPlayer, selected, nullptr, currentWorld->currentPlayer->getName()+"'s Inventory", "", true, false, flashImportantInfo>0);
        }

        flashImportantInfo--;

        /*setColor(C_WHITE);
        int minI = max(0, scrollOffset);
        int maxI = player->inventory.size() - scrollOffset;
        move(0, 0);
        clrtobot();
        int totalWeight = 0;
        forVector(player->inventory, i){
            totalWeight += player->inventory[i]->weight * player->inventory[i]->qty;
        }
        mvprintw(0, 0, "Inventory   Total Weight: %-3d", totalWeight);

        setColor(C_BLACK, C_WHITE);
                mvhline(selected+3, 0, ' ', terminalSize.x);
        setColor(C_WHITE, C_BLACK);

        mvprintw(1, columnPrefixChar, "");mvvline(1, columnPrefixChar-1, '|', terminalSize.y);
        mvprintw(1, columnName, "Item");mvvline(1, columnName-1, '|', terminalSize.y);
        mvprintw(1, columnQty-3, "Qty");//mvvline(1, columnQty-1, '|', terminalSize.y);
        //mvprintw(1, columnWeight, "Wgt");mvvline(1, columnWeight-1, '|', terminalSize.y);
        //mvprintw(1, columnHitPoints, "Hp");mvvline(1, columnHitPoints-1, '|', terminalSize.y);
        mvprintw(1, columnInfo, "Information");mvvline(1, columnInfo-1, '|', terminalSize.y);

        mvhline(2, 0, '-', terminalSize.x);
        Item* item;
        ItemWeapon* weapon;
        ItemRanged* ranged;
        ItemCombatSpell* spell;
        int y = 3;
        for(int i=minI;i<maxI;i++){
            item = player->inventory[i];
            weapon = dynamic_cast<ItemWeapon*>(item);
            ranged = dynamic_cast<ItemRanged*>(item);
            spell = dynamic_cast<ItemCombatSpell*>(item);
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(y, columnPrefixChar, "%c", item == player->getActiveItemWeapon()?'E':' ');
            mvprintw(y, columnName, item->getExtendedName().c_str());
            mvprintw(y, columnQty-4, "%4s", (to_string(item->qty)).c_str());
            //mvprintw(y, columnWeight+1, "%2.1f", item->weight);

            //if(weapon){
                //mvprintw(y, columnHitPoints+1, "%3.1f", weapon->baseDamage);
            //}
            if(i == selected){
                setColor(C_WHITE, C_BLACK);
            }

            mvhline(y, columnInfo, ' ', terminalSize.x-columnInfo);

            y++;
        }

        item = player->inventory[selected];

        if(item){
            weapon = dynamic_cast<ItemWeapon*>(item);
            ranged = dynamic_cast<ItemRanged*>(item);
            spell = dynamic_cast<ItemCombatSpell*>(item);

            int a = 3;

            setColor(C_DARK_GRAY, C_BLACK);
            if(item->artIndex > -1){
                debug("draw art");
                Art* art = Arts::getArt(item->artIndex);
                Point2 artSize = art->getSize();
                art->printAt(terminalSize - artSize - Point2One);
            }else{
                debug("did not draw art");
            }
            setColor(C_WHITE, C_BLACK);

            mvprintw(a++, columnInfo, "Name: %s", item->getExtendedName().c_str());
            if(item == player->getActiveItemWeapon()){
                mvprintw(terminalSize.y-1, columnInfo, "-Equipped");
            }else{
                mvprintw(terminalSize.y-1, columnInfo, "");
            }

            setColor(C_LIGHT_GRAY, C_BLACK);

            mvprintw(a++, columnInfo, "Qty: %d", item->qty);
            mvprintw(a++, columnInfo, "Weight: %.2f", item->weight);
            if(weapon){
                mvprintw(a++, columnInfo, "Damage: %.2f", weapon->baseDamage);
                mvprintw(a++, columnInfo, "Type: %s", ItemWeapon::damageTypeToString(weapon->damageType).c_str());
                mvprintw(a++, columnInfo, "Use Delay: %.2f (d/t: %.2f)", weapon->useDelay, (weapon->baseDamage/weapon->useDelay));
                if(ranged){
                    mvprintw(a++, columnInfo, "Range: %.2f", ranged->range);
                    if(spell){
                        mvprintw(a++, columnInfo, "Mana Cost: %d", spell->manaCost);
                    }
                }
                if(weapon->enchantments.size() > 0){
                    a++;
                        mvprintw(a++, columnInfo, "Enchantments: ");
                    for(Enchantment e : weapon->enchantments){
                        mvprintw(a++, columnInfo, "   %s x%d (%d)", ItemWeapon::enchantmentIdToString(e.eId).c_str(), e.power, e.chance);
                    }
                    a++;
                }
            }

        }*/

    }
}
