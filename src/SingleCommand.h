#ifndef __SINGLE_COMMAND_H__
#define __SINGLE_COMMAND_H__

#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h> 

class SingleCommand : public ExecCommand {
   protected:
      string path2;
      CommandComposite* cmd;
      const char** argv;
   public:
   
      //Constructor which takes in CommandComposite*
      //and sets a char array to hold the arguments for execvp
      SingleCommand(CommandComposite* com) {
         cmd = com;
         argv = new const char*[64];
      }
      
      /*
      Function Name: execute
      Input Parameters: None
      Return: void
      Description: This function takes the CommandComposite* and gets the
      argument list into the char array argv. Then it executes the command
      by forking and calling execvp.
      */
      void execute() {
         int status;
         pid_t child_pid;
         
         //Gets the vector of CommandComposite leaf objects
         vector<CommandComposite*> vec = cmd->getVec();
         const char* cmnd = vec.at(0)->getString().c_str();
         
         //Populates argv with the required argument list 
         for (unsigned i = 0; i< vec.size(); i++) {
            if (vec.at(i)->getString() != ";") {
               argv[i] = vec.at(i)->getString().c_str();
            }
         }
         
         //Exits the shell
         if (cmd->getString() == "exit") {
            exit(1);
         }
         
         if (cmd->getString() == "") { //If user enters empty string (aka \n)
            return; //return, output nothing, prompts user again
         }         
         
         child_pid = fork(); //Forks Parent process to a child process
         if (child_pid < 0) {
            perror("FORKING FAILED"); //error for when forking fails
            exit(1);
         }
         else if (child_pid == 0) {
            if (execvp(cmnd, (char**)argv) < 0) {
               perror("EXECVP FAILED"); //error for when execvp fails
               exit(1);
            }
         }   
         waitpid(child_pid, &status, 0); //waits for child process to terminate
      }
};

#endif //__SINGLE_COMMAND_H__