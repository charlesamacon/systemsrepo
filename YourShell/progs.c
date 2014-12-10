#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "progs.h"

// Tokenizer 2000
int tokenizer(char *c, char *argv[])
{
	// This is so we can keep track of how many elements are actually in argv.
	// I know we could get argc, but this keeps track of how many are actually USED
	int i = 0;
    char buffer[250][250];

	while (*c && isspace(*c))
	{
		c++;
	}
	
	while (*c && i < 255)	// Set the max number of arguments to a static number of 255 (I doubt we'll need more than that right now)
	{
		argv[i + 1] = buffer[i];

		while (*c && !isspace(*c))
		{
			c++;
		}

		while (*c && isspace(*c))
		{
			*(c + 1) = 0;
		}
	}

	argv[i] = NULL;
	return i;
}

// Man
void man(char argv[])
{
	// Output text files based on the argument name
	// I'm thinking this should be done as simply as possible

	char man[50];

	// copy command from argv[] to man[]
	int i;
	for (i = 0; i < 50; i++)
	{
		if (argv[i] != EOF)
		{
			man[i] = argv[i];
		}
	}

	//This is a sequence of escape codes that clears the screen.
	printf("\033[H\033[J");
	
	if (strcmp(man, "-echo") == 0)
	{
		printf("ECHO			User Commands\n\n"
			"Name\n"
			"echo - display a line of text\n\n"
			"Synopsis:\n"
			"echo [STRING]...[$VAR]...\n\n"
			"Description:\n"
			"Echo the STRING(s) to standard output.\n\n"
			"Echo also supports the use of variables.\n\n"
			"Example:\n\n"
			"[SSSH ] variable = 4\n"
			"[SSSH ] echo variable equals $variable\n"
			"variable equals 4\n\n");
	}
	else if (strcmp(man, "-clear") == 0)
	{
		printf("CLEAR			User Commands\n\n"
			"Name\n"
			"clear - clears the screen\n\n"
			"Synopsis:\n"
			"clear\n\n"
			"Description:\n"
			"Properly clears the screen by outputting a bunch of escape codes.\n"
			"The Linux version would cause issues with certain functions.\n\n");
	}
	else if (strcmp(man, "-cpusage") == 0)
	{
		printf("CPUSAGE			User Commands\n\n"
			"Name\n"
			"cpusage - Display the average CPU usage of the past 24 - hour period.\n\n"
			"Synopsis:\n"
			"cpusage[SHORT - OPTION]...\n\n"
			"Description:\n"
			"Displays the current average CPU usage of the past 24 - hour period.\n\n"
			"This is associated with a background process that uses the top command\n"
			"to get a 'snapshot' of the CPU usage every hour and log it.cpusage\n"
			"is then able to access this log and return an averaged value from it.\n");
	}
	else if (strcmp(man, "-man") == 0)
	{
		printf("MAN			User Commands\n\n"
			"Name\n"
			"man - displays the manual page for a given command\n\n"
			"Synopsis:\n"
			"man[SHORT - OPTION]...\n\n"
			"Description:\n"
			"man is the system's manual pager. Each page argument given to man is\n"
			"normally the name of a program, utility or function.The manual page\n"
			"associated with each of these arguments is then found and displayed.\n\n"
			"For the time being, this version of man will display the entire manual\n"
			"page for a given command. Whether or not this will persist in the final\n"
			"project remains to be seen. Short, sweet, and to the point right now.\n\n"
			"Currently implemented man pages :\n"
			"clear\n"
			"cpusage\n"
			"echo\n"
			"man\n"
			"quit\n"
			"strToBinary\n"
			"superBash\n"
			"XOR\n"
			"Examples\n\n"
			"man -XOR\n"
			"Display the manual page for the program XOR.\n");
	}
	else if (strcmp(man, "-superBash") == 0)
	{
		printf("SUPERBASH		User Commands\n\n"
				"Name\n"
				"superBash - translates basic c programs to some bash commands\n\n"
				"Synopsis:\n"
				"superBash[INPUT]...[OPTIONAL OUTPUT]\n\n"
				"Description:\n"
				"superBash allows the user to translate very basic (and very specific)\n"
				"c programs into a format that is usable by bash. This is mainly a translator\n"
				"for programs that make use of loops.\n\n"
				"Example:\n\n"
				"superBash input frank\n"
				"This will read in file input and output to file frank\n");
	}
	else if (strcmp(man, "-strToBinary") == 0)
	{
		printf("STRTOBINARY		User Commands\n\n"
				"Name\n"
				"strToBinary - returns a binary translation of a string\n\n"
				"Synopsis:\n"
				"strToBinary [STRING]\n\n"
				"Description:\n"
				"strToBinary returns a character-by-character binary translation of\n"
				"and ASCII string\n\n"
				"Example:\n\n"
				"strToBinary frank\n"
				"Returns 1100110 1110010 1100001 1101110 1101011\n");
	}
	else if (strcmp(man, "-XOR") == 0)
	{
		printf("XOR			User Commands\n\n"
				"Name\n"
				"XOR - returns the XOR value of two strings\n\n"
				"Synopsis:\n"
				"XOR [STRING 1] ... [STRING 2]\n\n"
				"Description:\n"
				"XOR returns the XOR value (in binary) of two strings to determine\n"
				"whether or not each character is different. XOR takes the length of\n"
				"of the shortest string\n"
				"Example:\n\n"
				"XOR frank franz\n"
				"Returns 0 0 0 0 1\n");
	}
	else if (strcmp(man, "-cd") == 0)
	{
		printf("CD			User Commands\n\n"
				"THIS FEATURE IS NOT YET IMPLEMENTED.\n");
	}
	else if (strcmp(man, "-quit") == 0)
	{
		printf("QUIT			User Commands\n\n"
				"Name\n"
				"quit - quits the shell\n\n"
				"Synopsis:\n"
				"quit\n\n"
				"Description:\n"
				"Frees all memory and exits out of the shell. Like it should.\n");
	}
	else
	{
		printf("Unknown MAN page for %s, please try again.\n", man);
	}


	return;
}


void cpuRead(){ //Call this to print cpu usage
	float loadavg,time;
	FILE* fp;
	
	fp = fopen("cpuStats","r");
	fscanf(fp,"%f %f", &loadavg, &time);
	fclose(fp);
	
	printf("The average cpu utilization over %.0f minutes is %f\n",time, loadavg);
	return;
}

float cpuReadFloat(){
	float loadavg,time;
	FILE* fp;
	
	fp = fopen("cpuStats","r");
	fscanf(fp,"%f %f", &loadavg, &time);
	fclose(fp);
	
	return loadavg;
}

	// Change directory action, "cd"
	// Very, very baby version of it but it's not hard to do. Reading through to find what variables are named what
	// all I have to do is use the chdir declaration and then specify where we want it to go i.e. what is inputted
	// If there is nothing inputted than it takes you back to the home directory or first parent

int cd(char input[]) 
{ 
    int tempV;
    //declare home and UserInput
	//if ( input == " ")
		//tempV = chdir(home);
	//else 
		//tempV = chdir(UserInput);
	
	
	//I'm not inputting another else if because these should return a 0 or -1, if a -1 is returned
	// aka it's false, it will stay where it is at. If it returns a 0, then it will change to that directory
	
	//Still looking into this because it seems too easy thus far....

}

void cpuAverage(){ //Returns first float of the file cpuLoad
	FILE *cpuStats;
	cpuStats = fopen("cpuLoad","r");
	
	char average[5];
	char c;
	int i = 0;
	while((c = fgetc(cpuStats)) != " ") average[i++] = c;
	
	printf("The average CPU usage is: %s\n", average);
	fclose(cpuStats);
}

// SuperBash
void superBash(int argc, char argv[], char out[])
{
	// This is a line for line copy of the superbash program. Except we need to fix the segmentation faults.
	FILE *input;
	FILE *output;
	char c;
	char string[100][100];
	char arr[100][100];
	char placeHolder[3] = " \n";
	int i, k, j, n;
	k = 0;
	i = 0;
	n = 0;

	//----------------------------Open File Input-----------------------------------

	printf("SuperBash: Opening file %s\n", argv);
	input = fopen(argv, "r");  //Opens the file listed on the command line call for reading

	//printf("Copying String\n");
	while ((c = fgetc(input)) != EOF){  //Copies file to 2d character array string
		if (c != '\n'){
			string[k][i] = c;
			i++;
		}
		else{
			string[k][i] = '\n';
			i = 0;
			k++;
			if ((string[k - 1][0] == 'f' && string[k - 1][1] == 'i') || string[k - 1][0] == '}'){
				strcpy(string[k], placeHolder);
				k++;
			}
			/*if(string[k-2][0]=='}'){
			strcpy(string[k],placeHolder);
			k++;
			}
			*/
		}
	}
	string[k][0] = EOF; //Puts EOF at end of the 2d char array

	fclose(input);

	//-----------------------------Repeat Fix------------------------------------

	i = 0;
	k = 0;
	n = 0;

	//Really simple way of doing this
	char repeat[100] = "repeatIndex1=0\nwhile [ $repeatIndex1 - lt $num ]\n";
	char doString[100] = "do\n";
	char doneString[100] = "repeatIndex1=$[$repeatIndex1+1]\ndone\n";

	printf("SuperBash: Converting\n");
	while (string[k][i] != EOF)
	{
		if (string[k][i] == 'r' && string[k][i + 1] == 'e' && string[k][i + 2] == 'p' && string[k][i + 3] == 'e' && string[k][i + 4] == 'a' && string[k][i + 5] == 't')
		{
			strcpy(string[k], repeat);
			k++;
		}
		else if (string[k][i] == '{')
		{
			strcpy(string[k], doString);
			k++;
		}
		else if (string[k][i] == '}')
		{
			strcpy(string[k], doneString);
			k++;
		}
		else k++;
	}
	//-----------------------------If Fix-----------------------------------------

	i = 0;
	k = 0;
	n = 0;

	while (string[k][i] != EOF){
		while (string[k][i] != '\n'){
			if (string[k][i] == 't' && string[k][i + 1] == 'h' && string[k][i + 2] == 'e' && string[k][i + 3] == 'n'){
				string[k][i] = '\n';
				string[k][i + 1] = ' ';
				string[k][i + 2] = ' ';
				string[k][i + 3] = ' ';
				string[k][i + 4] = ' ';
				string[k][i + 5] = ' ';
				string[k][i + 6] = 't';
				string[k][i + 7] = 'h';
				string[k][i + 8] = 'e';
				string[k][i + 9] = 'n';
				string[k][i + 10] = '\n';
				break;
			}
			i++;
		}
		i = 0;
		k++;
	}
	//-----------------------------Declaration Fix--------------------------------

	i = 0;
	k = 0;
	n = 0;
	int before, after;
	while (string[k][i] != EOF){
		while (string[k][i] != '\n'){
			if (string[k][i] == '='){
				if (string[k][i - 1] == ' ' && string[k][i + 1] == ' '){
					for (j = i - 1; string[k][j] != '\n'; j++){
						string[k][j] = string[k][j + 1];
					}
					for (j = i; string[k][j] != '\n'; j++){
						string[k][j] = string[k][j + 1];
					}
				}
			}
			i++;
		}
		i = 0;
		k++;
	}
	//-----------------------------Write to Output-------------------------------

	printf("SuperBash: Outputting to %s\n", out);
	output = fopen(out, "w"); //Creates output file and opens it for writing

	printf("SuperBash: Checking output file %s\n", out);
	while (n<k){ //Temporary code to check file copying and output
		fputs(string[n], output);
		n++;
	}

	printf("SuperBash: Closing output file %s\n", out);
	fclose(output);

	printf("\n");
	return;
}

// Extra Linux Util
int* charToBinary(char var){
	int bit,temp,i;
    int* arr = (int *)malloc(7 * (sizeof(int *)));
	temp = var;
	
	for(i=6;i>=0;i--){
		bit = temp%2;
		temp = temp/2;
		arr[i] = bit;
	}
	return arr;
}

int** strToBinary(char str[]){
	int i,k;
    int len = strlen(str);
    int** arr = (int **)malloc(len * sizeof(int *));
	for(i=0;i<len;i++) arr[i] = (int *)malloc(7*sizeof(int));
	
    int* tempArr = charToBinary('a');
	
    for(i=0;i<len;i++){
		tempArr = charToBinary(str[i]);
        for(k=0;k<7;k++){
            arr[i][k] = *(tempArr+k);
        }
	}
	
	//PRINTS OUT BINARY STRING
	for(i=0;i<len;i++){
		for(k=0;k<7;k++){
			printf("%d",arr[i][k]);
		}
		printf(" ");
	}

	//printBinary(arr, len); 
	return arr;
	
}

//2 strings are xor'd character by character, goes by shorter string length
void xorBinary(char arr1[], char arr2[],int size1, int size2){
	int i,k;
	int size;
	if(size1 > size2) size = size2;
	else size = size1;
	
	/*for(i=0;i<size;i++){
		for(k=0;k<size;k++){
		printf("Looping\n");
			if(arr1[i][k] == arr2[i][k]) printf("0");
			else printf("1");
		}
		printf(" ");
	}*/
	
	for (i = 0; i < size; i++)
	{
		if (arr1[i] == arr2[i])
		{
			printf("0");
		}
		else
		{
			printf("1");
		}
		printf(" ");
	}
	printf("\n");
}

void printBinary(int** arr, int len){
	int i,k;
	for(i=0;i<len;i++){
		for(k=0;k<7;k++){
			printf("%d",arr[i][k]);
		}
		printf(" ");
	}
}