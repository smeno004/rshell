#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Parameters : public Tokens{ /* Leaf class that inherits from Tokens 
                                     representing the flags or parameters 
                                     part of the function call. */
   protected:
      string param; /*String that holds a parameter/flag */
   public:
      /*Default Constructor*/
      Parameters() : param("") {}
      /*Constructor with string input - holds token*/
      Parameters(string p) : param(p) {}
      
      string getString() {
         return param;  
      }/*returns string storing the token*/
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      /*addElemsToVec is only needed in Commands.h. As a result it is included
        here only so that class Connectors is not an abstract class.*/
        
      bool parse() {
         return 1;
      }/*return true to show that the parse was successful*/
      
      vector<CommandComposite*> getVec() {
         vector<CommandComposite *> vec;
         vec.push_back(this);
         return vec;
      }/*Returns a vector of CommandComposite*'s with a single instance
         of itself */
         
      string getUserInput(){
         return param;
      }
};

#endif