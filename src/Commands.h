#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "CommandComposite.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>

class ExecCommand;

class Commands : public CommandComposite { 
   protected:
      string userInput;
      ExecCommand* vec1;
      vector<CommandComposite*> vec2;
   public:
      /*Default Constructor*/
      Commands() {}
      /*Constructor with string input*/
      Commands(string input){
         userInput = input;
      }
      
      /*
      Function Name: addElemsToVec
      Input Parameters: reference to a vector of CommandComposite*s
      Return: void
      Description: This function helps to populate the vec2 object with
      elements of the leaf sub-classes of CommandComposite*s
      */
      void addElemsToVec(vector<CommandComposite *> &commVec) {
         for (unsigned i = 0; i < vec2.size(); ++i) {
            // vec2.at(i)->getString();
            // cout << endl;
            commVec.push_back(vec2.at(i));
         }
         return;
      }
      
      /*
      Function Name: parse
      Input Parameters: None
      Return: bool
      Description: This function tokenizes the input string of commands
      based on connectors. Then, it creates new CommandComposite*s which
      get resolved to more Commands, Tokens or Connectors and adds the final 
      tokenized strings into vec2. It returns if the parse results in a 
      MultiCommand or a SingleCommand, which decides the Strategy used
      to execute command.
      */
      bool parse() {
         //cout << userInput << endl;
         bool semiColon = false; //flag for if semicolor exists in userInput
         bool multi = false; //flag for multicommand or single command - gets returned
         string newInput;
         
         unsigned ind = 0;
         while (ind != userInput.size()) {
            //The following code handles commented commands
            if (userInput.at(ind) == '#') {
               if (userInput.at(ind - 1) == ' ') {
                  newInput.erase(newInput.find_last_not_of(' ') + 1);
               }
               break;
            }
            else {
               newInput += userInput.at(ind);
            }
            ind++;
         }
         
         // cout << newInput << endl;
         // for(int i = 0; i < userInput.size(); i ++){
         unsigned i = 0;
         while (i != newInput.size() && !semiColon) {
            //If the input is a multi-command, split the input string
            //into separate Command pointers, separated by the connector
            if (newInput.at(i) == ';') {
               semiColon = true;  
               
               string left = newInput.substr(0, i);
               string semiC = newInput.substr(i, 1); 
               
               CommandComposite* lComm = new Commands(left);
               CommandComposite* semi = new Connectors(semiC);
               
               // parses new Command pointer
               lComm->parse();
               
               //adds final parsed elements of the left command to vec2
               lComm -> addElemsToVec(vec2);
               
               //push connector object to vec2
               vec2.push_back(semi);
               
               if(i != newInput.size() - 1) {
                  string right = newInput.substr(i + 1);
                  
                  //parses new Command pointer
                  CommandComposite* rComm = new Commands(right);
                  
                  rComm->parse();
                  rComm -> addElemsToVec(vec2);
               }
               
               multi = true;
               
            }
            ++i;
         }
         i = 0;
         //If the command/sub-command is a single-command/has no semicolon
         //parse the single command/command with && or ||
         if (!semiColon) {
            int begin = 0;
            bool first = true;
            string tokenOrConnect = newInput;
            
            while (i != newInput.size()) {
               if (newInput.at(i) == ' ') {
                  tokenOrConnect = newInput.substr(begin, i - begin);
                  if (tokenOrConnect != "") {
                     if (tokenOrConnect == "&&" || tokenOrConnect == "||") {
                        // cout << tokenOrConnect << endl;
                        // cout << "\ti: " << i << " \tbegin: " << begin << endl;
                        
                        //Creates new Connectors pointer
                        CommandComposite * connect = new Connectors(tokenOrConnect);
                        vec2.push_back(connect);
                        
                        //treat any command with connectors as multi-command
                        multi = true; 
                        first = true;
                     }
                     else {
                        if (first) {
                           //create a new function object
                           CommandComposite* funcns = new Functions(tokenOrConnect);
                           first = false;
                           vec2.push_back(funcns);
                        }
                        else{
                           //create a new parameter object
                           CommandComposite* paramtrs = new Parameters(tokenOrConnect); 
                           vec2.push_back(paramtrs);
                           // CommandComposite * token = new Tokens(tokenOrConnect);
                        }
                     }
                  }
                  begin = i + 1;
               }
               
               ++i;
               // cout << tokenOrConnect << endl;
            }
            tokenOrConnect = newInput.substr(begin, i - begin);
            if (tokenOrConnect != "&&" && tokenOrConnect != "||"){
               if(first){
                  //create a new function object
                  CommandComposite* funcns = new Functions(tokenOrConnect);
                  first = false;
                  vec2.push_back(funcns);
               }
               else{
                  //create a new parameter object
                  CommandComposite* paramtrs= new Parameters(tokenOrConnect); 
                  vec2.push_back(paramtrs);
               }
            }
         }
         return multi;
      }
      
      /*
      Function Name: getString
      Input Parameters: None
      Return: string
      Description: returns the string of Commands, Tokens or Connectors
      stored in vec2 after parsing
      */
      string getString(){
         string temp;
         for (unsigned i  = 0; i < vec2.size(); i++){
            temp += vec2.at(i)->getString();
         }
         return temp;
      }
      
      /*
      Function Name: getVec
      Input Parameters: None
      Return: vector of CommandComposite*s
      Description: This function simply returns the vector of final
      parsed CommandComposite* to leaf objects
      */
      vector<CommandComposite*> getVec() {
         return vec2;
      }
};

#endif //__COMMANDS_H__