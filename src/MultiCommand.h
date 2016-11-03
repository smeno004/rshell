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

class MultiCommand : public ExecCommand{ // strategy class for handling multiple commands or commands that have any form of seperation
   protected:
      string path2;
      CommandComposite* cmd;
      const char** argv;
      
   public:
      MultiCommand(CommandComposite* com){
         cmd = com;
         argv = new const char*[64];
      }
      void execute(){
         vector<CommandComposite*> vec = cmd->getVec();
         // cout << vec.at(vec.size() - 1)->getString() << endl;
         // int begin = 0;
         int status;
         int NUM_SIZE = 0;
         
         int * globvar = (int*)mmap(NULL, sizeof &globvar, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

         *globvar = 0;
         
         pid_t child_pid;
         
         for (unsigned i = 0; i < vec.size(); i++) {
            if (vec.at(i)->getString() == "&&" || vec.at(i)->getString() == "||" || vec.at(i)->getString() == ";" ) {
               NUM_SIZE += 1;
            }
         }
         
         if (vec.at(vec.size() - 1)->getString() != ";")
         {
            NUM_SIZE += 1;
         }
         
         // cout << "NUM_SIZE: " << NUM_SIZE << endl;
         
         // cout << "Vec size: " << vec.size() << endl;
         for(int i = 0; i < NUM_SIZE; i++){
            // cout << i << endl;
            
            int numCount = 0;
            int argInd = 0;
            unsigned vecInd = 0;
            // cout << i << endl;
            
            cout << "globvar: " << *globvar << endl;
            
            if (*globvar == 1)
            {
               // cout << "hmmm,, I: " << i << endl;
               i += 1;
            }
            
            // cout << "I: " << i << endl;
            
            for (vecInd = 0; vecInd < vec.size(); ++vecInd) {
               if (vec.at(vecInd)->getString() != "&&" && vec.at(vecInd)->getString() != "||" && vec.at(vecInd)->getString() != ";") {
                  if (numCount == i) {
                     argv[argInd] = vec.at(vecInd)->getString().c_str();
                     ++argInd;
                  }
               }
               else {
                  numCount += 1;
                  if (numCount == (i + 1)) {
                     break;
                  }
               }
            }
            
            // if (vecInd == vec.size())
            // {
            //    vecInd -= 1;
            // }
            
            // cout << vec.at(vecInd)->getString() << endl;
            
            // for (vecInd = 0; vecInd < argInd; ++vecInd)
            // {
            //    cout << argv[vecInd] << ' ';
            // }
            // cout << endl;
            
            
            
            // cout << "test: " << vec.at(i)->getString()<<endl;
            // if (vec.at(i)->getString() == ";" ||
            //     vec.at(i)->getString() == "&&" ||
            //     vec.at(i)->getString() == "||"){
               // const char* cmnd = vec.at(begin)->getString().c_str();
               // int k = begin;
               
               // // cout << begin << ' ' << i << ' ' << (i - begin) << endl;
               // for(int j = 0; j < (i - begin); j++){
               //    argv[j] = vec.at(k)->getString().c_str();
               //    k += 1;
               // }
               

               // sleep(1);
               
            // cout << "argv[0]: " << argv[0] << endl;
            // string test = argv[0];
            // if(test == "exit"){
            //    exit(0);
            // }
            child_pid = fork();
            
            if (child_pid < 0)
            {
               perror("FORKING FAILED");
               exit(1);
            }
            else if (child_pid == 0)
            {
               //if(execvp(argv[0], (char**)argv) < 0){ // if excepvp fails
                 // cout << "Execvp failed! "<< endl;
               //}
               /*if(connector = ';'){
                  
               }*/
               if (vecInd == vec.size() || vec.at(vecInd)->getString() == ";") {
                  // cout << "SEMICOLON or END!" << endl;
                  if (execvp(argv[0], (char**)argv) < 0) {
                     perror("EXECVP FAILED");
                     exit(2);
                  }
               }
               else if (vec.at(vecInd)->getString() == "||") {
                  if (execvp(argv[0], (char**)argv) < 0) {
                     perror("EXECVP FAILED");
                     // *globvar = 0;
                  }
                  else
                  {
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
               for(int i = 0; i < NUM_SIZE; i ++){     
                  waitpid(child_pid, &status, 0);
                  
                  if(WIFEXITED(status) && vecInd < vec.size()) {
                     if (WEXITSTATUS(status) > 0) {
                        if (vec.at(vecInd)->getString() == ";" || vec.at(vecInd)->getString() == "||") {
                           *globvar = 0;
                        }
                        else if (vec.at(vecInd)->getString() == "&&") {
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
            
            for(int j = 0; j < argInd; j++){
               argv[j] = '\0';
            }
         }
         
         //Parent Process
         
         // for(int i = 0; i < NUM_SIZE; i ++){     
         //    waitpid(child_pid, &status, 0);
         // }
                  
         /*The following is to execute it for the last command
         if (vec.at(vec.size() - 1)->getString() != ";" && child_pid > 0)
         {
            const char* cmnd = vec.at(begin)->getString().c_str();
            int k = begin;
            // cout << cmnd << endl;
            
            // cout << (vec.size() - begin) << endl;
            
            for(int j = 0; j < (vec.size() - begin); j++){
               argv[j] = vec.at(k)->getString().c_str();
               // cout << argv[j] << endl;
               k += 1;
            }
            
            child_pid = fork();
            
            if (child_pid < 0)
            {
               cout << "Forking failed!" << endl;
               exit(1);
            }
            else if (child_pid == 0)
            {
               execvp(cmnd, (char**)argv);
            }
            else
            {
               while (wait(&status) != child_pid){
               }
            }
         }*/
      }
      
};

#endif //__MULTICOMMAND_H__