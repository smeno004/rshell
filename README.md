Rshell
---

Rshell is a program that allows user to enter a flurry of linux commands either
alone or with connectors such as ``&&``, ``||``, and ``;``. The amount of 
commands it can accept are infinte (within the maximum amount of char characters
readble in one line). Rshell comes with user recognition printing out both
username and machiene name before the ``$`` prompt.

###Bugs
#####Below is a list of known bugs, tread with caution:
*  There is no significant error checking for simply entering connectors 
   without any tokens (commands and parameters)
* Space between command and ';' causes error for multicommands.
* exit. period. but not so much period cuz it works in a single command.