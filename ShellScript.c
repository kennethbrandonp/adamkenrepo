
// Program name: <yourNetID>kennethbrandonp_ShellScript.c
// Authors: and Kenneth Peterson
// Purpose: Create a shell using C



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>


/*
The program will execute a shell and loop until the user chooses to exit. 

The program should have two possible modes: interactive and batch. 

The default should be interactive (the user inputs a command, and the shell executes the command), 
but if the user provides a batch file name on the command line, 
then the program should execute in batch mode. 

Batch Mode simply reads the commands from a file, 
rather than manually inputting them in the shell (unlike interactive mode).


Additionally, the program should implement its own versions of exit, cd, and help


*/



void  promptUser(bool isBatch);
void  printError();
void  printHelp(char *tokens[], int numTokens);
int   parseInput(char *input, char *splitWords[]);
char* redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char* executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits);
char  getLetter(char *str, int index);
bool  exitProgram(char *tokens[], int numTokens);
void  launchProcesses(char *tokens[], int numTokens, bool isRedirect);
void  changeDirectories(char *tokens[], int numTokens);

int main(int argc, char **argv)
{
	// parse command-line args to determine if a batch file is presented
	if(argc == 2)
	{
		printf("There is a batchfile named %s", argv[argc]);
		FILE *fopen(char argv[argc], 'r') //Reads in batch file commands
	}
	else
	{
		printf("No batchfile");
	}
	
	/*** INTERACTIVE MODE ***/
	// initialize input, output, and file streams to NULL
	
	// declare/initalize vars
	
	// check batch/error status
	
	// display prompt
	
	// parse commands using strtok() (split string into tokens)
	
	promptUser(false);
	
	
	
	
	/*** BATCH MODE ***/ // check argc in main

	// print commands in batch file

	return 0;
}


void  promptUser(bool isBatch)
{
	if (!isBatch)
	{
		//char* name;
		char* hostname;
		char* directory;

		int x = gethostname(hostname, sizeof(hostname));
		directory = getcwd(directory, sizeof(directory));

		//Keep looping until user input is == exit
		//do{
			printf("\n\n%s\n\n", directory);
			printf("\n%s@%s:%s$ ", getenv("username"), hostname, directory);

		//}while(exitProgram(tokens, tokensize) != 0);


		
		
		// CWD: The buf argument should be a pointer to an array at least PATH_MAX bytes long
	}
}

void  printError()
{
	printf("Shell Program Error Encountered\n");
}

void  printHelp(char *tokens[], int numTokens)
{
	int x = strcmp(tokens[0], "help");
	if(numTokens >= 2)
	{
		printError();
	}
	else if(x == 0)
	{

		printf("\nAdam and Ken's example Linux shell\n");
		printf("These shell commands are defined internally.\n");
		printf("help -prints this screen so you can see available shell commands\n");
		printf("cd -changes directories to specified path; if not given, defaults to home\n");
		printf("exit -closes the example shell\n");
		printf("[input] > [output] - pipes input file into output file\n");
		printf("\n And more! If it's not explicitly defined here (or in the documentation for the assignment), then the command should try to be executed by launchProcesses\n");
		printf("That's how we get ls -la to work here!\n\n");

	}
	else
	{
		//do nothing
	}
}

int parseInput(char *input, char *splitWords[])
{
      int wordInd = 0;
      splitWords[0] = strtok(input, " ");
      while(splitWords[wordInd] != NULL)
      {
              splitWords[++wordInd] = strtok(NULL, " ");
      }

      return wordInd;
}



char* redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[]);
char* executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits);



char  getLetter(char *str, int index)
{
	return 'b';
}



bool  exitProgram(char *tokens[], int numTokens)
{
	int x = strcmp(tokens[0], "exit");
	if(numTokens >= 2)
	{
		printError();
	}
	else if(x == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void  launchProcesses(char *tokens[], int numTokens, bool isRedirect)
{

}

void  changeDirectories(char *tokens[], int numTokens)
{
	int x = strcmp(tokens[0], "cd");
	if(x == 0 && numTokens == 2)
	{
		chdir(tokens[1]);
	}
	else if(x == 0 && numTokens == 1 || numTokens > 2)
	{
		printError();
	}

}
