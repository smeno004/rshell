#ifndef __PARENSCOMMAND_H__
#define __PARENSCOMMAND_H__

using namespace std;

class ParensCommand : public ExecCommand {
   protected:
      CommandComposite* cmd;
      bool exitStatus;
   public:
      //Constructor which takes a command pointer
      ParensCommand(CommandComposite* command) : cmd(command) {
         exitStatus = false;
      }
   
      bool execute(){
         vector<string> cmdStrings;
         vector<string> parenConnectors;
         unsigned numCommands = 0; // number of total commands...kinda (multcommands = 1 command)
         string toAdd;
         
         for (unsigned i = 0; i < cmd->getVec().size(); i++) {
            string toCompare = cmd->getVec().at(i)->getString();
            // if(toCompare == "("){
            //    cout << toCompare << endl;
            //    cout << "\t" << "toCompare is an open paren: " << (toCompare == "(") << endl;
            //    cout << "\t" << "i not equal to 0: " << (i != 0) << endl;
            //    cout << "\t" << "crazy bool case: " << (cmd->getVec().at(i - 2)->getString() != ")" ) << endl;
            // }
               
            if(toCompare != "(" && toCompare != ")"){
               toAdd += toCompare + " ";
            }
            
            if(toCompare == ")" || i == (cmd->getVec().size() -1)) {
               if(toAdd.at(0) == '|' || toAdd.at(0) == '&'){
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,2);
               }
               else if(toAdd.at(0) == ';'){
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,1);
               }
               
               if (toAdd.at(toAdd.size() - 2) == '|' 
                     || toAdd.at(toAdd.size() - 2) == '&' ) {
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 3, 2);
               }
               else if (toAdd.at(toAdd.size() - 2) == ';') { 
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 2, 1);
               }
               
               cmdStrings.push_back(toAdd);
               numCommands ++;
               toAdd = "";
            }
            
            else if(toCompare == "(" && i != 0 && cmd->getVec().at(i - 2)->getString() != ")" ) {
               if(toAdd.at(0) == '|' || toAdd.at(0) == '&'){
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,2);
               }
               else if(toAdd.at(0) ==';'){
                  parenConnectors.push_back(toAdd.substr(0,1));
                  toAdd.erase(0,1);
               }
               
               if (toAdd.at(toAdd.size() - 2) == '|' 
                     || toAdd.at(toAdd.size() - 2) == '&') {
                  parenConnectors.push_back(toAdd.substr(toAdd.size() - 2, 1));
                  toAdd.erase(toAdd.size() - 3, 2);
               }
               else if(toAdd.at(toAdd.size() - 2) == ';'){
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
         
         for(unsigned i = 0; i < numCommands; i++) {
            CommandComposite* temp = new Commands(cmdStrings.at(i));
            bool commandType = temp->parse();
            bool execStatus;
            // cout << cmdStrings.at(i) << endl;
            
            // cout << commandType << endl;
            
            if (commandType) {
            // If command is a multicommand, run the strategy for multicommands
               ExecCommand* multiComm = new MultiCommand(temp); 
               execStatus = multiComm->execute();
               if(multiComm->getExitStatus()){
                  exitStatus = true;
                  return true;
               }
            }
            else if(!commandType) {
               // If command is a single command, run the strategy for single commands
               ExecCommand* singleComm = new SingleCommand(temp);
               execStatus = singleComm->execute();
               if(singleComm->getExitStatus()){
                   exitStatus = true;
                   return true;
               }
            }
            
            if (i != numCommands - 1) {
               if (parenConnectors.at(i) == "|") {
                  if (execStatus) {
                     i++;
                  }
               }
               else if (parenConnectors.at(i) == "&") {
                  if (!execStatus) {
                     i++;
                  }
               }
            }
         }
         return true;
      }
      
      bool getExitStatus(){
         return exitStatus;
      }
};

#endif 