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

using namespace std;

int main(){
    string userInput;
    // char * charUserInput = new char[64];
    char hostname[1024];
    gethostname(hostname, 1024);
    passwd *pw = getpwuid(getuid());
    string username = pw->pw_name;
    userInput = " ";
    /* fgets(charUserInput, 64, stdin);
    cin >> setw(64) >> charUserInput;
    cout << charUserInput[3] << endl;
    gets(charUserInput);
    int i = userInput.size();
    char * charUserInput = &userInput[0];
    charUserInput[i] = '\0'; */
    
    //The following while loop should keep the rshell open until exit is called
    while(userInput != "exit"){
        cout << "[" << username << "@" << hostname << "]$ "; //EXTRA CREDIT STUFF
        getline(cin, userInput); // Takes user input
        CommandComposite* temp = new Commands(userInput); // Makes a new Commands object using the userInput
        bool commandType = temp -> parse(); // Parses the userInput and tokenizes it based on whitespaces
        if(commandType){
            // If command is a multicommand, run the strategy for multicommands
            ExecCommand* multiComm = new MultiCommand(temp); 
            multiComm->execute(); 
        }
        else{
            // If command is a single command, run the strategy for single commands
            ExecCommand* singleComm = new SingleCommand(temp);
            singleComm->execute();
        }
        /* cout << temp->getString();
        cout << endl;
        cout << "[" << hostname << "@" << username << " ~]$ ";
        getline(cin, userInput);*/
    }
    return 0;
}