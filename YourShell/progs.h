#ifndef PROGS_H
#define PROGS_H

#include <sys/wait.h>
#include <sys/types.h>

// Each of the functions from progs.c need to be declared here so they can be properly used later
 
// Tokenizer 2000 (tm)
// - Create tokens and ignore whitespace. Profit.
int tokenizer(char *c, char *argv[]);

#endif