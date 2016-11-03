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
* Space between command and ';' causes error for multicommands.
* exit does not seem to work with multicommands yet. It does not get recognized
  as a valid command. However, exit works with the single command implementation
* At certain times, when various commands are executed in the shell, multiple
  single exit commands seem to be required to exit out of the shell.