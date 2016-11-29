#ifndef __CD_COMMAND_H__
#define __CD_COMMAND_H__

#include "BaseCommand.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

using namespace std;

//Command Pattern for the cd Command
class cd : public BaseCommand { 
   // protected:
   //    const char** argv;
   public:
      cd(string cmdStr) {
         // argv = new const char*[64];
         root = new Commands(cmdStr);
         root->parse();
      }
      
      /*
      Function Name: execute
      Input Parameters: None
      Return: int (tentative)
      Description: Executes the cd command. cd can take a filepath as
      parameters. 
      */
      int execute() {
         // int argInd = 0;
         string path;
         string interpath;
         string initpath;
         vector<string> pathVec;
         
         //Gets the vector of CommandComposite leaf objects
         vector<CommandComposite*> vec = root->getVec();
         //const char* cmnd = vec.at(0)->getString().c_str();
         
         // if (vec.size() >= 3) {
         //    cout << "Too many arguments to cd" << endl
         // throw error if cd syntax is wrong
         //    return 1;
         // }
         
         initpath += getenv("PWD"); // gets the current directory in a string
         initpath += "/"; // add the final slash
         bool cmdfail = false; // flag for invalid input
         
         if (vec.size() >= 3) { // if cd input is invalid, should not be > 2
            for (unsigned i = 1; i < vec.size() - 1; i++) { 
               initpath += vec.at(i)->getString();
               if (vec.at(i)->getString().find("\\") == string::npos) {
                  cmdfail = true; 
               } // if theres no backslash, there is a space without slash
               initpath += " "; // add space
            }
            initpath += vec.at(vec.size() - 1)->getString();
         }
         else if (vec.size() == 2) { // if there are no spaces in filepath
            if (vec.at(1)->getString() == "-") { // check for prev cd call
               initpath = vec.at(1)->getString(); //chane initpath to PATH
            }
            else { // a file path specified
               initpath += vec.at(1)->getString(); //add PATH onto initpath
            }
         }
         
         for (unsigned i = 0; i < initpath.size(); i++) {
            if (initpath.at(i) != '\\') { // check for backslashes in initpath
               interpath += initpath.at(i); // create initpath without "\"
            }
         }
         // cout << interpath << endl;
         
         if (cmdfail) { // if invalid input
            errno = ENOENT;
            perror("Error (backslash required if spaces in file/folder names)");
            return 1; // throw error, return
         }
         
         string temp = ""; // temp string used to parse filePath
         for (unsigned i = 0; i < interpath.size(); i++) {
            if (interpath.at(i) == '/') { // if at '/' character
               pathVec.push_back(temp); // push back current temp to pathVec
               temp = ""; // reset string temp
            }
            else { // if not slash
               temp += interpath.at(i); // continuosly add to string temp
               if ((i + 1) == interpath.size()) { // if at end of initpath
                  pathVec.push_back(temp); // push whats left onto pathVec
                  temp = ""; // reset string temp
               }
            }
         }
         
         for (unsigned i = 0; i < pathVec.size(); i++) { //iter thru pathVec
            if (pathVec.at(i) != "."){ // if a not '.' or ".."
               path += pathVec.at(i); 
               if(i != pathVec.size() - 1) {
                  path += "/"; // only add "/" if not last element in pathVec
               }
            }
         }
         
         if (path.at(path.size() - 1) == '/') { 
            path = path.substr(0, path.size() - 1);
         } // makes sure to remove any excess "/"'s on the end
         
         if (vec.size() >= 2) { // if not just "cd"
            // cout << vec.size() << endl;
            // string dest = vec.at(1)->getString();
            if (path != "-") { // if not "cd -"
               if (chdir(path.c_str()) != 0) { 
                  // setenv("PWD", getenv("OLDPWD"), 1);
                  perror("CHANGE DIRECTORY FAILED");
                  return 1;
               }
               
               path = ""; // reset path
               for (unsigned i = 0; i < pathVec.size(); i++) {//iter thru pVec
                  if (pathVec.at(i) != "."){ //ignore "." (do nothing if ".")
                     if(pathVec.at(i) != ".."){
                        path += pathVec.at(i);
                        if(i != pathVec.size() - 1) {
                           path += "/";
                        } 
                     }
                     else { //it is ".."
                        if(path.size() != 0){ 
                           path.erase(path.end() - 1);
                           path = path.substr(0, path.find_last_of("/"));
                           if (i != pathVec.size() - 1){
                              path += "/";
                           } // if last element add "/" at the end
                        } // if "..", remove last path element
                     }
                  }
               }
               
               if (path.at(path.size() - 1) == '/') {
                  path = path.substr(0, path.find_last_not_of('/') + 1);
               } // removes last slashes if existent
               
               if (setenv("OLDPWD", getenv("PWD"), 1) != 0) {
                  perror("ENVIRONMENT VARIABLE SETTING FAILED");
                  return 1;
               } // set oldPWD with current PWD
               if (setenv("PWD", path.c_str(), 1)) {
                  perror("ENVIRONMENT VARIABLE SETTING FAILED");
                  return 1; 
               } // set current PWD with user specified PATH
               
               // cout << getenv("PWD") << endl;
            }
            else { //if "cd -"
               char * temp = getenv("PWD"); // grab current PWD
               
               if (setenv("PWD", getenv("OLDPWD"), 1)) {
                  perror("ENVIRONMENT VARIABLE SETTING FAILED");
                  return 1;
               } // set oldPWD with current PWD
               
               string test = getenv("PWD"); // for error checking below
               
               if (test == "") { // if empty, error setting current PWD
                  errno = EINVAL;
                  perror("OLDPWD not set");
                  return 1;
               }
               
               if (chdir(getenv("PWD")) != 0) {
                  perror("CHANGE DIRECTORY FAILED");
                  return 1;
               }
               if (setenv("OLDPWD", temp, 1)) {
                  perror("ENVIRONMENT VARIABLE SETTING FAILED");
                  return 1;
               }
            }
         }
         else { // if "cd"
         
            const char *homedir;
   
            if ((homedir = getenv("HOME")) == NULL) {
                homedir = getpwuid(getuid())->pw_dir;
            } // grab homedirectory
            
            if (setenv("OLDPWD", getenv("PWD"), 1)) {
               perror("ENVIRONMENT VARIABLE SETTING FAILED");
                  return 1;
            } // set oldPWD with current PWD
            if (setenv("PWD", homedir, 1)) {
               perror("ENVIRONMENT VARIABLE SETTING FAILED");
                  return 1;
            } // set current PWD to homedir
            if (chdir(getenv("PWD")) != 0) {
                  perror("CHANGE DIRECTORY FAILED");
                  return 1;
            }
         }
         
         // cout << homedir << endl;
         
         // cout << getenv("PWD") << endl;
         
         // setenv("PWD", vec.at(1)->getString().c_str(), 1);
         
         // chdir(vec.at(1)->getString().c_str());
         
         // cout << getenv("PWD") << endl;
         
         return 0;
      }
   
};

#endif