//
//  ItemGenerator.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Ui.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include "Utility.hpp"
#include "ItemGenerator.hpp"
#include "Art.hpp"
#include "Entity.hpp"
#include "ItemPotion.hpp"
#include "ItemTimeActivated.hpp"
#include "ItemSpecial.hpp"

namespace ItemGenerator {

    Item* iCoin;
    Item* iSmallKey;

    vector<ItemArmorBase> armorList;
    vector<Condition> conditionList;
    vector<WeaponBase> weaponList;
    vector<ScrollBase> scrollList;

    vector<int> potionChanceList;
    vector<PotionBase> potionList;

    WeaponBase wKnife;
    WeaponBase wSword;
    WeaponBase wAxe;
    WeaponBase wMace;
    WeaponBase wSpear;

    WeaponBase wBow;
    WeaponBase wCrossbow;

    WeaponBase wFireItemCombatSpell;
    WeaponBase wFrostItemCombatSpell;
    WeaponBase wShockItemCombatSpell;

    WeaponBase wHealingCombatSpell;

    WeaponBase wNatural;

    void initItemTemplates() {

        iCoin = new Item("Coin");
        iCoin->artIndex = Arts::artCoin;
        iCoin->coinValue = -1;

        iSmallKey = new Item("Key");
        iSmallKey->artIndex = Arts::artSmallKey;
        iSmallKey->coinValue = 1000;

        PotionBase potionHealth = atl(PotionBase({{"Health Potion"}}, {EffIdMeta(effHeal, 0)}, 0, 0, 5, 100), 100);
        PotionBase potionMana = atl(PotionBase({{"Mana Potion"}}, {EffIdMeta(effHeal, 1)}, 0, 0, 5, 100), 80);

        PotionBase potionRegen = atl(PotionBase({{"Regeneration Potion"}}, {EffIdMeta(effHeal, 0)}, 2, 30, 1, 10), 50);
        PotionBase potionManaRegen = atl(PotionBase({{"Mana Regeneration Potion"}}, {EffIdMeta(effHeal, 1)}, 2, 30, 1, 10), 40);

        PotionBase potionPhysicalDefense = atl(PotionBase({{"Physical Defense Potion"}}, {
            EffIdMeta(effBuffDefense, damPierce),
            EffIdMeta(effBuffDefense, damSharp),
        	EffIdMeta(effBuffDefense, damBlunt)
        }, 4, 60, .1, 1, false), 50);

        PotionBase potionElementalDefense = atl(PotionBase({{"Elemental Defense Potion"}}, {
            EffIdMeta(effBuffDefense, damFire),
            EffIdMeta(effBuffDefense, damIce),
            EffIdMeta(effBuffDefense, damShock)
        }, 2, 30, .1, 1, false), 25);

        PotionBase potionPhysicalAttack = atl(PotionBase({{"Physical Attack Potion"}}, {
            EffIdMeta(effBuffAttack, damPierce),
            EffIdMeta(effBuffAttack, damSharp),
            EffIdMeta(effBuffAttack, damBlunt)
        }, 4, 60, 1.1, 2, false), 50);

        PotionBase potionElementalAttacK = atl(PotionBase({{"Elemental Attack Potion"}}, {
            EffIdMeta(effBuffAttack, damFire),
            EffIdMeta(effBuffAttack, damIce),
            EffIdMeta(effBuffAttack, damShock)
        }, 2, 30, 1.1, 2, false), 25);

        //PotionBase potionPreventBad = atl(PotionBase({{"Purity Potion"}}, {EffIdMeta(effPurity, 0)}, 3, 30, 0, 0, false), 5);
        PotionBase potionRemoveBad = atl(PotionBase({{"Clensing Potion"}}, {EffIdMeta(effPurity, 0)}, 0, 0, 0, 0, false), 10);

        /*PotionBase potionOrganicDefense = atl(PotionBase({"Organic Defense Potion"}, {
            EffIdMeta(effBuffDefense, damPoison),
            EffIdMeta(effBuffDefense, damBlood)
        }, 2, 30, .1, 1, false));*/


        ScrollBase scrollRemoteUse = atl(ScrollBase({{"Scroll of Telekinesis", "Scrolls of Telekinesis"}}, spellRemoteUse));
        ScrollBase scrollRelocate = atl(ScrollBase({{"Scroll of Relocation", "Scrolls of Relocation"}}, spellRelocate));
        ScrollBase scrollBarrier = atl(ScrollBase({{"Scroll of Protection", "Scrolls of Protection"}}, spellBarrier));



        wKnife = atl(WeaponBase({{"Knife", "Knives"}, {"Dagger"}, {"Cleaver"}}, 0.25, 1.0/3, damSharp, wepMelee).setArts({Arts::artKnife, Arts::artDagger, Arts::artCleaver}));
        wSword = atl(WeaponBase({{"Longsword"}, {"Cutlass", "Cutlasses"}, {"Katana"}, {"Machete"}, {"Gladius", "Gladii"}, {"Scimitar"}, {"Rapier"}, {"Short Sword"}, {"Broadsword"}, {"Saber"}, {"Claymore"}}, 1, 1, damSharp, wepMelee).setArts({Arts::artLongsword, Arts::artCutlass, Arts::artKatana, Arts::artMachete, Arts::artGladius, Arts::artScimitar, Arts::artRapier, Arts::artShortSword, Arts::artBroadsword, Arts::artSaber, Arts::artClaymore}));
        wAxe = atl(WeaponBase({{"Axe"}, {"Hatchet"}, {"Double Axe"}}, 1.2, 1.2, damSharp, wepMelee).setArts({Arts::artAxe, Arts::artAxe, Arts::artDoubleAxe}));
        wMace = atl(WeaponBase({{"Mace"}, {"Club"}, {"Flail"}, {"War Hammer"}, {"Mallet"}}, 1.5, 1.5, damBlunt, wepMelee).setArts({Arts::artMace, Arts::artClub, Arts::artFlail, Arts::artWarHammer, Arts::artWarHammer}));
        wSpear = atl(WeaponBase({{"Spear"}, {"Halberd"}}, 1.3, 1.3, damPierce, wepMelee).setArts({Arts::artSpear, Arts::artHalberd}));

        wBow = atl(WeaponBase({{"Longbow"}, {"Bow"}, {"Recurve Bow"}}, .5, 1, damPierce, wepRanged).ranged(20).setArts({Arts::artLongbow, Arts::artLongbow, Arts::artRecurveBow}));
        wCrossbow = atl(WeaponBase({{"Crossbow"}, {"Scorpion"}}, 0.6, 0.8, damPierce, wepRanged).ranged(10).setArts({Arts::artCrossbow}));

        wFireItemCombatSpell = atl(WeaponBase({{"Ignite Spell"}, {"Scorch Spell"}, {"Burn Spell"}}, 1, .2, damFire, wepMagic).magical(8, 2).setArts({Arts::artScrollFire}));
        wFireItemCombatSpell.enchs.push_back(EnchantmentBase(effDamage, 6, 12, 1, 2, 5, 10, damFire));

        wFrostItemCombatSpell = atl(WeaponBase({{"Freeze Spell"}, {"Chill Spell"}}, 1, .2, damIce, wepMagic).magical(8, 2).setArts({Arts::artScrollFrost}));
        wFrostItemCombatSpell.enchs.push_back(EnchantmentBase(effDamage, 6, 12, 1, 2, 5, 10, damIce));

        wShockItemCombatSpell = atl(WeaponBase({{"Electrocute Spell"}, {"Shock Spell"}, {"Zap Spell"}}, 1, .2, damShock, wepMagic).magical(8, 2).setArts({Arts::artScrollShock}));
        wShockItemCombatSpell.enchs.push_back(EnchantmentBase(effDamage, 6, 12, 1, 2, 5, 10, damShock));

        wHealingCombatSpell = atl(WeaponBase({{"Healing Spell"}}, 0, .2, damNone, wepMagic).magical(8, 2).setArts({Arts::artScrollHeal}));
        wHealingCombatSpell.enchs.push_back(EnchantmentBase(effHeal, 1, 1, 10, 30, 0, 0));




        wNatural = WeaponBase({{"Teeth", "Teeth"}, {"Claws", "Claws"}}, 1, 1, damSharp, wepMelee);




        Condition cBroken = atl(Condition({"Broken", "Cracked", "Damaged"}, .5, .7, 1, 1, {wepMelee, wepRanged}));
        Condition cNormal = atl(Condition({"Standard", "Normal", "Regular", "Stock"}, 1, 1, 1, 1, {wepMelee, wepRanged}));
        Condition cForged = atl(Condition({"Forged", "Crafted"}, 1.4, 1.6, 1, 1, {wepMelee, wepRanged}));
        Condition cFortified = atl(Condition({"Fortified", "Strengthened", "Reinforced"}, 1.8, 2.2, 1.3, 1.6, {wepMelee, wepRanged}));
        Condition cHeavy = atl(Condition({"Heavy", "Dense"}, 1, 1.1, 1.3, 1.5, {wepMelee, wepRanged}));


        Condition cCheapMagic = atl(Condition({"Cheap", "Frugal"}, .8, 1.0, 1, 1, {wepMagic}).magical(.5, .8));
        Condition cNormalMagic = atl(Condition({"Apprentice"}, 1, 1, 1, 1, {wepMagic}).magical(1.0, 1.0));
        Condition cExpenciveMagic = atl(Condition({"Expensive", "Costly"}, 0.9, 1.0, 1, 1, {wepMagic}).magical(3.0, 5.0));
        Condition cMasterMagic = atl(Condition({"Master", "Expert", "Warlock's"}, 1.5, 2.5, 2, 3, {wepMagic}).magical(2.0, 4.0));



        ItemArmorBase aLeatherChest = atl(ItemArmorBase({{"Leather Cuirass", "Leather Cuirasses"}}, {slotBody}, {DefenseRange(damSharp, .01, .10), DefenseRange(damPierce, .01, .20), DefenseRange(damBlunt, .01, .05)}));

        ItemArmorBase aLeatherHelm = atl(ItemArmorBase({{"Leather Coif"}}, {slotHead}, {DefenseRange(damSharp, .01, .06), DefenseRange(damPierce, .01, .03)}));

        ItemArmorBase aLeatherLeggings = atl(ItemArmorBase({{"Leather Greaves", "Pairs of Leather Greaves"}}, {slotLegs}, {DefenseRange(damSharp, .01, .05), DefenseRange(damPierce, .01, .02)}));

        ItemArmorBase aLeatherBoots = atl(ItemArmorBase({{"Leather Boots", "Pairs of Leather Boots"}}, {slotFeet}, {DefenseRange(damSharp, .01, .04), DefenseRange(damPierce, .01, .02)}));

        ItemArmorBase aLeatherGloves = atl(ItemArmorBase({{"Leather Gloves", "Pairs of Leather Gloves"}, {"Leather Bracers", "Pairs of Leather Bracers"}, {"Leather Vambraces", "Pairs of Leather Vambraces"}}, {slotHands}, {DefenseRange(damSharp, .01, .03), DefenseRange(damPierce, .01, .02)}));



        ItemArmorBase aMailChest = atl(ItemArmorBase({{"Mail Shirt"}, {"Hauberk"}}, {slotBody}, {DefenseRange(damSharp, .01, .30), DefenseRange(damPierce, .01, .30), DefenseRange(damBlunt, .01, .10)}));

        ItemArmorBase aMailHelm = atl(ItemArmorBase({{"Mail Coif"}, {"Mail Hood"}}, {slotHead}, {DefenseRange(damSharp, .01, .15), DefenseRange(damPierce, .01, .15)}));

        ItemArmorBase aMailLeggings = atl(ItemArmorBase({{"Mail Leggings", "Pairs of Mail Leggings"}}, {slotLegs}, {DefenseRange(damSharp, .01, .10), DefenseRange(damPierce, .01, .10)}));

        ItemArmorBase aMailBoots = atl(ItemArmorBase({{"Mail Socks", "Pairs of Mail Socks"}}, {slotFeet}, {DefenseRange(damSharp, .01, .08), DefenseRange(damPierce, .01, .08)}));

        ItemArmorBase aMailGloves = atl(ItemArmorBase({{"Mail Gloves", "Pairs of Mail Gloves"}, {"Mail Gauntlets", "Pairs of Mail Gauntlets"}}, {slotHands}, {DefenseRange(damSharp, .01, .08), DefenseRange(damPierce, .01, .08)}));




        ItemArmorBase aGoldenRing = atl(ItemArmorBase({{"Golden Ring"}, {"Silver Ring"}}, {slotFinger1, slotFinger2, slotFinger3, slotFinger4}, {DefenseRange(damFire, .01, .10, 10), DefenseRange(damIce, .01, .10, 10), DefenseRange(damShock, .01, .10, 10)}));

        ItemArmorBase aJewelRing = atl(ItemArmorBase({{"Golden Garnet Ring"}, {"Silver Amethyst Ring"}, {"Silver Garnet Ring"}, {"Golden Amethyst Ring"}}, {slotFinger1, slotFinger2, slotFinger3, slotFinger4}, {DefenseRange(damFire, .01, .20, 10), DefenseRange(damIce, .01, .20, 10), DefenseRange(damShock, .01, .20, 10)}));


    }

    void cleanupItemTemplates() {
        delete iCoin;
    }

    ItemArmorBase atl(ItemArmorBase a) {
        armorList.push_back(a);
        return a;
    }

    Condition atl(Condition c) {
        conditionList.push_back(c);
        return c;
    }

    WeaponBase atl(WeaponBase w) {
        weaponList.push_back(w);
        return w;
    }

    PotionBase atl(PotionBase p, int of100) {
        potionList.push_back(p);
        potionChanceList.push_back(of100);
        return p;
    }

    ScrollBase atl(ScrollBase s) {
        scrollList.push_back(s);
        return s;
    }

    WeaponBase getRandWeaponBase(const WeaponType w, const DamageType d) {
        while (true) {
            size_t i = (rand()) % weaponList.size();
            if (weaponList[i].weaponType == w && weaponList[i].damageType) {
                return weaponList[i];
            }
        }
        return weaponList[0];
    }

    WeaponBase getRandWeaponBase(const WeaponType w) {
        while (true) {
            size_t i = (rand()) % weaponList.size();
            if (weaponList[i].weaponType == w) {
                return weaponList[i];
            }
        }
        return weaponList[0];
    }

    WeaponBase getRandWeaponBase() {
        size_t i = (rand()) % weaponList.size();
        return weaponList[i];
    }

    ItemArmorBase getRandArmorBase() {
        size_t i = (rand()) % armorList.size();
        return armorList[i];
    }

    vector<Item*> createRandLoots(int difficulty, int goldMaxQty, int wepMaxQty, int armMaxQty, int altMaxQty, int keyChance) {
        vector<Item*> items;

        if (goldMaxQty > 0) {
            Item* item = Item::clone(iCoin);
            item->qty = (rand() % (goldMaxQty))+1;
            items.push_back(item);
        }

        if (keyChance > 0 && rand() % keyChance == 0) {
            Item* item = Item::clone(iSmallKey);
            items.push_back(item);
        }

        if (wepMaxQty > 0) {
            int weaponQty = (rand() % (wepMaxQty + 1));

            for (int i = 0; i < weaponQty; i++) {

                int itemDifficulty = 0;

                for (int i = 0; i < difficulty; i++) {
                    if (rand() % 10 == 0) {
                        itemDifficulty++;
                    }
                }

                ItemWeapon* w = createRandItemWeapon(itemDifficulty);
                if(rand() % 10 == 0 && w->enchantments.size() == 0){
                    DamageType damType = Random::choose<DamageType>(5, damBlood, damPoison, damFire, damIce, damShock);
                    Enchantment e = Enchantment(effDamage, (rand()%10)+10, Random::randDouble(1, 5), Random::randDouble(2, 30), damType);
                    w->addEnchantment(e);
                }
                w->coinValue = calculateItemValue(w);
                items.push_back(w);
            }
        }

        if(armMaxQty > 0){
            int armorQty = (rand() % (armMaxQty + 1));

            for(int i = 0;i<armorQty;i++){

                int itemDifficulty = 0;

                for (int i = 0; i < difficulty; i++) {
                    if (rand() % 10 == 0) {
                        itemDifficulty++;
                    }
                }

                ItemArmor* a = createRandArmor(itemDifficulty);
                a->coinValue = calculateItemValue(a);
                items.push_back(a);
            }
        }

        if (altMaxQty > 0) {

            int altQty = (rand() % (altMaxQty + 1));

            for (int i = 0; i < altQty; i++) {

                int itemDifficulty = 0;

                for (int i = 0; i < difficulty; i++) {
                    if (rand() % 10 == 0) {
                        itemDifficulty++;
                    }
                }

                Item* r = createRandAltLoot(itemDifficulty);
                r->coinValue = calculateItemValue(r);
                items.push_back(r);
            }

        }

        return items;
    }

    ItemPotion* createPotionFromBase(PotionBase pb, int itemDifficulty) {
        vector<Effect> effects;
        double time = Random::randDouble(pb.time.x, pb.time.y);
        double power = Random::randDouble(pb.power.x, pb.power.y);
        for (EffIdMeta eff : pb.effects) {
            effects.push_back(Effect(eff.id, time, power * (pb.difficultyScales?itemDifficulty+1:1), eff.meta));
        }
        ItemPotion* potion = new ItemPotion(effects, "");
        potion->setName(pb.names[rand() % pb.names.size()]);
        switch (rand() % 4) {
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

    ItemUtilitySpell* createScrollFromBase(ScrollBase sb) {
        ItemUtilitySpell* utilitySpell = new ItemUtilitySpell(sb.eff, -1, "");
        utilitySpell->setName(sb.names[rand() % sb.names.size()]);
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

    Item* createRandAltLoot(int itemDifficulty) {
        int r = rand() % 8;
        if(r < 4) {// 1/2 for potion
            PotionBase pb;
            int i;
            do{
                i = rand() % potionList.size();
            }while(rand()%100 <= potionChanceList[i]);
            
            pb = potionList[i];

            return createPotionFromBase(pb, itemDifficulty);
        } else if(r < 6) {// 1/4 for scroll
            ScrollBase sb = scrollList[rand() % scrollList.size()];
            return createScrollFromBase(sb);
        } else {// 1/4 for bomb
            return new ItemTimeActivated(rand()%10==0?timeActivatedBomb:timeActivatedWallBomb, Random::randDouble(10, 20), Random::randDouble(50, 500), Random::randDouble(2, 10));
        }
    }

    ItemWeapon* createItemWeaponFromType(WeaponType w, DamageType d, int itemDifficulty) {
        return createItemWeaponFromBase(getRandWeaponBase(w, d), itemDifficulty);
    }

    ItemWeapon* createItemWeaponFromType(WeaponType w, int itemDifficulty) {
        return createItemWeaponFromBase(getRandWeaponBase(w), itemDifficulty);
    }

    ItemWeapon* createItemWeaponFromBase(WeaponBase base, int itemDifficulty) {
        size_t ni = rand() % base.names.size();
        size_t arti = min(ni, base.arts.size() - 1);

        ItemWeapon* w;

        if (base.range != -1) {
            if (base.manaCost != -1) {
                w = new ItemCombatSpell();
                ((ItemCombatSpell*) w)->manaCost = (int) (base.manaCost * Random::randDouble(1.0, 2.0));
                //w->addEnchantment(Enchantment(effDamage, 10, 1, 6, base.damageType));
            } else {
                w = new ItemRanged();
            }
            ((ItemRanged*) w)->range = base.range;
        } else {
            w = new ItemWeapon();
        }
        if(w->durability != INFINITY){
            w->durability = (rand()%400)+50;
        }
        w->baseDamage = base.damage;
        w->useDelay = base.useDelay;
        w->setName(base.names[ni]);
        w->artIndex = base.arts[arti];
        w->damageType = base.damageType;
        w->weaponType = base.weaponType;
        for(EnchantmentBase e :  base.enchs){
            w->enchantments.push_back(createEnchantmentFromBase(e));
        }

        w->baseDamage *= (itemDifficulty * .1) + 1;
        w->baseDamage *= Random::randDouble(4.5, 5.5);


        return w;
    }
    
    Enchantment createEnchantmentFromBase(EnchantmentBase base){
        return Enchantment(base.effect, Random::randDouble(base.chance.x, base.chance.y), Random::randDouble(base.power.x, base.power.y), Random::randDouble(base.time.x, base.time.y), base.meta);
    }

    ItemArmor* createItemArmorFromBase(ItemArmorBase base, int itemDifficulty){
        size_t ni = rand() % base.names.size();

        ItemArmor* a;

        a = new ItemArmor();

        a->setName(base.names[ni]);
        a->viableSlots = base.viableSlots;
        for(DefenseRange dr : base.defences){
            if(rand() % dr.chance == 0){
                Defense def = Defense(dr.damageType, Random::randDouble(dr.amountMin, dr.amountMax));
                def.amount *= (itemDifficulty * .1) + 1;
                a->defenses.push_back(def);
            }
        }

        return a;
    }

    ItemWeapon* applyConditionToItemWeapon(ItemWeapon* w, Condition c, int itemDifficulty, bool prependName) {

        if (prependName) {
            w->prependName(c.names[rand() % c.names.size()] + " ");
        }

        w->baseDamage *= Random::randDouble(c.damage.x, c.damage.y);
        w->useDelay *= Random::randDouble(c.useDelay.x, c.useDelay.y);
        ItemCombatSpell* s = dynamic_cast<ItemCombatSpell*> (w);
        if (s) {
            s->manaCost *= Random::randDouble(c.manaCost.x, c.manaCost.y);
        }

        return w;
    }

    ItemWeapon* applyRandConditionToItemWeapon(ItemWeapon * w, int itemDifficulty, bool prependName) {
        Condition condition;
        do {
            condition = conditionList[rand() % conditionList.size()];
        } while (!Utility::vectorContains(condition.applicableWeaponTypes, w->weaponType));

        applyConditionToItemWeapon(w, condition, itemDifficulty);
        return w;
    }

    ItemWeapon* createRandItemWeapon(int itemDifficulty) {
        WeaponType wType;
        if (rand() % 3 == 0) {
            if (rand() % 3 == 0) {
                wType = wepMagic; // 1/9 chance
            } else {
                wType = wepRanged; // 2/9 chance
            }
        } else {
            wType = wepMelee; // 2/3 chance
        }
        WeaponBase base = getRandWeaponBase(wType);
        ItemWeapon* w = createItemWeaponFromBase(base, itemDifficulty);

        switch (wType) {
            case wepMelee:
                w->minimumAbilities.list[iSTR] = pow(2, itemDifficulty) - 1;
                break;
            case wepRanged:
                w->minimumAbilities.list[iDEX] = pow(2, itemDifficulty) - 1;
                break;
            case wepMagic:
                w->minimumAbilities.list[iINT] = pow(2, itemDifficulty) - 1;
                break;
        }

        applyRandConditionToItemWeapon(w, itemDifficulty);

        return w;
    }

    ItemArmor* createRandArmor(int itemDifficulty) {
        ItemArmorBase base = getRandArmorBase();
        ItemArmor* w = createItemArmorFromBase(base, itemDifficulty);

        w->minimumAbilities.list[iAGI] = pow(2, itemDifficulty) - 1;

        //applyRandConditionToItemArmor(w, itemDifficulty);
        //todo

        return w;
    }

    Item* makeCoins(int qty) {
        if (qty > 0) {
            Item* c = Item::clone(iCoin);
            c->qty = qty;
            return c;
        }
        return nullptr;
    }


    int calculateItemValue(Item* item){
        if(item->equalsExceptQty(iCoin)){
            return -1;
        }
        double value = 0;
        ItemEquipable* ie = dynamic_cast<ItemEquipable*>(item);
        ItemPotion* ip = dynamic_cast<ItemPotion*>(item);
        ItemSpecial* is = dynamic_cast<ItemSpecial*>(item);
        ItemTimeActivated* it = dynamic_cast<ItemTimeActivated*>(item);
        ItemUtilitySpell* iu = dynamic_cast<ItemUtilitySpell*>(item);
        if(ie){

            value += 5;

            ItemWeapon* iw = dynamic_cast<ItemWeapon*>(item);
            ItemArmor* ia = dynamic_cast<ItemArmor*>(item);
            if(iw){

                value += (iw->baseDamage * iw->useDelay) * 2;

                ItemRanged* ir = dynamic_cast<ItemRanged*>(item);
                if(ir){

                    value += ir->range * 4;

                    ItemCombatSpell* ic = dynamic_cast<ItemCombatSpell*>(item);
                    if(ic){//Combat Spell
                        value += Math::max(100.0 - ic->manaCost, 1.0) * 5;
                    }else{//Ranged Weapon

                    }

                }else{//Weapon

                }

                for(Enchantment e : iw->enchantments){
                    double eValue = 0;
                    switch(e.effectId){
                        case effDamage:{
                            eValue += e.power;
                        }
                        default:{
                            eValue += e.power / 3;
                        }
                    }
                    eValue += (50 - e.chance) * .25;
                    eValue += (e.time) * .25;
                    value += eValue * .2;
                }

            }else if(ia){//Armor

                value += 5;

                for(Defense d : ia->defenses){
                    double eValue = d.amount * 2;
                    eValue += 5;
                    value += eValue;
                }

                for(Enchantment e : ia->enchantments){
                    double eValue = 0;
                    switch(e.effectId){
                        default:{
                            eValue = e.power * .8;
                        }
                    }
                    value += eValue * .2;
                }

            }else{//Equipable

            }
        }else if(ip){//Potion

            value += 10;


            for(Effect e : ip->effects){
                double eValue = 0;
                if(e.timeLeft < 1){
                    eValue += e.power;
                }else{
                    eValue += e.timeLeft * e.power * .8;
                }
                value += eValue;
            }

        }else if(is){//Special

            switch (is->specialty) {
                case specialtyCompass:{
                    value = -1;
                }
                default:{
                    value = -1;
                }
            }

        }else if(it){//Time Activated

            switch (it->timeActivatedType) {
                case timeActivatedWallBomb:
                    value += 20;
                    //no break intentional
                case timeActivatedBomb:
                    value += it->radius * it->power;
                    break;
                case timeActivatedDud:
                    value = -1;
                    break;


                default:
                    break;
            }

        }else if(iu){//Utility Spell

            switch (iu->spellEffect) {
                case spellRemoteUse:
                    value = 50;
                    break;
                case spellRelocate:
                    value = 200;
                    break;
                case spellBarrier:
                    value = 150;
                    break;
                default:
                    value = -1;
                    break;
            }

        }else{//Item
            value = item->coinValue;
        }
        return (int)value;
    }


}
