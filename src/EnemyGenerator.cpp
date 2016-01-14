//
//  EnemyGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 3/4/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "EnemyGenerator.hpp"
#include "Level.hpp"
#include "EntityItem.hpp"


namespace EnemyGenerator {

    vector<EntityBase*> enemyWeightList;

    int total = 0;

    EntityBase* goblinScout;
    EntityBase* goblinWarrier;
    EntityBase* goblinArcher;
    EntityBase* troll;
    EntityBase* wraith;
    EntityBase* drake;
    EntityBase* slime;
    EntityBase* myconid;
    EntityBase* snake;

    EntityBase* mimic;
    EntityBase* bunny;
    EntityBase* rat;

    void initEnemies() {

        using namespace ItemGenerator;


        goblinScout = atl(new EntityBase(100, "Goblin Scout", 'g', aiAttackAndFleeAtLowHealth, 50, ItemGenerator::wKnife, "", C_LIGHT_GREEN, 1.0));
        goblinScout->weaknesses.push_back(Weakness(damPoison, 2));
        goblinScout->lootProfileIndex = atl(new LootProfile(true, false, {make_pair(10, wKnife), make_pair(10, wAxe)}));


        goblinWarrier = atl(new EntityBase(50, "Goblin Warrior", 'w', aiAttack, 75, ItemGenerator::wSword, "", C_DARK_GREEN, 1.0));
        goblinWarrier->weaknesses.push_back(Weakness(damPoison, 2));
        goblinWarrier->lootProfileIndex = atl(new LootProfile(true, false, {make_pair(10, wSword), make_pair(10, wSpear)}));


        goblinArcher = atl(new EntityBase(50, "Goblin Archer", 'a', aiAttackAndFleeAtLowHealth, 25, ItemGenerator::wBow, "", C_LIGHT_GREEN, 1.0));
        goblinArcher->weaknesses.push_back(Weakness(damPoison, 2));
        goblinArcher->weaknesses.push_back(Weakness(damBlunt, 1.25));
        goblinArcher->moveDelay = 1.2;
        goblinArcher->lootProfileIndex = atl(new LootProfile(true, false, {make_pair(10, wBow), make_pair(10, wCrossbow)}));


        ItemGenerator::WeaponBase* snakeWeapon = new ItemGenerator::WeaponBase(ItemGenerator::wNatural);
        snakeWeapon->damageType = damPierce;
        snakeWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effDamage, 20, 30, 1, 1, 10, 20, damPoison));
        snake = atl(new EntityBase(15, "Serpant", 's', aiAttack, 75, snakeWeapon, "Fangs", C_LIGHT_BLUE, 1.0));
        snake->attackMultiplier *= .75;
        snake->lootProfileIndex = atl(new LootProfile(true, true, {}));


		ItemGenerator::WeaponBase* trollWeapon = new ItemGenerator::WeaponBase(ItemGenerator::wMace);
        trollWeapon->damage *= 1.2;
        trollWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effDamage, 20, 25, 1, 5, 10, 20, damIce));
        troll = atl(new EntityBase(10, "Troll", 't', aiAttack, 100, trollWeapon, "Cold Mace", C_LIGHT_RED, 1.5, 1));
        troll->weaknesses.push_back(Weakness(damFire, 4));
        troll->moveDelay = 1.8;
        troll->lootProfileIndex = atl(new LootProfile(true, false, {make_pair(10, wMace)}));
        troll->maxLootDrop = 2;


        ItemGenerator::WeaponBase* wraithWeapon = new ItemGenerator::WeaponBase(ItemGenerator::wAxe);
        wraithWeapon->damage *= 2;
        wraithWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effDamage, 10, 20, 5, 10, 5, 20, damBlood));
        wraith = atl(new EntityBase(5, "Wraith", 'W', aiAttack, 200, wraithWeapon, "Bloody Double Axe", C_DARK_GRAY, 2.0, 2));
        wraith->weaknesses.push_back(Weakness(damFire, 2));
        wraith->weaknesses.push_back(Weakness(damIce, 2));
        wraith->weaknesses.push_back(Weakness(damShock, 2));
        wraith->weaknesses.push_back(Weakness(damSharp, .5));
        wraith->weaknesses.push_back(Weakness(damPierce, .5));
        wraith->weaknesses.push_back(Weakness(damBlunt, .5));
        wraith->moveDelay = 1.8;
        wraith->lootProfileIndex = atl(new LootProfile(true, true, {make_pair(20, wFireItemCombatSpell), make_pair(20, wFrostItemCombatSpell), make_pair(20, wShockItemCombatSpell), make_pair(20, wHealingCombatSpell)}));
        wraith->maxLootDrop = 4;

        ItemGenerator::WeaponBase* drakeWeapon = new ItemGenerator::WeaponBase(ItemGenerator::wNatural);
        drakeWeapon->damage *= 3;
        drakeWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effDamage, 5, 10, 10, 15, 5, 10, damFire));
        drakeWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effDamage, 30, 50, 10, 15, 20, 30, damFire));
        drake = atl(new EntityBase(1, "Drake", 'D', aiAttack, 500, drakeWeapon, "Teeth", C_LIGHT_RED, 2.0, 2));
        drake->weaknesses.push_back(Weakness(damSharp, .1));
        drake->weaknesses.push_back(Weakness(damBlunt, .1));
        drake->weaknesses.push_back(Weakness(damPierce, .1));
        drake->weaknesses.push_back(Weakness(damIce, 2));
        drake->moveDelay = .75;
        drake->solidity = tileFlagSolidInAir;
        drake->lootProfileIndex = atl(new LootProfile(true, true, {make_pair(10, wFireItemCombatSpell), make_pair(10, wFrostItemCombatSpell), make_pair(10, wShockItemCombatSpell), make_pair(10, wHealingCombatSpell)}));
        drake->maxLootDrop = 10;


        slime = atl(new EntityBase(30, "Slime", 's', aiAttack, 40, ItemGenerator::wNatural, "Goo", C_LIGHT_YELLOW, 1.0));
        slime->weaknesses.push_back(Weakness(damSharp, .25));
        slime->weaknesses.push_back(Weakness(damPierce, .25));
        slime->lootProfileIndex = atl(new LootProfile(true, false));


        ItemGenerator::WeaponBase* myconidWeapon = new ItemGenerator::WeaponBase(ItemGenerator::wNatural);
        myconidWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effLSD, 20, 30, 0, 0, 20, 40));
        myconidWeapon->enchs.push_back(new ItemGenerator::EnchantmentBase(effMemory, 20, 30, 0, 0, 50, 100));
        myconid = atl(new EntityBase(20, "Myconid", 'm', aiAttack, 50, myconidWeapon, "Finger", C_LIGHT_MAGENTA, 1.0));
        myconid->weaknesses.push_back(Weakness(damFire, 4));
        myconid->attackMultiplier *= .5;
        myconid->lootProfileIndex = atl(new LootProfile(true, true));



        mimic = new EntityBase(0, "Mimic", 'M', aiAttack | aiMoveRandom, 75, ItemGenerator::wNatural, "Teeth", C_LIGHT_GREEN, 0);
        mimic->weaknesses.push_back(Weakness(damFire, 4));
        mimic->lootProfileIndex = atl(new LootProfile(true, false));


        bunny = new EntityBase(0, "Bunny", 'b', aiFlee | aiMoveRandom, 10, ItemGenerator::wNatural, "Claws", C_LIGHT_WHITE, 0);
        bunny->moveDelay = 0.4;
        LootProfile* lp = new LootProfile(false, false, {make_pair(1, easterEgg)});
        lp->canBeModifiedByRandomness = false;
        lp->allowCoins = false;
        bunny->lootProfileIndex = atl(lp);
        bunny->minLootDrop = 1;


        rat = atl(new EntityBase(10, "Rat", 'r', aiFlee | aiMoveRandom, 8, ItemGenerator::wNatural, "Teeth", C_LIGHT_GRAY, 0));
        rat->moveDelay = 0.5;
        




    }

    /*void addStandardDrops(EntityBase* b, double rarityMult, bool magical){
        b->drops.emplace_back((int)(20*rarityMult), ItemGenerator::potionHealth);
        b->drops.emplace_back((int)(40*rarityMult), ItemGenerator::potionRegen);
        b->drops.emplace_back((int)(45*rarityMult), ItemGenerator::potionRemoveBad);
        b->drops.emplace_back((int)(50*rarityMult), ItemGenerator::potionPhysicalAttack);
        b->drops.emplace_back((int)(50*rarityMult), ItemGenerator::potionPhysicalDefense);
        if(magical){
            b->drops.emplace_back((int)(20*rarityMult), ItemGenerator::potionMana);
            b->drops.emplace_back((int)(40*rarityMult), ItemGenerator::potionManaRegen);
            b->drops.emplace_back((int)(50*rarityMult), ItemGenerator::potionElementalAttack);
            b->drops.emplace_back((int)(50*rarityMult), ItemGenerator::potionElementalDefense);
        }
        b->drops.emplace_back((int)(40*rarityMult), ItemGenerator::scrollRelocate);
        b->drops.emplace_back((int)(80*rarityMult), ItemGenerator::scrollRemoteUse);
        b->drops.emplace_back((int)(40*rarityMult), ItemGenerator::scrollBarrier);
        b->drops.emplace_back((int)(20*rarityMult), ItemGenerator::repairHammer);
    }*/

    void cleanupEnemies() {
        for (EntityBase* we : enemyWeightList) {
            delete we;
        }
        enemyWeightList.clear();
    }

    EntityBase* atl(EntityBase* we) {
        enemyWeightList.push_back(we);
        return we;
    }

    void setIntervals(int difficulty) {
        total = 0;
        for (int i = 0; i < (int)enemyWeightList.size(); i++) {
            total += enemyWeightList[i]->weight * enemyWeightList[i]->difficultyWeightMulti * (difficulty + 1);
            enemyWeightList[i]->interval = total;
        }
    }

    EntityAi* makeEntity(EntityBase* we, int difficulty) {
        EntityAi* e = new EntityAi(we->name, we->ai, we->icon, Point2Zero, we->color, we->maxHp);
        e->weaknesses = we->weaknesses;
        e->moveDelay = we->moveDelay;
        e->attackMultiplier = we->attackMultiplier;
        e->lootProfileIndex = we->lootProfileIndex;
        e->maxLootDrop = we->maxLootDrop;
        e->minLootDrop = we->minLootDrop;
        e->solidity = we->solidity;
        ItemWeapon* weapon = ItemGenerator::createItemWeaponFromBase(we->weaponBase, difficulty + we->weaponDifficultyAdd);
        if (we->weaponName.size() > 0) {
            weapon->getName(false) = we->weaponName;
        }
        e->setActiveItemWeapon(weapon);
        return e;
    }

    EntityAi* makeRandomEntity(int difficulty) {

        int r = rand() % total;
        EntityBase* last = enemyWeightList[0];
        for (EntityBase* we : enemyWeightList) {
            if (we->interval > r) {
                last = we;
                break;
            }
            //debugf("r: %d  total: %d  we's interval: %d", r, total, we->interval);
        }
        return makeEntity(last, difficulty);
    }



    

}
