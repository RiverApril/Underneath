//
//  Command.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Command.hpp"
#include "EntityAlive.hpp"
#include "MenuGame.hpp"
#include "Utility.hpp"
#include "ItemGenerator.hpp"



namespace Commands {


    vector<Command*> commandList;

    bool execute(string commandRaw, Menu* currentMenu) {

        auto firstSpacePos = commandRaw.find_first_of(' ');
        string argumentsRaw = (firstSpacePos == string::npos) ? "" : commandRaw.substr(firstSpacePos + 1);
        string commandName = commandRaw.substr(0, firstSpacePos);

        vector<string> arguments;
        string s;
        size_t lastI = 0;
        for (size_t i = 0; i < argumentsRaw.length(); i++) {
            if (argumentsRaw[i] == ' ') {
                s = argumentsRaw.substr(lastI, i - lastI);
                if (s.length() > 0) {
                    arguments.push_back(s);
                }
                lastI = i + 1;
            }
        }
        s = argumentsRaw.substr(lastI);
        if (s.length() > 0) {
            arguments.push_back(s);
        }

        if (!currentMenu->menuOnlyExecute(commandName, arguments, argumentsRaw)) {
            for (Command* c : commandList) {
                if (c->acceptableName(commandName)) {
                    c->execute(commandName, arguments, argumentsRaw, currentMenu);
                    break;
                }
            }

        }

        return false;
    }

    struct CommandHelp : Command {

        string help() {
            return "Provides information about a command.";
        }

        string usage() {
            return "help [command]";
        }

        string defaultName() {
            return "help";
        }

        bool acceptableName(string name) {
            return Command::acceptableName(name) || name.compare("?") == 0;
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {

            switch (arguments.size()) {
                case 0:
                {
                    console("Global Commands:");
                    for (size_t i = 0; i < commandList.size(); i++) {
                        console("- " + commandList[i]->usage());
                    }
                    string s = currentMenu->menuOnlyCommands();
                    if (s.length() > 0) {
                        console("Menu Commands:");
                        console(s);
                    }
                    return true;
                }

                case 1:
                {
                    for (size_t i = 0; i < commandList.size(); i++) {
                        if (commandList[i]->acceptableName(arguments[0])) {
                            console(commandList[i]->usage() + "  " + commandList[i]->help());
                            return true;
                        }
                    }
                    string s = currentMenu->menuOnlyCommandHelp(arguments[0]);
                    if (s.length() > 0) {
                        console(s);
                    } else {
                        console("Unknown Command: " + arguments[0]);
                    }
                    return false;
                }

                default:
                    return false;
            }
        }
    };

    struct CommandEcho : Command {

        string help() {
            return "Echos what you input.";
        }

        string usage() {
            return "echo [words]";
        }

        string defaultName() {
            return "echo";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            if (argumentsRaw.length() > 0) {
                console(argumentsRaw);
                return true;
            }
            return false;
        }
    };

    struct CommandDebug : Command {

        string help() {
            return "Toggles debug messages.";
        }

        string usage() {
            return "debug";
        }

        string defaultName() {
            return "debug";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            if (argumentsRaw.length() == 0) {
                printDebugEnabled = !printDebugEnabled;
                console("Debug " + string(printDebugEnabled ? "Enabled" : "Disabled") + ".");
                return true;
            }
            return false;
        }
    };

    struct CommandEffect : Command {

        string help() {
            return "Applies an effect to the player.";
        }

        string usage() {
            return "effect <id> [duration] [power] [meta]";
        }

        string defaultName() {
            return "effect";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            MenuGame* mg = dynamic_cast<MenuGame*> (currentMenu);
            if (mg) {
                Effect e = Effect(effDamage, 10, 1);
                if (arguments.size() > 0) {
                    e.eId = Utility::parseInt(arguments[0].c_str());
                    if (arguments.size() > 1) {
                        e.timeLeft = Utility::parseInt(arguments[1].c_str());
                    }
                    if (arguments.size() > 2) {
                        e.power = Utility::parseInt(arguments[2].c_str());
                    }
                    if (arguments.size() > 3) {
                        e.meta = Utility::parseDouble(arguments[3].c_str());
                    }
                    mg->currentWorld->currentPlayer->addEffect(e);
                    console("Applied Effect: " + e.toString());
                    return true;
                }
            } else {
                console("Must be ingame to apply.");
            }
            return false;
        }
    };

    struct CommandXp : Command {

        string help() {
            return "Gives the player XP";
        }

        string usage() {
            return "xp <amount>";
        }

        string defaultName() {
            return "xp";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            MenuGame* mg = dynamic_cast<MenuGame*> (currentMenu);
            if (mg) {
                if (arguments.size() == 1) {
                    int amount = Utility::parseInt(arguments[0]);
                    mg->currentWorld->currentPlayer->gainXp(amount);
                    console("Added " + to_string(amount) + "XP");
                    return true;
                } else {
                    console("Impropper use of command.");
                }
            } else {
                console("Must be ingame to add.");
            }
            return false;
        }
    };

    struct CommandWep : Command {

        string help() {
            return "Gives a weapon to the player";
        }

        string usage() {
            return "wep <weaponType> <difficulty>";
        }

        string defaultName() {
            return "wep";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            MenuGame* mg = dynamic_cast<MenuGame*> (currentMenu);
            if (mg) {
                if (arguments.size() == 2) {
                    int wepType = Utility::parseInt(arguments[0]);
                    int diff = Utility::parseInt(arguments[1]);
                    Item* i = ItemGenerator::createItemWeaponFromType(wepType, diff);
                    mg->currentWorld->currentPlayer->addItem(i);
                    consolef("ItemWeapon given");
                    return true;
                } else {
                    console("Impropper use of command.");
                }
            } else {
                console("Must be ingame to add.");
            }
            return false;
        }
    };

    struct CommandNextKey : Command {

        string help() {
            return "Waits for a key and gives info about it";
        }

        string usage() {
            return "nextkey";
        }

        string defaultName() {
            return "nextkey";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            setColor(C_WHITE);
            move(0, 0);
            addstr("Waiting for key...");
            int in;
            do{
            	in = getch();
            }while(in == ERR);
            consolef("Key pressed: int(%d)  char(%c)", (char) in, in);
            currentMenu->update();
            return true;
        }
    };

    struct CommandGodMode : Command {

        string help() {
            return "Toggles God Mode";
        }

        string usage() {
            return "godmode";
        }

        string defaultName() {
            return "godmode";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            godMode = !godMode;
            consolef("God Mode %s", godMode?"Enabled":"Disabled");
            return true;
        }
    };

    struct CommandClear : Command {

        string help() {
            return "Clears the console log";
        }

        string usage() {
            return "clear";
        }

        string defaultName() {
            return "clear";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            consoleBuffer.clear();
            return true;
        }
    };

    struct CommandDebugTools : Command {
        string help() {
            return "Gives debug tools";
        }

        string usage() {
            return "tools";
        }

        string defaultName() {
            return "tools";
        }

        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu) {
            MenuGame* mg = dynamic_cast<MenuGame*> (currentMenu);
            if (mg) {
                
                ItemUtilitySpell* i = new ItemUtilitySpell(spellDebugPlaceWall, 0, "DEBUG Wall wand", 0);
                i->continuousUse = true;
                mg->currentWorld->currentPlayer->addItem(i);

                i = new ItemUtilitySpell(spellDebugPlaceFloor, 0, "DEBUG Floor wand", 0);
                i->continuousUse = true;
                mg->currentWorld->currentPlayer->addItem(i);

                i = new ItemUtilitySpell(spellDebugPlaceGoblin, 0, "DEBUG Goblin wand", 0);
                i->continuousUse = true;
                mg->currentWorld->currentPlayer->addItem(i);

                i = new ItemUtilitySpell(spellRelocate, 0, "DEBUG Move wand", 0);
                i->continuousUse = true;
                mg->currentWorld->currentPlayer->addItem(i);


                consolef("Tools given");
                return true;
            } else {
                console("Must be ingame to use.");
            }
            return false;
            return true;
        }
    };

    void initCommands() {
        commandList.push_back(new CommandHelp());
        commandList.push_back(new CommandEcho());
        commandList.push_back(new CommandDebug());
        commandList.push_back(new CommandEffect());
        commandList.push_back(new CommandXp());
        commandList.push_back(new CommandWep());
        commandList.push_back(new CommandNextKey());
        commandList.push_back(new CommandGodMode());
        commandList.push_back(new CommandClear());
        commandList.push_back(new CommandDebugTools());
    }

    void cleanupCommands() {
        for (Command* c : commandList) {
            delete c;
        }
        commandList.clear();
    }

}
