#ifndef __EXECCOMMAND_H__
#define __EXECCOMMAND_H__

#include <string>
#include "CommandComposite.h"
#include "Commands.h"

class ExecCommand{
   protected:
      string Path2;
      CommandComposite* cmd;
   public:
      //Default Constructor
      ExecCommand(){}
      
      //Constructor which takes a command pointer
      ExecCommand(CommandComposite* command) : cmd(command) {}
      
      virtual bool getExitStatus() = 0;
      
      virtual bool getExecStatus() = 0;
      
      //pure virtual execute function
      virtual bool execute() = 0;
};

#endif //__EXECCOMMAND_H__