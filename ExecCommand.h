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
      ExecCommand(){}
      ExecCommand(CommandComposite* command) : cmd(command) {}
      virtual void execute() = 0;
};

#endif //__EXECCOMMAND_H__