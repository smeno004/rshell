#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <vector>
#include <iostream>

using namespace std;

class Tokens : public CommandComposite{
   protected:
      string funcTex;
      
      /*Default Constructor*/
      Tokens() : funcTex("") {}
      /*Constructor with string input - holds token*/
      Tokens(string input) : funcTex(input) {}
      
      string getString() {
         return funcTex;
      }/*returns string storing the token*/
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      
      bool parse() {
         return 1;
      }/*return true to show that the parse was successful*/
      
      vector<CommandComposite*> getVec() {
         vector<CommandComposite *> vec;
         vec.push_back(this);
         return vec;
      }/*Returns a vector of CommandComposite*'s with a single instance
         of itself */
};

#endif //__TOKENS_H__