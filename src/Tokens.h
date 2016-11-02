#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <vector>
#include <iostream>

using namespace std;

class Tokens : public CommandComposite{
   protected:
      string funcTex;
      
   public:
      Tokens() : funcTex("") {}
      Tokens(string input) : funcTex(input) { }
      
      string getString() {
         return funcTex;
      }
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      
      bool parse() {
         return 1;
      }
      
      vector<CommandComposite*> getVec(){
         vector<CommandComposite *> vec;
         vec.push_back(this);
         return vec;
      }
};

#endif //__TOKENS_H__