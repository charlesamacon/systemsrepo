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

extern static char *my_var[100];			// for echo to have access to.

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

// Echo
void echo(char *argv[], int argc)
{
	// Use printf to print arguments.
	int i;
	int err = 0;
	int j;

	for (i = 1; i < argc - 1; i++)
	{
		if (argv[i][0] == '$')
		{
			err = 1;
			char buffer[15];

			int k;

			for (k = 0; k < 15; k++)
			{
				buffer[k] = argv[i][k];
			}

			for (j = 0; j < 100; j++)
			{
				if (strcmp(buffer, my_var[i]))
				{
					printf("%s%s", my_var[i], " ");
					err = 0;
				}
			}

			if (err == 1)
			{
				printf("Error: Unrecognized Variable\n");
			}
		}
		else if (err = 0)
		{
			printf("%s%s", argv[i], " ");
		}
	}

	printf("%s%s", argv[argc - 1], " ");
	// Basically, we need the ability to assign variables values. We need to create a 2-Dimensional array for variables
	// The user should be able to type "variable=2" and then "echo $variable" and ysh should output "2".
	// So we just need echo to check that variable list, and if the variable exists, it outputs whatever.

	// Seems simple enough:
	// In YSH.c, create 2D array for variables and their data.
	// Pass this array (or a pointer to it, at least) to echo.
	// If there are any strings in *argv[] that start with $, check the rest of that string to all variables stored in the array.
	// If it exists, output its value.
	// Otherwise, output an error message.

	return;
}

// Man
void man(char *argv[])
{
	// Output text files based on the argument name
	// I'm thinking this should be done as simply as possible

	char man[50];

	// copy command from argv[] to man[]

	// if (man == "echo")
	//{
	//	printf("ECHO			User Commands\n\n"
	//		"Name\n"
	//		"echo - display a line of text\n\n"
	//		"Synopsis\n"
	//		"echo[SHORT - OPTION]...[STRING]...\n"
	//		"echo LONG - OPTION\n\n"
	//		"Description\n\n"
	//		"Echo the STRING(s) to standard output.\n\n"
	//		"- n		do not output the trailing newline\n"
	//		"- e 		enable interpretation of backslash escapes\n"
	//		"- E 		disable interpretation of backslash escapes(default)\n"
	//		"--help		display this page and exit\n");
	//}
	//else if (man == "cpusage")
	//{
	//	printf("cpusage			User Commands\n\n"
	//		"Name\n"
	//		"cpusage - Display the average CPU usage of the past 24 - hour period.\n\n"
	//		"Synopsis\n"
	//		"cpusage[SHORT - OPTION]...\n\n"
	//		"Description\n\n"
	//		"Displays the current average CPU usage of the past 24 - hour period.\n\n"
	//		"This is associated with a background process that uses the top command\n"
	//		"to get a 'snapshot' of the CPU usage every hour and log it.cpusage\n"
	//		"is then able to access this log and return an averaged value from it.\n\n"
	//		"Options are currently not implemented or decided upon.\n"
	//		"- cL	clear log\n"
	//		"--help	display this help page and exit\n")
	//}
	//else if (man == "man")
	//{
	//	printf("man			User Commands\n\n"
	//		"Name\n"
	//		"man - displays the manual page for a given command\n\n"
	//		"Synopsis\n"
	//		"man[SHORT - OPTION]...\n\n"
	//		"Description\n\n"
	//		"man is the system's manual pager. Each page argument given to man is\n"
	//		"normally the name of a program, utility or function.The manual page\n"
	//		"associated with each of these arguments is then found and displayed.\n\n"
	//		"For the time being, this version of man will display the entire manual\n"
	//		"page for a given command. Whether or not this will persist in the final\n"
	//		"project remains to be seen. Short, sweet, and to the point right now.\n\n"
	//		"Currently implemented man pages :\n"
	//		"echo\n"
	//		"cpusage\n"
	//		"man\n\n"
	//		"Examples\n\n"
	//		"man ls\n"
	//		"Display the manual page for the program ls.\n")
	//}


	return;
}

// cpuUsage
void cpusage()
{
	// Figure out a way to call Top.

	// As far as I can find, there's not a log that's actually stored somewhere, so we might have to create one and run this as a background (I know, I know...)

	struct sysinfo info;
	float cpuLoad;
	int error;
	int count = 0;
	FILE *cpuStats;
	char string[512];
	char* stats;
	error = sysinfo(&info);
	int i = 0;
	float total;
	char percentFloat[5];
	if (error != 0)
	{
		printf("Error = %d\n", error);
	}

	// Stores cpu load values to a file that is structured so the first value is the average cpu 
	// load over time its been running, all following values are previous loads at 15 minute intervals
	// maxmium stored values is 97, 1 overall average and 96 15 minute averages
	
	while(1){
		cpuStats = fopen("cpuLoad","r"); // Opens cpuLoad where all recorded loads are stored
		total=0;
		
		while((c = fgetc(cpuStats)) != EOF){ //Writes entire file to string
			string[i++] = c;
		}
		
		stats = strtok(string," "); //Tokenizes string into smaller strings containing the load values
		
		percents[count++] = strtof(stats); //Turns the value strings into floats and stores them in an array
		while(stats != NULL){
			stats = strtok(NULL," ");
			percents[count++] = strtof(stats);
		}
		
		i=1;
		while(i<count) total+= percents[i]; //Calculates total of all floats
		fclose(cpuStats); 
		cpuStats = fopen("cpuLoad","w"); //Destroys old cpuLoad file and opens a new one
		
		sprintf(percentFloat,"%f",total/(count-1)); //Converts float to string so it can be written to file
		fputc(total/(count-1))
		i=2;
		while(i<97 && i < count){
			fputc(' ');
			sprintf(percentFloat,"%f",percents[i]);
			fputs(percentFloat);
			i++;
		}
		cpuLoad = info.loads[2];
		sprintf(percentFloat,"%f",cpuLoad);
		fputc(' ');
		fputs(percentFloat);
		fclose("cpuLoad");
		// This will return the 15 minute average
		sleep(60*15);
	}
	// So basically, we append that to the last line of some file.
	// Then, if the file is longer than 96 lines (4 times each hour for 24 hours), we disregard the first line and rewrite the file.
	// Then store the value of each line in an unsigned long array[# of lines], add them up, and divide by [number of lines] for our 24-hour average.
	// While this won't immediately return a 24-hour average, it will at least return an average since the shell has been running.
	return;
}

float cpuAverage(){ //Returns first float of the file cpuLoad
	


}

// SuperBash
void superBash(int argc, char * argv[])
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

	input = fopen(argv[1], "r");  //Opens the file listed on the command line call for reading

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

	output = fopen("output", "w"); //Creates output file and opens it for writing

	while (n<k){ //Temporary code to check file copying and output
		fputs(string[n], output);
		n++;
	}

	fclose(output);

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

int** strToBinary(char* str){
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
	
	/* PRINTS OUT BINARY STRING
	for(i=0;i<len;i++){
		for(k=0;k<7;k++){
			printf("%d",arr[i][k]);
		}
		printf(" ");
	}
	*/
	return arr;
	
}

//2 strings are xor'd character by character, goes by shorter string length
void xorBinary(int** arr1, int** arr2,int size1, int size2){
	int i,k;
	int size;
	if(size1 > size2) size = size2;
	else size = size1;
	
	for(i=0;i<size;i++){
		for(k=0;k<7;k++){
			if(arr1[i][k] == arr2[i][k]) printf("0");
			else printf("1");
		}
		printf(" ");
	}

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

// Redirect
void ioRedirect()
{
	return;
}

// Backgrounding
void background()
{
	// Each process that is done in the background should not produce output, but should still be doing its thing. While other things are going on
	// pid_t, etc.
	return;
}

// Piping
void piping()
{
	// Create an array. Every time a '|' is encountered, move to the next index. Execute all commands at roughly the same time.
	// Should probably be in ysh.c
	return;
}