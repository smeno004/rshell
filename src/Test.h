#ifndef __TEST_H__
#define __TEST_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "BaseCommand.h"

using namespace std;

class Test : public BaseCommand {
   public:
      Test(string cmdStr) {
         root = new Commands(cmdStr);
         root->parse();
      }
      int execute() {
         bool e = false;
         bool filepath = false;
         struct stat sb;
         unsigned size = root->getVec().size();
         
         // cout << "Command size: " << root->getVec().size() << endl;
         
         if(size > 1 && root->getVec().at(size - 1)->getString().at(0) != '-'){
            filepath = true;
         }
         
         if (size == 1
          || (root->getVec().at(1)->getString() == "-e")
          || (size == 2 && filepath)
          || (size == 3 && !filepath)
          || size > 3) {
            e = true;
         }
         
         string filename;
         
         if (!filepath) {
            filename = ".";
         }
         else {
            filename = root->getVec().at(size - 1)->getString();
         }
         
         if (stat(filename.c_str(), &sb) == -1) {
            perror("(False)");
            return 1;
         }
         
         if (e) {
            if (sb.st_mode && S_IFMT) {
               switch (sb.st_mode & S_IFMT) {
                  case S_IFDIR:  printf("(True)\n");     return 0;
                  case S_IFREG:  printf("(True)\n");     return 0;
                  default:       printf("(False)\n");    return 1;
               }
            }
         }
         
         else {
            if (root->getVec().at(1)->getString() == "-d") {
               switch (sb.st_mode & S_IFMT) {
                  case S_IFDIR:  printf("(True)\n");     return 0;
                  default:       printf("(False)\n");    return 1;
               }
            }
            else if(root->getVec().at(1)->getString() == "-f") {
               switch (sb.st_mode & S_IFMT) {
                  case S_IFREG:  printf("(True)\n");     return 0;
                  default:       printf("(False)\n");    return 1;
               }
            }
            else {
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