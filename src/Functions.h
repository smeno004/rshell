#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Functions : public Tokens{
   protected:
      string func;
   public:
      Functions() : func("") {}
      Functions(string f) : func(f) {}
      
      string getString(){
         return func;
      }
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      
      bool parse(){
         return 1;
      }
      
      vector<CommandComposite*> getVec(){
         vector<CommandComposite*> vec;
         vec.push_back(this);
         return vec;
      }
};

#endif