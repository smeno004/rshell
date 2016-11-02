#ifndef __COMMAND_COMPOSITE_H__
#define __COMMAND_COMPOSITE_H__

#include <vector>

using namespace std;

class CommandComposite{
   protected:
      string inputCommand;
   public:
      CommandComposite(){}
      virtual string getString() = 0;
      virtual void addElemsToVec(vector<CommandComposite *> &) = 0;
      virtual bool parse() = 0; 
      virtual vector<CommandComposite*> getVec() = 0;
};

#endif //__COMMAND_COMPOSITE_H__