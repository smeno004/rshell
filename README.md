Rshell
---

Rshell is a program that allows users to enter a flurry of linux commands either
alone or with connectors such as ``&&``, ``||``, and ``;``. The amount of 
commands it can accept are infinite (within the maximum amount of char characters
readable in one line). Rshell comes with user recognition, printing out both
username and machine name before the ``$`` prompt.

**Update v 1.1** allows users to have precedence to their commands, meaning that 
users can specify the precedence of the returns of their commands. For Example,
the command `echo A && echo B || echo C && echo D` will print out:

 >A <br>
 >B <br>
 >D <br>
 
However, with precedence, the command `(echo A && echo B) || (echo C && echo D)`
will print out:

>A <br>
>B <br>

This update also adds functionality to the `test` command. The result of the
test command call now outputs the result of the test as either

>(True) <br>

or

>(False) <br>

###Bugs
#####Below is a list of known bugs, tread with caution:
* When parentheses are used, if no connector is used following the parentheses
  like in the command `(ls || echo A) ls  -a`, vector out of range is thrown.
* For the command `lsa && ls && ls || ls -a`, ls -a does not print when it 
  should, we are not completely sure why :(
* `ls && && &&` throws a vector out of range error while commands like 
  `ls || ; &&` do not.