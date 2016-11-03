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
      
      //pure virtual execute function
      virtual void execute() = 0;
};

#endif //__EXECCOMMAND_H__