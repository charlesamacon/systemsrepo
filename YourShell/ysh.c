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
static char *search_path[10];
static struct variable myvar[100];

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
				int fd = open([FILENAME], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

				dup2(fd, 1);	// make stdout go to file
				
				close (fd);

				outRedir = 0;	// Reset this
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
				waitpid(%status);	// Fork() returns new pid to parent
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

void assign_variable(char *var, char value)
{
	// This should only be called when there is an = in the command line
	// Example:
	// MY_SHELL> var = value
	// This should store var in myvar[i].varName and value in myvar[i].varData.
	// We are then able to manipulate said data in echo (which is being moved over to this file to keep from messing with externs).
	
	myvar[0].varName = "PATH";
	myvar[0].varData = "/usr/bin/clear";

	//We have to declare this outside of a for loop in C
	//C++ lets you declare it as part of the for loop declaration, C does not, however.
	//-CAM
	int i;

	for (i= 1; i < 100; i++)
	{
		if (myvar[i] == NULL)
		{
			// This doesn't make sense, and I'm exhausted from work. Can anyone pick this up? Basically, we need to store var and its value, but I can't put 2 and 2 together right now.
			strncpy(myvar[i].varName, var);
			strncpy(myvar[i].varData, value);
			//my_var[i].var = var;
			//my_var[i][0] = value;
			break;
		}

	}
}

// Echo
void echo(char *argv[], int argc)
{
	// Basically, we need the ability to assign variables values. We need to create a 2-Dimensional array for variables
	// The user should be able to type "variable=2" and then "echo $variable" and ysh should output "2".
	// So we just need echo to check that variable list, and if the variable exists, it outputs whatever.

	// Seems simple enough:
	// In YSH.c, create 2D array for variables and their data.
	// Pass this array (or a pointer to it, at least) to echo.
	// If there are any strings in *argv[] that start with $, check the rest of that string to all variables stored in the array.
	// If it exists, output its value.
	// Otherwise, output an error message.
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
				if (argv[i][k] != EOF)
				{
					buffer[k] = argv[i][k];
				}
			}

			for (j = 0; j < 100; j++)
			{
				if (strcmp(buffer, myvar[i].varName))
				{
					printf("%s%s", my_var[i].varData, " ");
					err = 0;
				}
			}

			if (err == 1)
			{
				printf("\nError: Unrecognized Variable\n");
			}
		}
		else if (err == 0)
		{
			printf("%s%s", argv[i], " ");
		}
	}

	printf("%s%s", argv[argc - 1], " ");
	return;
}

void execute_pipe(char *argv[], char *args[])
{
	// This is to be reworked by Jordan.
	// While this is the general idea of the thing, a much simpler method was figured out on 11/25/2014.


	int fds[2];
	pipe(fds);

	if (fork())
	{
		dup2(fds[1], STDOUT_FILENO);
		call_execve(COMMAND FROM ARGV);
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		call_exceve(COMMAND FROM ARGV);
	}
	return;
}

void redir_in(char cmd[])
{
	// if redirecting I/O

	// Example:
	// sort < input > output

	// Input
	int in;

	in = open(cmd, O_RDONLY);
	call_execve(cmd);
	dup(in, STDIN_FILENO);
	close(in);

	return;
}

void redir_out(char output[], char file[])
{
	// Output (this needs to be read from STD OUT)
	int out;
	out = open(file, O_WRONLY | O_CREAT, 0666);
	write(out, output, strlen(output));
	dup2(out, STDOUT_FILENO);
	close(out);
	return;
}

int main(int argc, char *argv[], char *envp[])
{
    char c;
	int i;

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
							
							if(index(cmd, '/') == NULL)		
							{
								if (strcmp(cmd, "echo"))
								{
									//echo();
								}
								else if (strcmp(cmd, "cpusage"))
								{
									cpusage();
								}
								else if (strcmp(cmd, "superBash"))
								{
									//superBash();
								}
								else if (strcmp(cmd, "strToBinary"))
								{
									//strToBinary();
								}
								else if (strcmp(cmd, "XOR"))
								{
									//xorBinary();
								}
								else if (strcmp(cmd, "cd"))
								{
									//change directory
								}
								else if(attach_path(cmd) == 0) 
								{
									if (strcmp(argv[argc - 1],"&"))
									{
										// If last argv is &, run in background.
										call_execve_background(cmd);
									}
									else
									{
										call_execve(cmd);
									}
								} 
								else 
								{
									printf("%s: command not found\n", cmd);
								}
							} 
						else 
						{
                           if((fd[1] = open(cmd, O_RDONLY)) > 0) 
						   {
                               close(fd);
                               call_execve(cmd);
                           } 
						   else 
						   {
                               printf("%s: command not found\n", cmd);
                           }
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
