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
      Commands() {}
      Commands(string input){
         userInput = input;
      }
   
      void addElemsToVec(vector<CommandComposite *> &commVec) {
         //cout << "YOLO!" << endl;
         for (unsigned i = 0; i < vec2.size(); ++i) {
            // vec2.at(i)->getString();
            // cout << endl;
            commVec.push_back(vec2.at(i));
         }
         return;
      }
      
      bool parse() {
         //cout << userInput << endl;
         bool semiColon = false;
         bool multi = false;
         
         string newInput;
         
         unsigned ind = 0;
         while (ind != userInput.size()) {
            // cout << ind << ' ' << endl;
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
            if (newInput.at(i) == ';') {
               semiColon = true;  
               
               string left = newInput.substr(0, i);
               string semiC = newInput.substr(i, 1); 
               
               CommandComposite* lComm = new Commands(left);
               CommandComposite* semi = new Connectors(semiC);
               
               // vec2.push_back(lComm);
               lComm->parse();
               lComm -> addElemsToVec(vec2);
               vec2.push_back(semi);
               
               if(i != newInput.size() - 1) {
                  string right = newInput.substr(i + 1);
                  CommandComposite* rComm = new Commands(right);
                  // vec2.push_back(rComm);
                  rComm->parse();
                  rComm -> addElemsToVec(vec2);
               }
               
               multi = true;
               
               // divide string into the multiple commands and pass them into vec2
               // call parse on each command 
            }
            ++i;
         }
         // else {
         //       divide string into various char *
         //       based on whether it is a token or a connector, make a new token or connector object and pass into vec2
         // }
         i = 0;
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
                        CommandComposite * connect = new Connectors(tokenOrConnect);
                        vec2.push_back(connect);
                        multi = true;
                        // cout << "&&" << endl;
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
                           CommandComposite* paramtrs = new Parameters(tokenOrConnect); 
                           vec2.push_back(paramtrs);
                           //create a new parameter object
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
      
      /*bool parse(){
         int i = 0;
         while (userInput.at(i) != '\0')
            while(userInput.at(i) == ' ' || userInput.at(i) == '\t'){
               userInput.at(i) == '\0'; // replaces white space
            }
            
      }*/
      
      string getString(){
         string temp;
         for (unsigned i  = 0; i < vec2.size(); i++){
            temp += vec2.at(i)->getString();
         }
         return temp;
      }
      
      vector<CommandComposite*> getVec(){
         return vec2;
      } 
};

#endif //__COMMANDS_H__