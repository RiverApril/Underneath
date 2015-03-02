//
//  Command.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/25/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Command.h"
#include "Alive.h"
#include "MenuGame.h"
#include "Utility.h"


namespace Commands{


    vector<Command*> commandList;


    bool execute(string commandRaw, Menu* currentMenu){

        auto firstSpacePos = commandRaw.find_first_of(' ');
        string argumentsRaw = (firstSpacePos==string::npos)?"":commandRaw.substr(firstSpacePos+1);
        string commandName = commandRaw.substr(0, firstSpacePos);

        vector<string> arguments;
        string s;
        size_t lastI = 0;
        for(size_t i = 0;i<argumentsRaw.length();i++){
            if(argumentsRaw[i] == ' '){
                s = argumentsRaw.substr(lastI, i-lastI);
                if(s.length() > 0){
                    arguments.push_back(s);
                }
                lastI = i+1;
            }
        }
        s = argumentsRaw.substr(lastI);
        if(s.length() > 0){
            arguments.push_back(s);
        }

        if(!currentMenu->menuOnlyExecute(commandName, arguments, argumentsRaw)){
            for(Command* c : commandList){
                if(c->acceptableName(commandName)){
                    c->execute(commandName, arguments, argumentsRaw, currentMenu);
                    break;
                }
            }

        }

        return false;
    }

    struct CommandHelp : Command{
        string help(){
            return "Provides information about a command.";
        }
        string usage(){
            return "help [command]";
        }
        string defaultName(){
            return "help";
        }
        bool acceptableName(string name){
            return Command::acceptableName(name) || name.compare("?") == 0;
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){

            switch(arguments.size()){
                case 0:{
                    console("Global Commands:");
                    for(size_t i=0;i<commandList.size();i++){
                        console("- "+commandList[i]->usage());
                    }
                    string s = currentMenu->menuOnlyCommands();
                    if(s.length() > 0){
                        console("Menu Commands:");
                        console(s);
                    }
                    return true;
                }

                case 1:{
                    for(size_t i=0;i<commandList.size();i++){
                        if(commandList[i]->acceptableName(arguments[0])){
                            console(commandList[i]->usage() + "  " + commandList[i]->help());
                            return true;
                        }
                    }
                    string s = currentMenu->menuOnlyCommandHelp(arguments[0]);
                    if(s.length() > 0){
                        console(s);
                    }else{
                        console("Unknown Command: "+arguments[0]);
                    }
                    return false;
                }

                default:
                    return false;
            }
        }
    };

    struct CommandEcho : Command{
        string help(){
            return "Echos what you input.";
        }
        string usage(){
            return "echo [words]";
        }
        string defaultName(){
            return "echo";
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            if(argumentsRaw.length() > 0){
                console(argumentsRaw);
                return true;
            }
            return false;
        }
    };

    struct CommandDebug : Command{
        string help(){
            return "Toggles debug messages.";
        }
        string usage(){
            return "debug";
        }
        string defaultName(){
            return "debug";
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            if(argumentsRaw.length() == 0){
                printDebugEnabled = !printDebugEnabled;
                console("Debug "+string(printDebugEnabled?"Enabled":"Disabled")+".");
                return true;
            }
            return false;
        }
    };

    struct CommandEffect : Command{
        string help(){
            return "Applies an effect to the player.";
        }
        string usage(){
            return "effect <id> [duration] [power]";
        }
        string defaultName(){
            return "effect";
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            MenuGame* mg = dynamic_cast<MenuGame*>(currentMenu);
            if(mg){
                Effect e = Effect(effBleed, mg->currentWorld->worldTime+10, 1, mg->currentWorld->worldTime);
                if(arguments.size() > 0){
                    e.eId = ParsingUtility::parseInt(arguments[0].c_str());
                    if(arguments.size() > 1){
                        e.timeEnd = mg->currentWorld->worldTime+ParsingUtility::parseInt(arguments[1].c_str());
                    }
                    if(arguments.size() > 2){
                        e.power = ParsingUtility::parseInt(arguments[2].c_str());
                    }
                    mg->currentWorld->currentPlayer->addEffect(e);
                    debug("Applied Effect: "+e.toString());
                    return true;
                }
            }else{
                debug("Must be ingame to apply.");
            }
            return false;
        }
    };

    struct CommandXp : Command{
        string help(){
            return "Gives the player XP in the given ability.";
        }
        string usage(){
            return "xp <skill> <amount>";
        }
        string defaultName(){
            return "effect";
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            MenuGame* mg = dynamic_cast<MenuGame*>(currentMenu);
            if(mg){
                if(arguments.size() == 2){
                    int ability = ParsingUtility::parseInt(arguments[0]);
                    int amount = ParsingUtility::parseInt(arguments[1]);
                    if(ability < abilityCount){
                        debug("Not an ability index. Must be smaller than "+to_string(abilityCount)+".");
                        return false;
                    }else{
                        mg->currentWorld->currentPlayer->gainXp(ability, amount);
                        debug("Added "+to_string(amount)+"XP to "+abilityNames[ability]);
                        return true;
                    }
                }else{
                    debug("Impropper use of command.");
                }
            }else{
                debug("Must be ingame to add.");
            }
            return false;
        }
    };
    
    void initCommands(){
        commandList.push_back(new CommandHelp());
        commandList.push_back(new CommandEcho());
        commandList.push_back(new CommandDebug());
        commandList.push_back(new CommandEffect());
        commandList.push_back(new CommandXp());
    }

    void cleanupCommands(){
        commandList.erase(commandList.begin(), commandList.end());
    }
    
}
