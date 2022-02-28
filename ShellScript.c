
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
	FILE *file;
	if(argc == 2)
	{
		printf("There is a batchfile named %s", argv[1]);
		fopen(char argv[1], "r") //Reads in batch file commands
		//stuff inbetween in necessary
		fclose(argv[1]); //Closes file
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
		const int LEN = 128;
		int size = LEN * sizeof(char);
		char hostname[LEN];
		char directory[LEN];

		if(gethostname(hostname, size) != 0)
		{
			printf("\nError printing hostname\n");
		}

		printf("\n%s@%s:%s$ ", getenv("USER"), hostname, getcwd(directory, size));
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



char* redirectCommand(char *special, char *line, bool *isRedirect, char *tokens[], char *outputTokens[])
{
	FILE *file1, *file2;
	char c;
	//parse twice, once for input file name and twice for output
	int x = parseInput(special, tokens);
	int y = parseInput(line, outputTokens);

	if(x != 1 || y >= 3)
	{
		*isRedirect = 0;
		printError(); //no tokens or too many arguments 
		return outputTokens[1];
	}
	else
	{
		*isRedirect = 1;
		file1 = fopen(tokens[1], "r"); //Out input file
			if(file1 == NULL){
				printError();
				fclose(file1);
			}
		file2 = fopen(outputTokens[1], "w"); //Our output file
			if(file2 == NULL){
				printError();
				fclose(file2);
			}

		c = fgetc(file1);
		while(c != EOF)  //Copy contents of input file into output file character by character till end of file (EOF)
		{
			fputc(c, file2);
			c = fgetc(file1);
		}
		fclose(file1);
		fclose(file2);

	}
	return "Successful redirect";
}




char* executeCommand(char *cmd, bool *isRedirect, char* tokens[], char* outputTokens[],  bool *isExits)
{
	char* cmdCopy = strdup(cmd); // make a copy of the command

	cmdCopy = strcat(cmdCopy, "\n");
	
	char* outputFile = NULL;	// Create another char* for the output file name, and initialize it to an empty string
	
	char* carrotVar = strchr(cmdCopy, '>');   //  check if a redirect symbol ('>') is used, and store the return in an appropriate variable.

	if (carrotVar != NULL) // If the return is not null, then call redirectCommand, and return  the output file name from this function
	{
		// outputfile = redirectCommand(char *special, char *line, bool *isRedirect, tokens, outputTokens);
		// return outputFile;


		// changeDirectories()

		// printHelp()

		// launchProcesses()
	}
	else
	{
		// tokenCount = parseInput(char *input, char *splitWords[])		// store number of returned tokens
		if (tokenCount == 0)
		{
			return NULL;
		}
		// bool  exitProgram(char *tokens[], int numTokens)

		
		
		
		// if exit, return output file name

		
	}

	
	// If the return is null, call exitProgram and set the exit bool pointer so that main will know to exit the program.

	// If the user has chosen to exit, you should immediately return the output file name.

	// Otherwise, you should call changeDirectories, printHelp, and launchProcesses, and then return the output file name.



	// return output file name
}


// int execvp(const char *file, char *const argv[]);

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
