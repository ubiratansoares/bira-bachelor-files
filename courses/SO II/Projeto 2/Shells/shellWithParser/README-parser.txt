There are two functions you need to be aware of.

   1.  parseInfo *parse(char* cmdline);

      Takes in a string cmdline, and returns a pointer to a struct
parseInfo. The members of parseInfo can be seen in parse.h.  Commands
are always stored in the array CommArray. This can accommodate
multiple pipes. In commandType, command is the executable name and
VarList is the argv to pass to the program.  cmdline can end either
with '\n' or '\0'. 

   2.  void free_info(parseInfo *info);
      Frees the struct info. info was allocated inside parse. However,
I have to free it in shell.c because the shell code needs the info
struct. So I recommend that the students always call this function at
the end in the while loop before calling parse again. 

Convention:
   Maximum number of pipes: 		10
   File name size:      		40
   Maximum number of Args: 		10
   Maximum length of a command: 	80

I tried to make it as flexible as possible and can take arbitrary
number of spaces. However, I did not test all weird cases, so it
should work for the right formats. If the parser found some error, it
will always return NULL. 

The parser parses these symbols <, >, |, &.

I tested it with my shell.c. It should work at least for the commands
in the assignment. 
