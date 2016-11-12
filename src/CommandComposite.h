#ifndef __COMMAND_COMPOSITE_H__
#define __COMMAND_COMPOSITE_H__

#include <vector>

using namespace std;

class CommandComposite{ /*Abstract base class for the composite pattern
                          used to parse user input */
   protected:
      string inputCommand; /*string holding different parts of userInput 
                           command line depending on what kind of pointer 
                           child is */
   public:
      CommandComposite(){}
      virtual string getString() = 0; 
      /*returns the string the object
        is storing*/
        
      virtual void addElemsToVec(vector<CommandComposite *> &) = 0; 
      /* Allows us to pass down the parent vector to a newly created Child
         object to copy over the contents of the child's vector into the 
         parent's vector.*/
         
      virtual bool parse() = 0;
      /*Main parsing function, takes in user input, tokenizes it by the
        connectors, creates appropriate objects of type commands, tokens,
        or connects, and stores them all in an array using addElemsToVec()*/
        
      virtual vector<CommandComposite*> getVec() = 0;
      /*Returns a vector storing CommandComposite*'s  which collectively
        store the userInput in a tokenized string*/
      
      virtual string getUserInput() = 0;
};

#endif //__COMMAND_COMPOSITE_H__