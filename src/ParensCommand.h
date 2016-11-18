#ifndef __PARENSCOMMAND_H__
#define __PARENSCOMMAND_H__

using namespace std;

class ParensCommand : public ExecCommand {
   protected:
      CommandComposite* cmd;
      bool exitStatus; // bool used for exit command being used in
      bool execStatus;
   public:
      //Constructor which takes a command pointer
      ParensCommand(CommandComposite* command) : cmd(command) {
         exitStatus = false;
         execStatus = true;
      }
      
      /*
      Function Name: execute
      Input Parameters: None
      Return: bool
      Description: Runs the commands specified in Parens command.
      */
      bool execute(){
         vector<string> cmdStrings; // string of commands in the parens
         vector<string> parenConnectors;
         unsigned numCommands = 0; // number of total commands
         string toAdd;
         
         for (unsigned i = 0; i < cmd->getVec().size(); i++) {
            string toCompare = cmd->getVec().at(i)->getString();
            
            if (toCompare == ")" 
               && (cmd->getVec().at(i - 1)->getString() == "&&"
               || cmd->getVec().at(i - 1)->getString() == "||"
               || cmd->getVec().at(i - 1)->getString() == ";")
               && cmd->getVec().at(i - 2)->getString() == "(") {
               // if one of (&&), (||), or (;)
               cmdStrings.push_back(toAdd); // push back emptystring
               numCommands++;
               toAdd = "";
               continue;
            }
            
            if (toCompare != "(" && toCompare != ")") {
               toAdd += toCompare + " "; // if not '(' or ')', add str to toAdd
            }
            
            if(toCompare == ")" || i == (cmd->getVec().size() -1)) {
               // if reached end of parenthetical expression
               if(toAdd.at(0) == '|' || toAdd.at(0) == '&'){
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,2);
               } // if the string starts with a logical connector, remove it
               else if(toAdd.at(0) == ';'){
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,1);
               } // if the string starts with a semicolon, remove it
               
               
               if (toAdd.at(toAdd.size() - 2) == '|' 
                     || toAdd.at(toAdd.size() - 2) == '&' ) {
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 3, 2);
               }
               else if (toAdd.at(toAdd.size() - 2) == ';') { 
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 2, 1);
               }
               
               cmdStrings.push_back(toAdd); // add parse'd out command to vec
               numCommands ++;
               toAdd = ""; // empty toAdd
            }
            
            else if (toCompare == "(" &&
                     i != 0 && 
                     cmd->getVec().at(i - 2)->getString() != ")" ) {
               /* if beginning of parenthetical expression, and two spaces
               before was a closing parenthasee */
               if (toAdd.at(0) == '|' || toAdd.at(0) == '&') {
                  // if toAdd starts with a connector
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,2); //remove the connectors
               }
               else if (toAdd.at(0) ==';') { // if toAdd starts with a ;
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,1); // remove ;
               }
               
               if (toAdd.at(toAdd.size() - 2) == '|' 
                     || toAdd.at(toAdd.size() - 2) == '&') {
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 3, 2);
               }
               else if (toAdd.at(toAdd.size() - 2) == ';') {
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 2, 1);
               }
               
               cmdStrings.push_back(toAdd);
               numCommands++;
               toAdd = "";
            }
         }
         
         // for (unsigned i = 0; i < cmdStrings.size(); i++) {
         //    cout << cmdStrings.at(i) << endl;
         // }
         // for(unsigned i = 0; i < parenConnectors.size(); i++){
         //    cout << parenConnectors.at(i) <<endl;
         // }
         
         // cout << numCommands << endl;
         
         for (unsigned i = 0; i < numCommands; i++) {
            CommandComposite* temp = new Commands(cmdStrings.at(i));
            bool commandType = temp->parse();
            bool executeStatus;
            // cout << cmdStrings.at(i) << endl;
            
            // cout << commandType << endl;
            
            if (commandType) {
            // If command is a multicommand, run the strategy for multicommands
               ExecCommand* multiComm = new MultiCommand(temp); 
               multiComm->execute();
               executeStatus = multiComm->getExecStatus();
               if(multiComm->getExitStatus()){
                  exitStatus = true;
                  
                  execStatus = true;
                  return true;
               }
            }
            else if (!commandType) {
               // If command is a single command, run the strategy for single commands
               ExecCommand* singleComm = new SingleCommand(temp);
               singleComm->execute();
               executeStatus = singleComm->getExecStatus();
               if(singleComm->getExitStatus()){
                   exitStatus = true;
                   
                   execStatus = true;
                   return true;
               }
            }
            
            
            if (i != numCommands - 1) {
               if (parenConnectors.at(i) == "|") {
                  if (executeStatus) {
                     i++;
                  }
               }
               else if (parenConnectors.at(i) == "&") {
                  if (!executeStatus) {
                     i++;
                  }
               }
            }
         }
         execStatus = true;
         // return;
         return true;
      }
      
      bool getExitStatus() {
         return exitStatus;
      }
      
      bool getExecStatus() {
         return execStatus;
      }
};

#endif 