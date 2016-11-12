#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <iomanip>
#include "CommandComposite.h"
#include "Connectors.h"
#include "Tokens.h"
#include "Parameters.h"
#include "Functions.h"
#include "Commands.h"
#include "ExecCommand.h"
#include "SingleCommand.h"
#include "MultiCommand.h"
#include "Test.h"

using namespace std;

int main(){
    string userInput;
    // char * charUserInput = new char[64];
    char hostname[1024];
    gethostname(hostname, 1024);
    passwd *pw = getpwuid(getuid());
    string username = pw->pw_name;
    userInput = " ";
    
    //The following while loop should keep the rshell open until exit is called
    while(userInput != "exit"){
        cout << "[" << username << "@" << hostname << "]$ "; //EXTRA CREDIT STUFF
        getline(cin, userInput); // Takes user input
        CommandComposite* temp = new Commands(userInput); // Makes a new Commands object using the userInput
        bool commandType = temp -> parse(); // Parses the userInput and tokenizes it based on whitespaces
        cout << temp->getString() << endl;
        if(commandType){
            // If command is a multicommand, run the strategy for multicommands
            if (temp->getVec().at(0)->getString() == "(") {
                continue; // for now
            }
            ExecCommand* multiComm = new MultiCommand(temp); 
            multiComm->execute();
            if(multiComm->getExitStatus()){
                return 0;
            }
        }
        else{
            // If command is a single command, run the strategy for single commands
            if (temp->getVec().at(0)->getString() == "(") {
                continue; // for now
            }
            ExecCommand* singleComm = new SingleCommand(temp);
            singleComm->execute();
            if(singleComm->getExitStatus()){
                return 0;
            }
        }
        
        /* cout << temp->getString();
        cout << endl;
        cout << "[" << hostname << "@" << username << " ~]$ ";
        getline(cin, userInput);*/
    }
    return 0;
}