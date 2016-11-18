#ifndef __TEST_H__
#define __TEST_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "BaseCommand.h"

using namespace std;

class Test : public BaseCommand { //Command Pattern for the test Command
   public:
      Test(string cmdStr) {
         root = new Commands(cmdStr);
         root->parse();
      }
      /*
      Function Name: execute
      Input Parameters: None
      Return: int
      Description: Executes the test command as follows: test allows us to
      check for certain properties of specified files with flags. -d allows
      us to check if the file specified is a directory, -f allows us to 
      check if a file is a regular file, and -e checks if the file exists.
      */
      int execute() {
         bool e = false; // true if the test is a -e evaluation
         bool filepath = false; // true if the user specifies a filepath
         struct stat sb; // struct used for stat function
         unsigned size = root->getVec().size();
         
         // cout << "Command size: " << root->getVec().size() << endl;
         
         if(size > 1 && root->getVec().at(size - 1)->getString().at(0) != '-'){
            filepath = true; // user has specified a filepath
         }
         
         if (size == 1
          || (root->getVec().at(1)->getString() == "-e")
          || (size == 2 && filepath)
          || (size == 3 && !filepath)
          || size > 3) {
            e = true;
         } /*any of these conditions mean that the test command input by the
         user is to be evaluated as a -e command*/
         
         string filename;
         
         if (!filepath) {
            filename = "."; // current directory
         }
         else { // else set filename to user specified filepath
            filename = root->getVec().at(size - 1)->getString();
         }
         
         if (stat(filename.c_str(), &sb) == -1) {
            perror("(False)"); //throw error if stat fails
            return 1;
         }
         
         if (e) { // if test command is checking for -e
            if (sb.st_mode && S_IFMT) {
               /*As long as either S_IFDIR or S_ISREG is true, then
               the file exists. */
               switch (sb.st_mode & S_IFMT) {
                  case S_IFDIR:  printf("(True)\n");     return 0;
                  case S_IFREG:  printf("(True)\n");     return 0;
                  default:       printf("(False)\n");    return 1;
               }
            }
         }
         
         else { // test command is not checking for -e
            if (root->getVec().at(1)->getString() == "-d") {
               /*If doing a directory check, check S_IFMT */
               switch (sb.st_mode & S_IFMT) {
                  case S_IFDIR:  printf("(True)\n");     return 0;
                  default:       printf("(False)\n");    return 1;
               }
            }
            else if(root->getVec().at(1)->getString() == "-f") {
               /* If doing a regular file check, check S_IFREG*/
               switch (sb.st_mode & S_IFMT) {
                  case S_IFREG:  printf("(True)\n");     return 0;
                  default:       printf("(False)\n");    return 1;
               }
            }
            else { // if not -e -d or -f then the input must be invalid
               perror("Invalid flag");
               return 1;
            }
         }
         
         // cout << "Filepath: " << filepath << endl;
         // cout << "E: " << e << endl;         
         return 0;
      }
};

#endif