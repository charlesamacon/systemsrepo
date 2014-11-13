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

// Tokenizer 2000
int tokenizer(char *c, char *argv[])
{
	// This is so we can keep track of how many elements are actually in argv.
	// I know we could get argc, but this keeps track of how many are actually USED
	int i = 0;

	while (*c && isspace(*c))
	{
		c++;
	}
	
	while (*c && i < 255)	// Set the max number of arguments to a static number of 255 (I doubt we'll need more than that right now)
	{
		argv[i + 1] = buffer;

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
void echo()
{
	// Use printf to print arguments. If a $ is encountered, what follows should be made a variable.
	return;
}

// Man
void man()
{
	// Output text files based on the argument name
	return;
}

// cpuUsage
void cpuUsage()
{
	// Figure out a way to call Top.
	return;
}

// SuperBash
void superBash()
{
	// This is a line for line copy of the superbash program. Except we need to fix the segmentation faults.
	return;
}

// Extra Linux Util
void util()
{
	// We should probably decide this soon.
	return;
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