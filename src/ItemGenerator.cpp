//
//  ItemGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "Random.h"
#include "Utility.h"
#include "ItemGenerator.h"
#include "Art.h"
#include "Entity.h"
#include "Potion.h"
#include "UtiitySpell.h"

namespace ItemGenerator {

    Item* iCoin;


    vector<Condition> conditionList;
    vector<WeaponBase> weaponList;
    vector<PotionBase> potionList;
    vector<ScrollBase> scrollList;

    WeaponBase wKnife;
    WeaponBase wSword;
    WeaponBase wAxe;
    WeaponBase wMace;
    WeaponBase wSpear;

    WeaponBase wBow;
    WeaponBase wCrossbow;

    WeaponBase wFireCombatSpell;
    WeaponBase wFrostCombatSpell;
    WeaponBase wShockCombatSpell;

    WeaponBase wNatural;

    void initItemTemplates(){

        iCoin = new Item("Coin", .01);
        iCoin->artIndex = Arts::artCoin;

        PotionBase potionHealth = atl(PotionBase({"Health Potion"}, {effHeal}, 0, 0, 1, 100, 0));
        PotionBase potionMana = atl(PotionBase({"Mana Potion"}, {effHeal}, 0, 0, 1, 100, 1));

        PotionBase potionRegen = atl(PotionBase({"Regeneration Potion"}, {effHeal}, 2, 20, 1, 10, 0));
        PotionBase potionManaRegen = atl(PotionBase({"Mana Regeneration Potion"}, {effHeal}, 2, 20, 1, 10, 1));


        ScrollBase scrollRemoteUse = atl(ScrollBase({"Scroll of Telekinesis"}, spellRemoteUse));
        ScrollBase scrollRelocate = atl(ScrollBase({"Scroll of Relocation"}, spellRelocate));




        wKnife = atl(WeaponBase({"Knife", "Dagger", "Cleaver"}, 0.25, 0.5, damSharp, wepMelee).setWeight(1).setArts({Arts::artKnife, Arts::artDagger, Arts::artCleaver}));
        wSword = atl(WeaponBase({"Longsword", "Cutlass", "Katana", "Machete", "Gladius", "Scimitar", "Rapier", "Short Sword", "Broadsword", "Saber", "Claymore"}, 1, 1, damSharp, wepMelee).setWeight(2).setArts({Arts::artLongsword, Arts::artCutlass, Arts::artKatana, Arts::artMachete, Arts::artGladius, Arts::artScimitar, Arts::artRapier, Arts::artShortSword, Arts::artBroadsword, Arts::artSaber, Arts::artClaymore}));
        wAxe = atl(WeaponBase({"Axe", "Hatchet", "Double Axe"}, 1.2, 1.2, damSharp, wepMelee).setWeight(2).setArts({Arts::artAxe, Arts::artAxe, Arts::artDoubleAxe}));
        wMace = atl(WeaponBase({"Mace", "Club", "Flail", "War Hammer", "Mallet"}, 1.5, 1.5, damBlunt, wepMelee).setArts({Arts::artMace, Arts::artClub, Arts::artFlail, Arts::artWarHammer, Arts::artWarHammer}));
        wSpear = atl(WeaponBase({"Spear", "Halberd"}, 1.3, 1.3, damPierce, wepMelee).ranged(1.8).setWeight(2.5).setArts({Arts::artSpear, Arts::artHalberd}));

        wBow = atl(WeaponBase({"Longbow", "Bow", "Recurve Bow"}, .5, 1, damPierce, wepRanged).ranged(20).setWeight(1.5).setArts({Arts::artLongbow, Arts::artLongbow, Arts::artRecurveBow}));
        wCrossbow = atl(WeaponBase({"Crossbow", "Scorpion"}, .6, 0.8, damPierce, wepRanged).ranged(10).setWeight(2).setArts({Arts::artCrossbow}));

        wFireCombatSpell = atl(WeaponBase({"Ignite", "Scorch", "Burn"}, 1, .1, damFire, wepMagic).magical(8, 1).setWeight(.1).setArts({Arts::artScrollFire}));
        wFrostCombatSpell = atl(WeaponBase({"Freeze", "Chill"}, 1, .1, damIce, wepMagic).magical(8, 1).setWeight(.1).setArts({Arts::artScrollFrost}));
        wShockCombatSpell = atl(WeaponBase({"Electrocute", "Shock", "Zap"}, 1, .1, damShock, wepMagic).magical(8, 1).setWeight(.1).setArts({Arts::artScrollShock}));


        wNatural = atl(WeaponBase({"Teeth", "Claws"}, 1, 1, damSharp, wepMelee));


        Condition cBroken = atl(Condition({"Broken", "Cracked", "Damaged"}, .5, .7, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cNormal = atl(Condition({"Standard", "Normal", "Regular", "Stock"}, 1, 1, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cForged = atl(Condition({"Forged", "Crafted"}, 1.4, 1.6, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cFortified = atl(Condition({"Fortified", "Strengthened", "Reinforced"}, 1.8, 2.2, 1.1, 1.1, 1.3, 1.6, {wepMelee, wepRanged}));
        Condition cHeavy = atl(Condition({"Heavy", "Overweight", "Obese", "Dense"}, 1, 1.1, 1.3, 1.5, 2, 3, {wepMelee, wepRanged}));


        Condition cCheapMagic = atl(Condition({"Cheap", "Frugal"}, .8, 1.0, 1, 1, 1, 1, {wepMagic}).magical(.5, .8));
        Condition cNormalMagic = atl(Condition({"Apprentice"}, 1, 1, 1, 1, 1, 1, {wepMagic}).magical(1.0, 1.0));
        Condition cExpenciveMagic = atl(Condition({"Expencive", "Constly"}, 0.9, 1.0, 1, 1, 1, 1, {wepMagic}).magical(3.0, 5.0));
        Condition cMasterMagic = atl(Condition({"Master", "Expert", "Warlock's"}, 1.5, 2.5, 2, 3, 1, 1, {wepMagic}).magical(2.0, 4.0));


    }

    void cleanupItemTemplates(){
        delete iCoin;
    }

    Condition atl(Condition c){
        conditionList.push_back(c);
        return c;
    }

    WeaponBase atl(WeaponBase w){
        weaponList.push_back(w);
        return w;
    }

    PotionBase atl(PotionBase p){
        potionList.push_back(p);
        return p;
    }

    ScrollBase atl(ScrollBase s){
        scrollList.push_back(s);
        return s;
    }

    WeaponBase getRandWeaponBase(const WeaponType w){
        while(true){
            size_t i = (rand())%weaponList.size();
            if(weaponList[i].weaponType == w){
                return weaponList[i];
            }
        }

    }

    WeaponBase getRandWeaponBase(){
        size_t i = (rand())%weaponList.size();
        return weaponList[i];
    }

    vector<Item*> createRandLoots(int difficulty, int goldMaxQty, int wepMaxQty, int altMaxQty){
        vector<Item*> items;

        if(goldMaxQty > 0){
            Item* item = Item::clone(iCoin);
            item->qty = rand()%(goldMaxQty+1);
            items.push_back(item);
        }

        if(wepMaxQty > 0){
            int weaponQty = (rand()%(wepMaxQty+1));

            for (int i = 0; i < weaponQty; i++){

                int itemDifficulty = 0;

                for(int i=0;i<difficulty;i++){
                    if(rand()%10 == 0){
                        itemDifficulty++;
                    }
                }

                items.push_back(createRandWeapon(itemDifficulty));
            }
        }

        if(altMaxQty > 0){

            int altQty = (rand()%(altMaxQty+1));

            for (int i = 0; i < altQty; i++){

                int itemDifficulty = 0;

                for(int i=0;i<difficulty;i++){
                    if(rand()%10 == 0){
                        itemDifficulty++;
                    }
                }

                 items.push_back(createRandAltLoot(itemDifficulty));
            }

        }

        return items;
    }

    Item* createRandAltLoot(int itemDifficulty){
        switch (rand()%2) {
            default:
            case 0:{
                PotionBase pb = potionList[rand()%potionList.size()];
                vector<Effect> effects;
                double time = Random::randDouble(pb.time.x, pb.time.y);
                double power = Random::randDouble(pb.power.x, pb.power.y);
                for(EffectId effId : pb.effIds){
                    effects.push_back(Effect(effId, time, power * itemDifficulty));
                }
                Potion* potion = new Potion(effects, pb.names[rand()%pb.names.size()], .2);
                switch (rand()%4) {
                    default:
                    case 0:
                        potion->artIndex = Arts::artPotion1;
                        break;
                    case 1:
                        potion->artIndex = Arts::artPotion2;
                        break;
                    case 2:
                        potion->artIndex = Arts::artPotion3;
                        break;
                    case 3:
                        potion->artIndex = Arts::artPotion4;
                        break;
                }
                return potion;
            }

            case 1:
                ScrollBase sb = scrollList[rand()%scrollList.size()];
                UtilitySpell* utilitySpell = new UtilitySpell(sb.eff, -1, sb.names[rand()%sb.names.size()], .1);
                switch (sb.eff) {
                    case spellRelocate:
                        utilitySpell->artIndex = Arts::artScrollPerson;
                        break;

                    case spellRemoteUse:
                        utilitySpell->artIndex = Arts::artScrollHand;
                        break;

                    default:
                        utilitySpell->artIndex = Arts::artScroll;
                        break;
                }
                return utilitySpell;
        }
    }

    Weapon* createWeaponFromType(WeaponType w, int itemDifficulty){
        return createWeaponFromBase(getRandWeaponBase(w), itemDifficulty);
    }

    Weapon* createWeaponFromBase(WeaponBase base, int itemDifficulty){
        size_t ni = rand()%base.names.size();
        size_t arti = min(ni, base.arts.size()-1);

        Weapon* w;

        if(base.range != -1){
            if(base.manaCost != -1){
                w = new CombatSpell();
                ((CombatSpell*)w)->manaCost = (int)(base.manaCost * Random::randDouble(4.0, 12.0));
                w->addEnchantment(Enchantment(effDamage, 10, 1, 6).setMeta(base.damageType));
            }else{
                w = new Ranged();
            }
            ((Ranged*)w)->range = base.range;
        }else{
            w = new Weapon();
        }
        w->baseDamage = base.damage;
        w->useDelay = base.useDelay;
        w->weight = base.weight;
        w->name = base.names[ni];
        w->artIndex = base.arts[arti];
        w->damageType = base.damageType;
        w->weaponType = base.weaponType;

        w->baseDamage *= ((itemDifficulty) * .5)+1;
        w->baseDamage *= Random::randDouble(4.5, 5.5);

        return w;
    }

    Weapon* applyConditionToWeapon(Weapon* w, Condition c, int itemDifficulty, bool prependName){

        if(prependName){
            w->name = c.names[rand()%c.names.size()] + " " + w->name;
        }

        w->baseDamage *= Random::randDouble(c.damage.x, c.damage.y);
        w->useDelay *= Random::randDouble(c.useDelay.x, c.useDelay.y);
        w->weight *= Random::randDouble(c.weight.x, c.weight.y);
        CombatSpell* s = dynamic_cast<CombatSpell*>(w);
        if(s){
            s->manaCost *= Random::randDouble(c.manaCost.x, c.manaCost.y);
        }

        return w;
    }

    Weapon* applyRandConditionToWeapon(Weapon * w, int itemDifficulty, bool prependName){
        Condition condition;
        do{
            condition = conditionList[rand()%conditionList.size()];
        }while(!Utility::vectorContains(condition.applicableWeaponTypes, w->weaponType));

        applyConditionToWeapon(w, condition, itemDifficulty);
        return w;
    }

    Weapon* createRandWeapon(int itemDifficulty){
        WeaponType wType;
        if(rand()%3==0){
            if(rand()%3==0){
                wType = wepMagic; // 1/9 chance
            }else{
                wType = wepRanged; // 2/9 chance
            }
        }else{
            wType = wepMelee; // 2/3 chance
        }
        WeaponBase base = getRandWeaponBase(wType);
        Weapon* w = createWeaponFromBase(base, itemDifficulty);

        switch (wType) {
            case wepMelee:
                w->minimumAbilities.list[iSTR] = pow(2, itemDifficulty)-1;
                break;
            case wepRanged:
                w->minimumAbilities.list[iDEX] = pow(2, itemDifficulty)-1;
                break;
            case wepMagic:
                w->minimumAbilities.list[iINT] = pow(2, itemDifficulty)-1;
                break;
        }

        applyRandConditionToWeapon(w, itemDifficulty);

        return w;
    }

    Item* makeCoins(int qty){
        if(qty > 0){
            Item* c = Item::clone(iCoin);
            c->qty = qty;
            return c;
        }
        return nullptr;
    }


}
