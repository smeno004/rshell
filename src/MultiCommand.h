#ifndef __MULTICOMMAND_H__
#define __MULTICOMMAND_H__

#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fstream>

#include <iostream>
#include <ostream>
#include <istream>

using namespace std;

class MultiCommand : public ExecCommand{ 
   /* strategy class for handling multiple commands or commands 
      that have any form of seperation */
   protected:
      string path2;
      CommandComposite* cmd;
      const char** argv;
      bool exitStatus;
      
   public:
      /*Constructor with CommandComposite pointer */
      MultiCommand(CommandComposite* com){
         cmd = com;
         argv = new const char*[64];
         exitStatus = false;
      }
      
      bool getExitStatus() {
         return exitStatus;
      }
      
      /*
      Function Name: execute
      Input Parameters: None
      Return: void
      Description: This function takes the CommandComposite* and gets the
      argument list into the char array argv. Then it executes the command
      by forking and calling execvp on the separate commands.
      Certain cases involving connectors and their functionality are also
      implemented.
      */
      bool execute(){
         vector<CommandComposite*> vec = cmd->getVec();
         // cout << vec.at(vec.size() - 1)->getString() << endl;
         // int begin = 0;
         int status;
         int NUM_SIZE = 0;

         /*
         Creates a variable that is shared across all processes.
         This helps in accessing flags that indicate when to skip
         a command based on the execution status of a previous 
         command (due to presence of a constructor) 
         */
         int * globvar = (int*)mmap(NULL, sizeof &globvar, PROT_READ
                        | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

         *globvar = 0;
         
         pid_t child_pid;
         
         for (unsigned i = 0; i < vec.size(); i++) {
            if (vec.at(i)->getString() == "&&" || 
                vec.at(i)->getString() == "||" || 
                vec.at(i)->getString() == ";" ) {
               NUM_SIZE += 1; //Gets the number of commands to execute
            }
         }
         
         if (vec.at(vec.size() - 1)->getString() != ";") {
            NUM_SIZE += 1;
         }
         
         // cout << "NUM_SIZE: " << NUM_SIZE << endl;
         // cout << "Vec size: " << vec.size() << endl;
         for (int i = 0; i < NUM_SIZE; i++) {
            // cout << i << endl;
            int numCount = 0;
            int argInd = 0;
            unsigned vecInd = 0;
            int prevConn = 0; // 0 - ;, 1 - &&, 2 - ||
            
            // unsigned begInd = 0;
            // cout << i << endl;
            // cout << "globvar: " << *globvar << endl;
            
            //If this flag is set, skip next command
            if (*globvar == 1) {
               i += 1;
            }
            
            if (i == NUM_SIZE) {
               break;
            }
            
            //populates the argument vector with the required argument list
            //of the present command pointed to by i
            for (vecInd = 0; vecInd < vec.size(); ++vecInd) {
               if (vec.at(vecInd)->getString() != "&&" &&
                   vec.at(vecInd)->getString() != "||" && 
                   vec.at(vecInd)->getString() != ";") {
                  if (numCount == i) {
                     argv[argInd] = vec.at(vecInd)->getString().c_str();
                     ++argInd;
                  }
               }
               else {
                  numCount += 1;
                  
                  if (numCount == i - 1) {
                     if (vec.at(vecInd)->getString() == ";") {
                        prevConn = 0;
                     }
                     else if (vec.at(vecInd)->getString() == "&&") {
                        prevConn = 1;
                     }
                     else if (vec.at(vecInd)->getString() == "||") {
                        prevConn = 2;
                     }
                  }
                  
                  if (numCount == (i + 1)) {
                     break;
                  }
                  // ++begInd;
               }
            }
            argv[argInd] = '\0';
            
            if(*globvar == 1) {
               if (vec.at(vecInd - argInd - 1)->getString() == "&&" && prevConn == 1) {
                  continue;
               }
               else if (vec.at(vecInd - argInd - 1)->getString() == "||" && prevConn == 2) {
                  continue;
               }
               else {
                  *globvar = 0;
               }
            }
            
            if (vec.at(vecInd - 1)->getString() == "exit") {
               exitStatus = true;
               return true;
            }
            
            if(vec.at(vecInd - argInd)->getString() == "test" || vec.at(vecInd - argInd)->getString() == "[" ) {
               string inputString = "";
               string openBracket = "[";
               string closeBracket = "]";
               
               for (int j = 0; j < argInd; j++) {
                  if (vec.at(vecInd - argInd + j)->getString() == openBracket) {
                     inputString += "test";
                     inputString += " ";
                  }
                  
                  else if (vec.at(vecInd - argInd + j)->getString() == closeBracket.c_str()) {
                  }
                  
                  else {
                     inputString += argv[j];
                     inputString += " ";
                  }
               }
               
               Test* test = new Test(inputString);
               
               int testExecute = test->execute();
               
               if (vecInd == vec.size()) {
                  *globvar = 0;
               }
               else if(testExecute == 1 && vec.at(vecInd)->getString() == "&&"){
                  *globvar = 1;
               }
               else if(testExecute == 0 && vec.at(vecInd)->getString() == "||"){
                  *globvar = 1;
               }
               else{
                  *globvar = 0;
               }
               
               for (int j = 0; j < argInd; j++) { // for the entirety of argv
                argv[j] = '\0'; //replace each element with null '\0'
               }
               
               continue; // Continues to next iteration of the outer for loop
            }
         
            /*string testComm = "test";
            
            if (argv[0] == testComm.c_str()){
               cout << "Found a test " << endl;
            }*/
            
            child_pid = fork(); //Fork to a child process, to execute command
            
            if (child_pid < 0) { // if the fork failed
               perror("FORKING FAILED"); //print error message
               exit(1); // exit
            }
            else if (child_pid == 0) { // child process
               //if(execvp(argv[0], (char**)argv) < 0){ // if excepvp fails
                 // cout << "Execvp failed! "<< endl;
               //}
               /*if(connector = ';'){
                  
               }*/
               
               if (vecInd == vec.size() || vec.at(vecInd)->getString() == ";") {
                  // cout << "SEMICOLON or END!" << endl;
                  if (execvp(argv[0], (char**)argv) < 0) {
                     perror("EXECVP FAILED");
                  }
               }
               else if (vec.at(vecInd)->getString() == "||") {
                  if (execvp(argv[0], (char**)argv) < 0) {
                     perror("EXECVP FAILED");
                  }
                  else {
                     *globvar = 1;
                  }
               }
               else if (vec.at(vecInd)->getString() == "&&") {
                  // cout << "AND!" << endl;
                  if (execvp(argv[0], (char**)argv) < 0) {
                     perror("EXECVP FAILED");
                     *globvar = 1;
                  }
               }
            }
            else {
               //Parent Process
               for (int i = 0; i < NUM_SIZE; i ++) {
                  //Waits for child process to terminate
                   waitpid(child_pid, &status, 0); 
                  
                  //Checks exit status of child process to determine flag
                  if (WIFEXITED(status) && vecInd < vec.size()) {
                     if (WEXITSTATUS(status) > 0) {
                        if (vec.at(vecInd)->getString() == ";" ||
                            vec.at(vecInd)->getString() == "||") {
                           *globvar = 0;
                        }
                        else if (vec.at(vecInd)->getString() == "&&") {
                           //cout << "Failed" << endl;
                           *globvar = 1;
                        }
                     }
                     else {
                        if (vec.at(vecInd)->getString() == "||") {
                           *globvar = 1;
                        }
                     }
                  }
               }
            }
            
            for (int j = 0; j < argInd; j++) { // for the entirety of argv
                argv[j] = '\0'; //replace each element with null '\0'
            }
         }
         
         unsigned i = vec.size() - 1;
         while(vec.at(i)->getString() != "&&" &&
               vec.at(i)->getString() != "||" &&
               vec.at(i)->getString() != ";"){
                  i--;
               } // grabs last connector in the command
         
         if (*globvar == 1) {
            if (vec.at(i)->getString() == "&&") {
               return false;
            }
            else {
               return true;
            }
         }
         else {
            if (vec.at(i)->getString() == "&&" ||
                vec.at(i)->getString() == ";") {
               return true;
            }
            else {
               return false;
            }
         }
         
         return true;
      }
};

#endif //__MULTICOMMAND_H__