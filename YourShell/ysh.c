// Group 13

// The Super-Simple Shell. Named Ironically.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "progs.h"

// Globals
extern int errno;
int outRedir = 0;		// For checking to see if output needs to be redirected.
typedef void (*sighandler_t)(int);
struct variable
{
	char varName[25];
	char varData[25];
};
static char *my_argv[100], *my_envp[100], *my_var[100];			// Added array for variables (for echo to use)
static char *temp_argv[100];
static char *search_path[10];
static struct variable myvar[100];
int outBool = 0;
int inBool = 0;
int pipeBool = 0;				
int outCheck = 1;
int inCheck = 1;
int pipeCheck = 1;
int pipeIndex = 0;

void handle_signal(int signo)
{
    printf("\n[SSSH ] ");
    fflush(stdout);
}

void fill_argv(char *tmp_argv)
{
    char *foo = tmp_argv;
    int index = 0;
    char ret[100];
    bzero(ret, 100);
    while(*foo != '\0') {
        if(index == 10)
            break;

        if(*foo == ' ') {
            if(my_argv[index] == NULL)
                my_argv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
            else {
                bzero(my_argv[index], strlen(my_argv[index]));
            }
            strncpy(my_argv[index], ret, strlen(ret));
            strncat(my_argv[index], "\0", 1);
            bzero(ret, 100);
            index++;
        } else {
            strncat(ret, foo, 1);
        }
        foo++;
        /*printf("foo is %c\n", *foo);*/
    }
    my_argv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
    strncpy(my_argv[index], ret, strlen(ret));
    strncat(my_argv[index], "\0", 1);
}

void fill_temp_argv(int index)
{
	int i = 0;
	int j = index;
	
	//printf("While not null\n");
	while (my_argv[j] != NULL)
	{
		if (strcmp(my_argv[j],"|") != 0)
		{
			//printf("Copy\n");
			temp_argv[i] = my_argv[j];
			//strncpy(temp_argv[i], my_argv[j], strlen(my_argv[i]));
			//printf("Iterate\n");
			i++;
			j++;
		}
		else
		{
			break;
		}
	}
}

void copy_envp(char **envp)
{
int index = 0;
for (; envp[index] != NULL; index++) {
	my_envp[index] = (char *)
		malloc(sizeof(char) * (strlen(envp[index]) + 1));
	memcpy(my_envp[index], envp[index], strlen(envp[index]));
}
}

void get_path_string(char **tmp_envp, char *bin_path)
{
	int count = 0;
	char *tmp;
	while (1) {
		tmp = strstr(tmp_envp[count], "PATH");
		if (tmp == NULL) {
			count++;
		}
		else {
			break;
		}
	}
	strncpy(bin_path, tmp, strlen(tmp));
}

void insert_path_str_to_search(char *path_str)
{
	int index = 0;
	char *tmp = path_str;
	char ret[100];

	while (*tmp != '=')
		tmp++;
	tmp++;

	while (*tmp != '\0') {
		if (*tmp == ':') {
			strncat(ret, "/", 1);
			search_path[index] =
				(char *)malloc(sizeof(char) * (strlen(ret) + 1));
			strncat(search_path[index], ret, strlen(ret));
			strncat(search_path[index], "\0", 1);
			index++;
			bzero(ret, 100);
		}
		else {
			strncat(ret, tmp, 1);
		}
		tmp++;
	}
}

int attach_path(char *cmd)
{
	char ret[100];
	int index;
	int fd;
	bzero(ret, 100);
	for (index = 0; search_path[index] != NULL; index++) {
		strcpy(ret, search_path[index]);
		strncat(ret, cmd, strlen(cmd));
		if ((fd = open(ret, O_RDONLY)) > 0) {
			strncpy(cmd, ret, strlen(ret));
			close(fd);
			return 0;
		}
	}
	return 0;
}

void call_execve(char *cmd)
{
    int i;
	// Only run if last command in argv is not '&'
	printf("cmd is %s\n", cmd);
	if (fork() == 0) 
	{
			// Need to create a boolean type thing to check this
			if (outRedir == 1)
			{
				//int fd = open([FILENAME], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

				//dup2(fd, 1);	// make stdout go to file
				
				//close (fd);

				//outRedir = 0;	// Reset this
			}
			i = execve(cmd, my_argv, my_envp);
			printf("errno is %d\n", errno);
			if (i < 0) {
				printf("%s: %s\n", cmd, "command not found");
				exit(1);
			}
	}
	else {
		wait(NULL);
	}
}

void call_execve2(char *cmd)
{
    int i;
	// Only run if last command in argv is not '&'
	printf("cmd is %s\n", cmd);
	if (fork() == 0) 
	{
			// Need to create a boolean type thing to check this
			if (outRedir == 1)
			{
				//int fd = open([FILENAME], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

				//dup2(fd, 1);	// make stdout go to file
				
				//close (fd);

				//outRedir = 0;	// Reset this
			}
			i = execve(cmd, temp_argv, my_envp);
			printf("errno is %d\n", errno);
			if (i < 0) {
				printf("%s: %s\n", cmd, "command not found");
				exit(1);
			}
	}
	else {
		wait(NULL);
	}
}

void call_execve_background(char *cmd)
{
	// Basically just seperates Parent process and Child Process

	// Only run if last command in argv is '&'
	int i;
	pid_t pid;
	int status;

	switch (pid = fork())
	{
	case -1:	// Error
		printf("Errno is %d\n", errno);
		break;
	case 0:
					i = execve(cmd, my_argv, my_envp);
			if (i < 0)
			{
				printf("%s: %s\n", cmd, "command not found");
				exit(1);
			}
			else
			{
				waitpid(pid, &status, WNOHANG);	// Fork() returns new pid to parent
			}
	}

	printf("Process ID %d\n", pid);

	return;
}

// From gnu.org/software/libc/manual/html_node/Foreground-and-Background.html
/* //Not seeing this used anywhere
void put_job_in_background(job *j, int cont)
{
	if (cont)
	{
		if (kill(-j->pgid, SIGCONT) < 0)
		{
			perror("kill (SIGCONT)");
		}
	}
}
*/

void free_argv()
{
	int index;
	for (index = 0; my_argv[index] != NULL; index++) {
		bzero(my_argv[index], strlen(my_argv[index]) + 1);
		my_argv[index] = NULL;
		free(my_argv[index]);
	}
}

void assign_variable(char var[], char value[])
{
	// This should only be called when there is an = in the command line
	// Example:
	// MY_SHELL> var = value
	// This should store var in myvar[i].varName and value in myvar[i].varData.
	// We are then able to manipulate said data in echo (which is being moved over to this file to keep from messing with externs).
	
	strncpy(myvar[0].varName,"PATH",strlen("PATH"));
	strncpy(myvar[0].varData,"/usr/bin/clear",strlen("/usr/bin/clear"));

	//We have to declare this outside of a for loop in C
	//C++ lets you declare it as part of the for loop declaration, C does not, however.
	//-CAM
	int i;
	int found = 0;
	
	for (i = 1; i < 100; i++)
	{
		if (strcmp(myvar[i].varName, var) == 0)
		{
			int r = 0;
			
			for (r; r < 15; r++)
			{
				myvar[i].varData[r] = '\0';
			}
			strncpy(myvar[i].varName, var, strlen(var));
			strncpy(myvar[i].varData, value, strlen(value));
			found = 1;
			//my_var[i].var = var;
			//my_var[i][0] = value;
			break;
		}
	}
	
	if (found == 0)
	{
	for (i= 1; i < 100; i++)
	{
		if (myvar[i].varName[0] == '\0')
		{
			// This doesn't make sense, and I'm exhausted from work. Can anyone pick this up? Basically, we need to store var and its value, but I can't put 2 and 2 together right now.
			strncpy(myvar[i].varName, var, strlen(var));
			strncpy(myvar[i].varData, value, strlen(value));
			//my_var[i].var = var;
			//my_var[i][0] = value;
			break;
		}

	}
	}
}

void redir_in(char file[])
{
	// if redirecting I/O

	// Example:
	// sort < input > output

	// Input
	int in;
	in = open(file, O_RDONLY);
	dup2(in, STDIN_FILENO);
	close(in);
	inBool = 0;
	inCheck = 1;
	return;
}

void redir_out(char file[])
{
	// Output (this needs to be read from STD OUT)
	int out;
	out = open(file, O_WRONLY | O_CREAT, 0666);
	//write(out, output, strlen(output));
	dup2(out, STDOUT_FILENO);
	close(out);
	outBool = 0;
	inCheck = 1;
	return;
}
void execute_function(int argIndex, int out, int in)
{

	//We should only call functions this way.
	//We'll need another method for calling cmds (call_exceve)
	//We just need to figure out a way to differentiate between them
	int saved_stdout;
	int saved_stdin;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);

		char *cmd = (char *)malloc(sizeof(char) * 100);
		if (strcmp(my_argv[argIndex],"echo") == 0 || strcmp(my_argv[argIndex],"cpusage") == 0 || 
		strcmp(my_argv[argIndex],"superBash") == 0 || strcmp(my_argv[argIndex],"strToBinary") == 0 ||
		strcmp(my_argv[argIndex],"XOR") == 0 || strcmp(my_argv[argIndex],"cd") == 0 || 
		strcmp(my_argv[argIndex],"man") == 0 || strcmp(my_argv[argIndex],"quit") == 0)
		{
			printf("Executing function: %s\n", my_argv[argIndex]);
			
			//XOR
			if (strcmp(my_argv[argIndex], "XOR") == 0)
			{
				saved_stdout = dup(STDOUT_FILENO);
				if(my_argv[argIndex + 3] != NULL)
				{
					if (strcmp(my_argv[argIndex + 3], ">") == 0)
					{
						if (my_argv[argIndex + 4] != NULL)
						{
							redir_out(my_argv[argIndex + 4]);
							xorBinary(my_argv[argIndex + 1], my_argv[argIndex + 2], strlen(my_argv[argIndex + 1]), strlen(my_argv[argIndex + 2])); 
							fflush(stdout);
						}
					}
				}
				else
				{
						xorBinary(my_argv[argIndex + 1], my_argv[argIndex + 2], strlen(my_argv[argIndex + 1]), strlen(my_argv[argIndex + 2])); 
				}
										
				dup2(saved_stdout, 1);
				close(saved_stdout);
			}// END XOR
			
			// strToBinary
			else if (strcmp(my_argv[argIndex], "strToBinary") == 0)
			{
				saved_stdout = dup(STDOUT_FILENO);
				if (my_argv[argIndex + 2] != NULL)
				{
					if (strcmp(my_argv[argIndex + 2], ">") == 0)
					{
						if (my_argv[argIndex + 3] != NULL)
						{
							// Redirect output to a file.
							redir_out(my_argv[argIndex + 3]);
							int** p = strToBinary(my_argv[argIndex + 1]);
							fflush(stdout);
						}
					}
				}
				else
				{
					int** p = strToBinary(my_argv[argIndex + 1]);
				}
				dup2(saved_stdout, 1);
				close(saved_stdout);
				printf("\n");
			} // end strToBinary
			 
			 // echo
			else if (strcmp(my_argv[argIndex], "echo") == 0)
			{
												//echo(my_argv, argc+1);
									saved_stdout = dup(STDOUT_FILENO);
									int it = 1;
									int it2 = 1;
									int redirOut = 0;
									int myargc2 = 0;
									
									for (it2; my_argv[argIndex + it2] != NULL; it2++)
									{
										if (strcmp(my_argv[argIndex + it2], ">") == 0)
										{
											redirOut = 1;
										}
										myargc2++;
									}
									
									if (redirOut == 1)
									{
										redir_out(my_argv[argIndex + myargc2]);
										for (it; my_argv[argIndex + it] != NULL; it++)
										{
											if (my_argv[argIndex + it][0] == '$')
											{
												// Check and print variable.
												//printf("VAR ");
											
												int err = 1;
												int j;
												char buffer[15];

												int k;

												for (k = 0; k < 15; k++)
												{
													if (my_argv[argIndex + it][k] != '\0')
													{
														buffer[k] = my_argv[argIndex + it][k+1];
													}
												}

												for (j = 0; j < 100; j++)
												{
													//printf("%d\n", j);
													//printf("%s\n", myvar[j].varName);
													if (strcmp(buffer, myvar[j].varName) == 0)
													{
														printf("%s%s", myvar[j].varData, " ");
														err = 0;
													}
												}
	
												if (err == 1)
												{
													printf("$UNKNOWN_VAR ");
												}
											}
											else
											{
												if (it < myargc2 - 1)
												{
													printf("%s ", my_argv[argIndex + it]);
												}
											}
										}
										fflush(stdout);
									}
									else
									{
										for (it; my_argv[argIndex + it] != NULL; it++)
										{
											if (my_argv[argIndex + it][0] == '$')
											{
												// Check and print variable.
												//printf("VAR ");
											
												int err = 1;
												int j;
												char buffer[15];

												int k;

												for (k = 0; k < 15; k++)
												{
													if (my_argv[argIndex + it][k] != '\0')
													{
														buffer[k] = my_argv[argIndex + it][k+1];
													}
												}

												for (j = 0; j < 100; j++)
												{
													//printf("%d\n", j);
													//printf("%s\n", myvar[j].varName);
													if (strcmp(buffer, myvar[j].varName) == 0)
													{
														printf("%s%s", myvar[j].varData, " ");
														err = 0;
													}
												}
	
												if (err == 1)
												{
													printf("$UNKNOWN_VAR ");
												}
											}
											else
											{
												printf("%s ", my_argv[argIndex + it]);
											}
										}
									}
									dup2(saved_stdout, 1);
									close(saved_stdout);
									printf("\n");

			} // end echo
			
			// man
			else if (strcmp(my_argv[argIndex], "man") == 0)
			{
			saved_stdout = dup(STDOUT_FILENO);
									saved_stdin = dup(STDIN_FILENO);
									if (my_argv[argIndex + 1] != NULL)
									{
										if (my_argv[argIndex + 2] != NULL)
										{
											if (outBool == 1 && inBool == 0)
											{
												if (my_argv[argIndex + 3] != NULL)
												{
													redir_out(my_argv[argIndex + 3]);
													//change directory
													man(my_argv[argIndex + 1]);
												}
											}
											else if (outBool == 0 && inBool == 1)
											{
												printf("Input\n");
												FILE *input;
												char c;
												char temp[25];
												char temp2[25];
												int k = 0;
												printf("Init Input\n");
												input = fopen(my_argv[argIndex + 2], "r");
												
												if (input == NULL)
												{
													// For Some reason, it doesn't always work.
													printf("Error opening file\n");
													printf("Sometimes you have to try again\n");
												}
												else
												{
													printf("Input not null\n");
													int l = 0;
													while((c = fgetc(input)) != EOF)
													{
														if (c != '\n')
														{
															temp[k] = c;
															k++;
														}
													else
													{
														//Don't
													}
												}
												fclose(input);
												fflush(stdin);
												strncpy(temp2,temp,k);
												printf("%s\n%s\n",temp, temp2);
												man(temp2);
												//fclose(input);
												}
											}
											else if (outBool == 1 && inBool == 1)
											{
												// Do both (input then output)
												FILE *input;
												char c;
												char temp[25];
												char temp2[25];
												int k = 0;
												input = fopen(my_argv[argIndex + 2], "r");
												
												if (input == NULL)
												{
													printf("Error opening file\n");
													printf("Sometimes you have to try again\n");
												}
												else
												{
												while((c = fgetc(input)) != EOF)
												{
													if (c != '\n')
													{
														temp[k] = c;
														k++;
													}
													else
													{
														//Don't
													}
												}
												fclose(input);
												fflush(stdin);
												strncpy(temp2,temp,k);
												redir_out(my_argv[argIndex + 4]);
												man(temp2);
												fflush(stdout);
												}
											}
										}
										else
										{
											man(my_argv[argIndex + 1]);
										}
									}
									else
									{
										man("ERR");
									}
									
									dup2(saved_stdout, 1);
									close(saved_stdout);
									
									dup2(saved_stdin, 0);
									close(saved_stdin);
			} // end man
			
			// superBash
			else if (strcmp(my_argv[argIndex], "superBash") == 0)
			{
				if (my_argv[argIndex + 2] != NULL)
				{
					superBash(0, my_argv[argIndex + 1], my_argv[argIndex + 2]);
				}
				else
				{
					superBash(0, my_argv[argIndex + 1], "output");
				}				
			} // end superBash
			
			// cd
			else if (strcmp(my_argv[argIndex], "cd") == 0)
			{
				saved_stdout = dup(STDOUT_FILENO);
				if(outBool = 1)
				{
					redir_out(my_argv[argIndex + 2]);
					printf("cd\n");
				}
				else
				{
					//change directory
					printf("cd\n");
				}
			} // end cd
			else if (strcmp(my_argv[argIndex], "cpusage") == 0)
			{
				saved_stdout = dup(STDOUT_FILENO);
				if(outBool = 1)
				{
					redir_out(my_argv[argIndex + 2]);
					printf("Cpusage\n");
				}
				else
				{
					//CPUsage
					printf("Cpusage\n");
				}
			}
			// Flush stdout and reset its file descriptor.
			fflush(stdout);
			dup2(saved_stdout, 1);
			close(saved_stdout);
		}
		else if (strcmp(my_argv[argIndex], "|") == 0)
		{
			printf("Error: Unrecognized Command\n");
		}
		else
		{
			if (my_argv[argIndex - 1] != NULL)
			{
				if (strcmp(my_argv[argIndex - 1], "|") != 0)
				{
					execute_function (argIndex - 1, out, in);
				}
				else
				{
					printf("Move to cmd2\n");
					strncpy(cmd, my_argv[argIndex], strlen(my_argv[argIndex]));
					strncat(cmd, "\0", 1);
					fill_temp_argv(argIndex);
					if (attach_path(cmd) == 0)
					{
					// Incidentally, this actually works better than the original
					// YSH method of handling Linux commands. For whatever reason,
					// this actually allows for command line options.
						call_execve2(cmd);
					}
				}
			}
			else
			{
				printf("Move to cmd\n");
				strncpy(cmd, my_argv[argIndex], strlen(my_argv[argIndex]));
				strncat(cmd, "\0", 1);
				printf("Fill Temp Argv\n");
				fill_temp_argv(argIndex);
				if (attach_path(cmd) == 0)
				{
					printf("Call Execve 2\n");
					call_execve2(cmd);
				}
				//printf("Error22: Unrecognized Command\n");
			}
		}
	return;
}

void execute_pipe(int arg1Index, int arg2Index, int pipeI)
{
	// This is to be reworked by Jordan.
	// While this is the general idea of the thing, a much simpler method was figured out on 11/25/2014.
	
	printf("Executing Pipe\n");

	
	int i = 0;
	int j = 0;
	
	//pipe(fds);
	
	if (strcmp(my_argv[arg1Index],"echo") == 0 || strcmp(my_argv[arg1Index],"cpusage") == 0 || 
		strcmp(my_argv[arg1Index],"superBash") == 0 || strcmp(my_argv[arg1Index],"strToBinary") == 0 ||
		strcmp(my_argv[arg1Index],"XOR") == 0 || strcmp(my_argv[arg1Index],"cd") == 0 || 
		strcmp(my_argv[arg1Index],"man") == 0 || strcmp(my_argv[arg1Index],"quit") == 0)
		{
			printf("Executing function: %s\n", my_argv[arg1Index]);
			execute_function(arg1Index, 1, 0);
			i = 1;
		}
	
	if (strcmp(my_argv[arg2Index],"echo") == 0 || strcmp(my_argv[arg2Index],"cpusage") == 0 || 
		strcmp(my_argv[arg2Index],"superBash") == 0 || strcmp(my_argv[arg2Index],"strToBinary") == 0 ||
		strcmp(my_argv[arg2Index],"XOR") == 0 || strcmp(my_argv[arg2Index],"cd") == 0 || 
		strcmp(my_argv[arg2Index],"man") == 0 || strcmp(my_argv[arg2Index],"quit") == 0)
		{
			printf("Executing function: %s\n", my_argv[arg2Index]);
			execute_function(arg2Index, 0, 1);
			j = 1;
		}
		
	if (i == 0 && j == 0)
	{
	char *cmd = (char *)malloc(sizeof(char) * 100);
	char *cmd2 = (char *)malloc(sizeof(char) * 100);
	int fds[2];
	int status;
	pipe(fds);
	if (fork())
	{
		//dup2(fds[1],STDOUT_FILENO);
		//execute_function(arg1Index, 1, 0);
		strncpy(cmd, my_argv[arg1Index], strlen(my_argv[arg1Index]));
					strncat(cmd, "\0", 1);
					fill_temp_argv(arg1Index);
					if (attach_path(cmd) == 0)
					{
					// Incidentally, this actually works better than the original
					// YSH method of handling Linux commands. For whatever reason,
					// this actually allows for command line options.
						call_execve2(cmd);
					}
	}
	else
	{
		//dup2(fds[0], STDIN_FILENO);
		//execute_function(arg2Index, 0, 1);
		strncpy(cmd2, my_argv[arg2Index], strlen(my_argv[arg2Index]));
					strncat(cmd2, "\0", 1);
					fill_temp_argv(arg2Index);
					if (attach_path(cmd2) == 0)
					{
					// Incidentally, this actually works better than the original
					// YSH method of handling Linux commands. For whatever reason,
					// this actually allows for command line options.
						call_execve2(cmd2);
					}
	}

	int c = close(fds);
	}
	//execute_function(arg1Index, 1, 0);
	//execute_function(arg2Index, 0, 1);
	
	//if (fork() == 0)
	//{
		//dup2(fds[1], STDOUT_FILENO);
		
		//call_execve(arg1);
	//}
	//else
	//{
		//dup2(fds[0], STDIN_FILENO);
		
		//call_exceve(arg2);
	//}
	return;
}





int main(int argc, char *argv[], char *envp[])
{
    char c;
	int i;

	int saved_stdout;
	int saved_stdin;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
    char *tmp = (char *)malloc(sizeof(char) * 100);
    char *path_str = (char *)malloc(sizeof(char) * 256);
    char *cmd = (char *)malloc(sizeof(char) * 100);
    
    signal(SIGINT, SIG_IGN);
    signal(SIGINT, handle_signal);

    copy_envp(envp);

    get_path_string(my_envp, path_str);   
    insert_path_str_to_search(path_str);

    if(fork() == 0) {
        execve("/usr/bin/clear", argv, my_envp);
		// I think this is where we should call the background function for cpusage();
        exit(1);
    } else {
        wait(NULL);
    }
    printf("[SSSH ] ");
    fflush(stdout);

    while(c != EOF) {
        c = getchar();
        switch(c) {
            case '\n': if(tmp[0] == '\0') 
						{
							printf("[SSSH ] ");
						} 
					   else 
					   {
							fill_argv(tmp);
							strncpy(cmd, my_argv[0], strlen(my_argv[0]));
							strncat(cmd, "\0", 1);
							
							int myargc = 0;
							int argcounter = 0;
							for (argcounter; my_argv[argcounter] != NULL; argcounter++)
							{
								myargc++;
							}
							
							if(index(cmd, '/') == NULL)		
							{			
								
								
								saved_stdout = dup(STDOUT_FILENO);
								
								// Check to see if we're redirecting output.
								for (outCheck; outCheck < myargc; outCheck++)
								{
									if (my_argv[outCheck] != NULL)
									{
										if (strcmp(my_argv[outCheck], ">") == 0)
												{
													if (my_argv[outCheck + 1] != NULL)
													{
														outBool = 1;
													}
												}
									}
								}
								
								outCheck = 1;
								
								// Check to see if we're redirecting input
								for (inCheck; inCheck < myargc; inCheck++)
								{
									if (my_argv[inCheck] != NULL)
									{
										if (strcmp(my_argv[inCheck], "<") == 0)
												{
													if (my_argv[inCheck + 1] != NULL)
													{
														inBool = 1;
													}
												}
									}
								}
								
								inCheck = 1;
								// Check for pipes
								for (pipeCheck; pipeCheck < myargc; pipeCheck++)
								{
									if (my_argv[pipeCheck] != NULL)
									{
										if (strcmp(my_argv[pipeCheck], "|") == 0)
										{
											if (my_argv[pipeCheck + 1] != NULL)
											{
												pipeBool = 1;
												pipeIndex = pipeCheck;
											}
										}
									}
								}
								pipeCheck = 0;
								if (pipeBool == 1)
								{
									execute_pipe(pipeIndex - 1, pipeIndex + 1, pipeIndex);
								}
								else
								{
								if (strcmp(cmd, "superBash") == 0 || strcmp(cmd, "strToBinary") == 0 ||
										strcmp(cmd, "XOR") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd, "cpusage") == 0 ||
										strcmp(cmd, "man") == 0 || strcmp(cmd, "echo") == 0)
								{
									execute_function(0,0,0);
								}
								else if(strcmp(cmd, "clear") == 0)
								{
									printf("\033[H\033[J");
								}
								else if (strcmp(cmd, "quit") == 0)
								{
									// Free all memory
									// Empty all arrays
									// Return 0
									free(tmp);
									free(path_str);
									for(i=0;my_envp[i]!=NULL;i++)
										free(my_envp[i]);
									for(i=0;i<10;i++)
										free(search_path[i]);
									printf("\n");
									return 0;
								}
								else if (my_argv[1] != NULL && strcmp(my_argv[1], "=") == 0)
								{
									if (strcmp(my_argv[1], "=") == 0)
									{
										assign_variable(my_argv[0],my_argv[2]);
									}
								}
								else if(attach_path(cmd) == 0) 
								{
									saved_stdout = dup(STDOUT_FILENO);
									
									if (strcmp(my_argv[myargc - 1],"&") == 0)
									{
										
										// If last argv is &, run in background.
										call_execve_background(cmd);
									}
									else
									{
										if (my_argv[1] != NULL)
										{
											if (strcmp(my_argv[1], ">") == 0)
											{
												if (my_argv[2] != NULL)
												{
													//char *tempcmd = (char *)malloc(sizeof(char) * 100);
													//strncat(tempcmd, "/bin/" , 5);
													//strncat(tempcmd, my_argv[0], strlen(my_argv[0]));
													//strncat(tempcmd, "\0", 1);
													//strncpy(cmd, my_argv[0], strlen(my_argv[0]));
													//strncat(cmd, "\0", 1);
													//redir_out(my_argv[2]);
													call_execve(cmd);
													//fflush(stdout);
												}
											}
										}
										else
										{
											call_execve(cmd);
										}
										
									}
								}
								else 
								{
									printf("%s: command not found\n", cmd);
								}
								fflush(stdout);
								dup2(saved_stdout, 1);
								close(saved_stdout);
							} 
							}
							else 
							{
                           //if((fd[1] = open(cmd, O_RDONLY)) > 0) 
						   //{
                           //    close(fd);
                           //    call_execve(cmd);
                           //} 
						   //else 

									printf("%s: command not found\n", cmd);

							}
                       free_argv();
                       printf("[SSSH ] ");
                       bzero(cmd, 100);
                   }

                   bzero(tmp, 100);
                   break;

            default: strncat(tmp, &c, 1);
                 break;
        }
    }
    free(tmp);
    free(path_str);
    for(i=0;my_envp[i]!=NULL;i++)
        free(my_envp[i]);
    for(i=0;i<10;i++)
        free(search_path[i]);
    printf("\n");
    return 0;
}
