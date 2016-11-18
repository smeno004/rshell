#ifndef __BASECOMMAND_H__
#define __BASECOMMAND_H__

#include "CommandComposite.h"

using namespace std;

class BaseCommand{
   protected:
      CommandComposite * root;
   public:
      BaseCommand(){}
      
      virtual int execute() = 0;
};

#endif