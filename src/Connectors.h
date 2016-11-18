#ifndef __CONNECTORS_H__
#define __CONNECTORS_H__

#include <vector>

using namespace std;

class Connectors : public CommandComposite{ /* Class representing &&,
                                               ||, and ; */
   protected:
      string connecOper; /* The string storing the connector characters */
      
   public:
      Connectors() : connecOper(NULL) {} 
      /*default operator sets string to null*/
      Connectors(string input) : connecOper(input) {}
      /*operator with parameter sets connecOper to input*/
      
      string getString() {
         return connecOper;
      }/*returns strings storing the connector*/
      
      virtual void addElemsToVec(vector<CommandComposite *> &) {}
      /*addElemsToVec is only needed in Commands.h. As a result it is included
        here only so that class Connectors is not an abstract class.
      */
      bool parse() {
         return 1;
      }/*return true to show that the parse was successful*/
      
      vector<CommandComposite*> getVec() {
         vector<CommandComposite*> vec;
         vec.push_back(this);
         return vec;
      }/*Returns a vector of CommandComposite*'s with a single instance
         of itself */
         
      virtual string getUserInput() {
         return connecOper;
      }
};

#endif //__CONNECTORS_H__