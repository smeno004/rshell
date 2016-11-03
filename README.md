Rshell
---

Rshell is a program that allows user to enter a flurry of linux commands either
alone or with connectors such as ``&&``, ``||``, and ``;``. The amount of 
commands it can accept are infinite (within the maximum amount of char characters
readable in one line). Rshell comes with user recognition, printing out both
username and machine name before the ``$`` prompt.

###Bugs
#####Below is a list of known bugs, tread with caution:
* There is no significant error checking for simply entering connectors 
  without any tokens (commands and parameters)
    * solely entering connectors such as ``&&`` without any other characters 
      results in an out of range error for a vector as seen below.
      ``[ubuntu@andre_castro-cs_private-1980025]$ &&#
        terminate called after throwing an instance of 'std::out_of_range'
        what():  vector::_M_range_check
        Aborted
      ``
    * This may possibly be fixed by adding a check to the incrementation of the 
      variable ```c++ NUM_SIZE += 1;``` to only increment if the last element 
      is not a connector.
      ```c++
        if (vec.at(vec.size() - 1)->getString() != ";") {
            NUM_SIZE += 1;
        }```
* Space between command and ';' causes error for multicommands.
* exit does not seem to work with multicommands yet. It does not get recognized
  as a valid command. However, exit works with the single command implementation.
  For example:
  ``[ubuntu@andre_castro-cs_private-1980025]$ exit; ls
    EXECVP FAILED: No such file or directory
    Makefile  README.md  a.out  bin  hw2  src  test
    [ubuntu@andre_castro-cs_private-1980025]$ 
  ``
* At certain times, when various commands are executed in the shell, multiple
  single exit commands seem to be required to exit out of the shell.
* In hammer, ls doest not appear to work. Upon calling ls, the following error
  message is output: ``ls: cannot access P6▒: No such file or directory``. The 
  ls command works just fine in other IDE's without any error messages.
