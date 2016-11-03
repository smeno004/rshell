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
      /*Default Constructor*/
      Functions() : func("") {}
      /*Constructor with string input - holds token*/
      Functions(string f) : func(f) {}
      
      string getString(){
         return func;
      }/*returns string storing the token*/
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      
      bool parse(){
         return 1;
      }/*return true to show that the parse was successful*/
      
      vector<CommandComposite*> getVec(){
         vector<CommandComposite*> vec;
         vec.push_back(this);
         return vec;
      }/*Returns a vector of CommandComposite*'s with a single instance
         of itself */
};

#endif