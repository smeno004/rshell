#ifndef __CONNECTORS_H__
#define __CONNECTORS_H__

#include <vector>
#include <iostream>

using namespace std;

class Connectors : public CommandComposite{ // Class representing &&, ||, and ;
   protected:
      string connecOper; // The string storing the connector characters
   public:
      Connectors() : connecOper(NULL) {}
      Connectors(string input) : connecOper(input) {}
      
      string getString() {
         return connecOper;
      }
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      
      bool parse() {
         return 1;
      }
      
      vector<CommandComposite*> getVec(){
         vector<CommandComposite*> vec;
         vec.push_back(this);
         return vec;
      }
};

#endif //__CONNECTORS_H__