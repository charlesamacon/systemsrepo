// Group 13

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

extern int errno;

typedef void (*sighandler_t)(int);
static char *my_argv[100], *my_envp[100];
static char *search_path[10];

void handle_signal(int signo)
{
    printf("\n[MY_SHELL ] ");
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
	//printf("cmd is %s\n", cmd);
	//if (fork() == 0) {
	//	i = execve(cmd, my_argv, my_envp);
	//	printf("errno is %d\n", errno);
	//	if (i < 0) {
	//		printf("%s: %s\n", cmd, "command not found");
	//		exit(1);
	//	}
	//}
	//else {
	//	wait(NULL);
	//}

	int i;
	pid_t pid;
	int status;

	switch (pid = fork())
	{	
	case -1:				// Error
		printf("errno is %d\n", errno);	
		break;
	case 0:					// Fork Succeeded
		i = execve(cmd, my_argv, my_envp);
		if (i < 0)
		{
			printf("%s: %s\n", cmd, "command not found");
			exit(1);	// Parent exits
		}
		else
		{
			wait(%status);	// Fork() returns new pid to parent
		}
	}

	// We are not checking waitpid here, so essentially every parent runs independently of its child. Essentially, it's in the background here.

	printf("Process ID %d\n", (int)pid);

	return;
}

// From gnu.org/software/libc/manual/html_node/Foreground-and-Background.html
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

void free_argv()
{
	int index;
	for (index = 0; my_argv[index] != NULL; index++) {
		bzero(my_argv[index], strlen(my_argv[index]) + 1);
		my_argv[index] = NULL;
		free(my_argv[index]);
	}
}

void execute_pipe(char *argv[], char *args[])
{
	// This is to be reworked by Jordan.
	// While this is the general idea of the thing, a much simpler method was figured out on 11/25/2014.
	
	// Not 100% sure on this, so its commented

	// int fds[2];
	// pipe(fds);

	// if (fork())
	// {
	//		dup2(fds[1], STDOUT_FILENO);
	//		call_execve(COMMAND FROM ARGV);
	// }
	// else
	// {
	//		dup2(fds[0], STDIN_FILENO);
	//		call_exceve(COMMAND FROM ARGV);
	// }

	// return;
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
        exit(1);
    } else {
        wait(NULL);
    }
    printf("[MY_SHELL ] ");
    fflush(stdout);

    while(c != EOF) {
        c = getchar();
        switch(c) {
            case '\n': if(tmp[0] == '\0') 
						{
							printf("[MY_SHELL ] ");
						} 
					   else 
					   {
							fill_argv(tmp);
							strncpy(cmd, my_argv[0], strlen(my_argv[0]));
							strncat(cmd, "\0", 1);
							
							if(index(cmd, '/') == NULL)		
							{
								if(attach_path(cmd) == 0) 
								{
									// if redirecting I/O

									// Example:
									// sort < input > output

									// int in;
									// int out;

									// Input
									// in = open("input", O_RDONLY);		// var "input" is what we're reading in from. O_RDONLY states that it should be read only
									// do things with in (pass to command, whatever).
									// dup(in, STDIN_FILENO);				// duplicate file descriptor, create new file descriptor that refers to the same file
									// close(in);							// close in

									

									// Output
									// out = open("output", O_WRONLY|O_CREAT,0666);		// var "output" is what we're writing to. O_WRONLY states we're only writing. O_CREAT means that if it doesn't exist, we create a file of specific permissions
									// write(out, $VAR, strlen($VAR));					// write to file descriptor, $VAR is what we want to write, and strlen($VAR) is how long it is.
									// dup2(out, STDOUT_FILENO);						// duplicate file descriptor
									// close(out);										// close out
									call_execve(cmd);
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
                       printf("[MY_SHELL ] ");
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
