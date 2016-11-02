#ifndef __SINGLE_COMMAND_H__
#define __SINGLE_COMMAND_H__

#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h> 

class SingleCommand : public ExecCommand{
   protected:
      string path2;
      CommandComposite* cmd;
      const char** argv;
   public:
      SingleCommand(CommandComposite* com){
         cmd = com;
         argv = new const char*[64];
      }
      void execute(){
         int status;
         pid_t child_pid;
         
         vector<CommandComposite*> vec = cmd->getVec();
         const char* cmnd = vec.at(0)->getString().c_str();
         // cout << "first part of single command function: " << func << endl;
         for(unsigned i = 0; i< vec.size(); i++){
            if(vec.at(i)->getString() != ";"){
               argv[i] = vec.at(i)->getString().c_str();
            }
         }
         
         if(cmd->getString() == "exit"){
            exit(1);
         }
         if(cmd->getString() == ""){
            return;
         }         
         child_pid = fork();
            
         if (child_pid < 0)
         {
            perror("FORKING FAILED");
            exit(1);
         }
         else if (child_pid == 0)
         {
            if(execvp(cmnd, (char**)argv) < 0){
               perror("EXECVP FAILED");
               exit(1);
            }
         }   
         waitpid(child_pid, &status, 0);
      }
};

#endif //__SINGLE_COMMAND_H__