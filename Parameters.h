#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Parameters : public Tokens{
   protected:
      string param;
   public:
      Parameters() : param("") {}
      Parameters(string p) : param(p) {}
   
      string getString(){
         return param;  
      }
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      
      bool parse(){
         return 1;
      }
      
      vector<CommandComposite*> getVec(){
         vector<CommandComposite *> vec;
         vec.push_back(this);
         return vec;
      }
};

#endif