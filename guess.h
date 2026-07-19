#ifndef GUESS_H
#define GUESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>

#define MAXOPL 1024

struct Node {
    char *data;
    struct Node *next;
};

void usage();
struct Node *node_alloc(char *data);
int parse_line(struct Node *head, FILE *file, char sepchar);
int choose_line(FILE *file, int setline);
void guess_loop(struct Node *ops, struct Node *secret, int nops, int show_all, int use_case, int max_attempts);
void cleanup(struct Node *ops, struct Node *secret);

#endif
