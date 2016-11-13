#ifndef __SINGLE_COMMAND_H__
#define __SINGLE_COMMAND_H__

#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include "Test.h"

class SingleCommand : public ExecCommand {
   protected:
      string path2;
      CommandComposite* cmd;
      const char** argv;
      bool exitStatus;
      
   public:
   
      //Constructor which takes in CommandComposite*
      //and sets a char array to hold the arguments for execvp
      SingleCommand(CommandComposite* com) {
         cmd = com;
         argv = new const char*[64];
         exitStatus = false;
      }
      
      /*
      Function Name: execute
      Input Parameters: None
      Return: void
      Description: This function takes the CommandComposite* and gets the
      argument list into the char array argv. Then it executes the command
      by forking and calling execvp.
      */
      bool execute() {
         int status;
         pid_t child_pid;
         int argInd = 0;
         
         //Gets the vector of CommandComposite leaf objects
         vector<CommandComposite*> vec = cmd->getVec();
         const char* cmnd = vec.at(0)->getString().c_str();
         
         //Populates argv with the required argument list 
         for (unsigned i = 0; i < vec.size(); i++) {
            if (vec.at(i)->getString() != ";") {
               argv[i] = vec.at(i)->getString().c_str();
               ++argInd;
            }
         }
         argv[argInd] = '\0';
         
         //Exits the shell
         if (cmd->getString() == "exit") { //checks to see if userInput is "exit"
            exitStatus = true; // sets bool variable exit status to true
            return true; /* return statement exits from execute function preventing
                       "exit" command from being passed into execvp */
         }
         
         if(cmd->getString().substr(0,4) == "test" || cmd->getString().substr(0,1) == "["){
            
            string inputString = "";
            string openBracket = "[";
            string closeBracket = "]";
            
            for (int j = 0; j < argInd; j++) {
               if (vec.at(j)->getString() == openBracket) {
                  inputString += "test";
                  inputString += " ";
               }
               
               else if (vec.at(j)->getString() == closeBracket.c_str()) {
               }
               
               else {
                  inputString += argv[j];
                  inputString += " ";
               }
            }
            
            Test* test = new Test(inputString);
            return !(test->execute()); /* return statement exits from execute function preventing
                       "test" command from being passed into execvp */
         }
         
         if (cmd->getString() == "") { //If user enters empty string (aka \n)
            return true; //return, output nothing, prompts user again
         }         
         
         child_pid = fork(); //Forks Parent process to a child process
         if (child_pid < 0) {
            perror("FORKING FAILED"); //error for when forking fails
            exit(1);
         }
         else if (child_pid == 0) {
            if (execvp(cmnd, (char**)argv) < 0) {
               perror("EXECVP FAILED"); //error for when execvp fails
               return false;
               //exit(1);
            }
         }   
         waitpid(child_pid, &status, 0); //waits for child process to terminate
         
         for (int j = 0; j < argInd; j++) { // for the entirety of argv
                argv[j] = '\0'; //replace each element with null '\0'
         }
         
         return true;
      }
      
      bool getExitStatus(){
         return exitStatus;
      }
};

#endif //__SINGLE_COMMAND_H__