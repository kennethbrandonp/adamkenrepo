
// Program name: adamwhurdkennethbrandonp_ShellScript.c
// Authors: Adam Whurd and Kenneth Peterson
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


void trimNewline(char* string, int stringLength);
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
	FILE *inFile = NULL;
	FILE *outFile = NULL;
	FILE *redirectFile = NULL;
	const int bufferLength = 256;
	char buffer[bufferLength];

	//char* currentLine = NULL;
	//char* currentToken = NULL;

	//char inputString[128] = {""};
	int  tokenCount = 0;

	bool *isRedirect = calloc(1, sizeof(bool));
	bool *isExits = calloc(1, sizeof(bool));

	*isRedirect = false;
	*isExits = false;

	//char outputTokens[8][32];
	//char inputTokens[8][32]; 	// just doing static allocation for now

	pid_t processID = getpid();	// get process ID of this program


	/*** BATCH MODE ***/
	if(argc == 2)
	{
		char tokens[1][32] = {{""}};
		printf("There is a batchfile named %s", argv[1]);
		inFile = fopen(argv[1], "r"); //Reads in batch file commands
		if(inFile) //If we can open the file
		{
			while(!feof(inFile)) //Reads file into a string
			{
				fscanf(inFile, "%s ", buffer);
				printf("%s \n", buffer);
			}

			tokenCount = parseInput(buffer, tokens); //Parse our string into buffer
			launchProcesses(*buffer, tokenCount, isRedirect); //Launch the process for each token			
		}

		fclose(argv[1]); //Closes file
		char *batchEnd[1][6] = {"exit"}; //send a command to end program
		bool isExit = exitProgram(*batchEnd, 2);
	}
	else
	{
		printf("No batchfile");
	}

	/*** INTERACTIVE MODE ***/
	// check batch/error status

	while(true)	// loop until exitProgram() is called
	{
		char tokens[1][32] = {{""}};
		char outputTokens[1][32] = {{""}};
		promptUser(false);
		fgets(buffer, bufferLength, stdin);

		trimNewline(buffer, bufferLength);
		outFile = executeCommand(buffer, isRedirect, tokens, outputTokens, isExits);

		if(*isExits)
		{
			int x = kill(processID, 15);	// 15 is signal value for "termination signal"
		}
		*isRedirect = false;	//re-initalize if used
	}

	free(isRedirect);
	free(isExits);

	return 0;
}

void trimNewline(char* string, int stringLength)
{
	for (int i = 0; i < stringLength; ++i)
	{
		if(string[i] == '\n')
		{
			string[i] = '\0';
			return;
		}
	}
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
	printf("Shell Program Error Encountered");
}

void  printHelp(char *tokens[], int numTokens)
{
	int x = strcmp(tokens[0], "help\n");
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
}	// returns number of parsed arguments



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

	cmdCopy = strcat(cmdCopy, "\n"); // append newline so sys calls are recognized
	//char* cmdCopy = cmd;

	char* outputFile = "";	// Create another char* for the output file name, and initialize it to an empty string

	char* carrotVar = strchr(cmdCopy, '>');   //  check if a redirect symbol ('>') is used, and store the return in an appropriate variable.

	if (carrotVar != NULL) // If the return is not null, then call redirectCommand, and return  the output file name from this function
	{
		*carrotVar = '>';
		*isRedirect = true;
		outputFile = redirectCommand(carrotVar, cmdCopy, isRedirect, tokens, outputTokens);
		return outputFile;
	}
	else
	{
		int tokenCount = parseInput(cmdCopy, tokens); // store number of returned tokens
		if (tokenCount == 0)
		{
			return outputFile;
		}
		// Execute command
		else if (strcmp(tokens[0], "cd") == 0)
		{
			changeDirectories(tokens, tokenCount);
		}
		else if (strcmp(tokens[0], "help\n") == 0)
		{
			printHelp(tokens, tokenCount);
		}
		else if (strcmp(tokens[0], "exit\n") == 0)
		{
			*isExits = exitProgram(tokens, tokenCount);
		}
		else if (strcmp(tokens[0], "ls\n") == 0 || strcmp(tokens[0], "clear\n") == 0)
		{
			//printf("execvp() stuff here");
			launchProcesses(tokens, tokenCount, *isRedirect);
		}
		else
		{
			printError();
		}
	}

	return outputFile;
}


// int execvp(const char *file, char *const argv[]);

char  getLetter(char *str, int index)
{
	return 'b';
}



bool  exitProgram(char *tokens[], int numTokens)
{
	int x = strcmp(tokens[0], "exit\n");
	if(numTokens > 2)
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
	int x;
	int child;
	for(int i = 1; i < numTokens; i++) //Create child process using fork()
	{
		child = fork();
		if (child >= 0)
		{
			printf( "Child Process: %d created\n", child);//Check if it was made
		}

		isRedirect = strcmp(tokens[0], "execvp"); //If our first provided command is exevp then we know it's a redirect

		//if(isRedirect == 0)
		{
			x = execvp(tokens[0], tokens[1]); //Store the return from execvp
			if(x == -1) //Unsuccessful return
			{
				printError();
			}

			/*
			else if(tokens[i] == "help") //If a command is help
			{
				printHelp(tokens, numTokens);
			}
			else if(tokens[i] == "cd") //If a command is change directories
			{
				changeDirectories(tokens, numTokens);
			}
			else if(tokens[i] == "exit") //If a command is to exit
			{
				exitProgram(tokens, numTokens);
			}
			*/	// all of this is handled in executeCommand 

			wait(NULL); //Wait for execvp to get done then go back to parent process.
		}
	}
	/*
	child = fork();
	if (child >= 0)
	{
		printf("Child Process: %d created\n", child);
	}
	x = execvp(tokens[0], "");
	if(x == -1)
	{
		printError();
	}
	wait(NULL);
	*/
}

void changeDirectories(char *tokens[], int numTokens)
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
