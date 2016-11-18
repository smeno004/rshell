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
#include "ParensCommand.h"

using namespace std;

int main(){
    string userInput;
    // char * charUserInput = new char[64];
    char hostname[1024];
    gethostname(hostname, 1024);
    passwd *pw = getpwuid(getuid());
    string username = pw->pw_name;
    
    //The following while loop should keep the rshell open until exit is called
    while(userInput != "exit"){
        cout << "[" << username << "@" << hostname << "]$ "; //EXTRA CREDIT STUFF
        getline(cin, userInput); // Takes user input
        CommandComposite* temp = new Commands(userInput); // Makes a new Commands object using the userInput
        bool commandType = temp -> parse(); // Parses the userInput and tokenizes it based on whitespaces
        bool parensType = false;
        
        for(unsigned i = 0 ; i < userInput.size(); i++) {
            if (userInput.at(i) == '(') {
                parensType = true;
            }    
        }
        
        if (parensType) {
            ExecCommand * parensObj = new ParensCommand(temp);
            parensObj->execute();
            if (parensObj->getExitStatus()) {
                return 0;
            }
        }
        else if (commandType) {
            // If command is a multicommand, run the strategy for multicommands
            ExecCommand* multiComm = new MultiCommand(temp); 
            multiComm->execute();
            if(multiComm->getExitStatus()) {
                return 0;
            }
        }
        else if(!commandType) {
            // If command is a single command, run the strategy for single commands
            ExecCommand* singleComm = new SingleCommand(temp);
            singleComm->execute();
            if(singleComm->getExitStatus()) {
                return 0;
            }
        }
    }
    return 0;
}