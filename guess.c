#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

#define MAXOPL 1024

struct Node {
    char *data;
    struct Node *next;
};

typedef struct Node Header;
typedef struct Node node;

void usage();
struct Node *node_alloc(char *data);
int parse_line(struct Node *head, FILE *file, char sepchar);
int choose_line(FILE *file, int setline);
void guess_loop(struct Node *ops, struct Node *secret, int nops, int show_all, int use_case, int max_attempts);
void cleanup(struct Node *ops, struct Node *secret, FILE *file);

int main(int argc, char *argv[]) {
    char sepchar = ',';
    int show_all = 0, use_case = 0, setline = 0, max_attempts = -1;

    int opt;

    while ((opt = getopt(argc, argv, "acd:hl:n:")) != -1) {
        switch (opt) {
            case 'a': show_all = 1; break;
            case 'c': use_case = 1; break;
            case 'd': sepchar = optarg[0]; break;
            case 'h': usage(); return 0;
            case 'l': setline = strtol(optarg, NULL, 10); break;
            case 'n': max_attempts = strtol(optarg, NULL, 10); break;
            case '?': usage(); return 1;
        }
    }

    if (optind >= argc) {
        usage();
        return 1;
    }

    FILE *file = fopen(argv[optind], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    Header *ops = node_alloc("");
    int nops = parse_line(ops, file, sepchar);

    srand(time(NULL) ^ getpid());
    int line = choose_line(file, setline);

    node *secret = node_alloc("");
    if (nops != parse_line(secret, file, sepchar)) {
        printf("Malformed line at line %d\n", line + 1);
        cleanup(ops, secret, file);
        return 1;
    }

    guess_loop(ops, secret, nops, show_all, use_case, max_attempts);
    cleanup(ops, secret, file);
}

void usage() {
    printf(
        "Usage:\n"
        "  guess [-a] [-c] [-d CHAR] [-h] [-l LINE] [-n MAX] file\n"
        "\n"
        "  -a        show all fields at once\n"
        "  -c        case-sensitive answer matching (default: insensitive)\n"
        "  -d CHAR   field delimiter (default: ',')\n"
        "  -h        show this help message\n"
        "  -l LINE   use a specific data row instead of a random one\n"
        "  -n MAX    limit number of guesses before revealing the answer\n"
    );
}

struct Node *node_alloc(char *data) {
    struct Node *temp = (struct Node *) malloc(sizeof(struct Node));
    temp->next = NULL;
    temp->data = malloc(sizeof(char) * (strlen(data) + 1));
    strcpy(temp->data, data);
    return temp;
}

int parse_line(struct Node *head, FILE *file, char sepchar) {
    int c;
    int i = 0;
    int nops = 1;
    char buf[MAXOPL];
    struct Node *current = head;
    while ((c = getc(file)) != '\n' && c != EOF) {
        if (c == sepchar) {
            buf[i] = '\0';
            current->next = node_alloc(buf);
            current = current->next;
            i = 0;
            nops++;
            while((c = getc(file)) == ' ');
            ungetc(c, file);
        }
        else {
            if (i < MAXOPL - 1) {
                buf[i++] = c;
            }
        }
    }
    buf[i] = '\0';
    current->next = node_alloc(buf);
    current = current->next;
    return nops;
}

int choose_line(FILE *file, int setline) {
    fseek(file, 0, SEEK_SET);

    int c;
    int lines = -1; // Exclude header
    while ((c = getc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }

    fseek(file, 0, SEEK_SET);

    if (lines <= 0) {
        fprintf(stderr, "Error: file has no data rows\n");
        exit(1);
    }
    int rng;
    if (setline > 0) {
        if (setline <= lines) {
            rng = setline;
        }
        else {
            printf("Line %d is not in file. Using random line.\n", setline);
            rng = (rand() % lines) + 1;
        }
    }
    else {
        rng = (rand() % lines) + 1;
    }

    lines = 0;
    while ((c = getc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
        if (lines == rng) {
            break;
        }
    }

    return lines;
}

void guess_loop(struct Node *ops, struct Node *secret, int nops, int show_all, int use_case, int max_attempts) {
    int guessed = 0;
    int attempts = 0;
    while (!guessed && attempts != max_attempts) {
        attempts++;
        int i = 0;
        for (node *p = secret->next->next, *o = ops->next->next; p && o; p = p->next, o = o->next) {
            if (show_all) {
                printf("\e[0;93m%s: %s\e[0m\n", o->data, p->data);
            }
            else {
                printf("%d. %s\n", ++i, o->data);
            }
        }
        if (show_all) {
            printf("\e[1;91mType 'quit' to quit.\e[0m\n");
        }
        else {
            printf("0. quit\n");
        }
        printf("\e[1;94m=> ");

        char input[MAXOPL];
        scanf("%1023s", input);
        printf("\e[0m");
        char *end;
        long ask = strtol(input, &end, 10);

        if (*end == '\0' && !show_all) {
            if (ask < nops) {
                if (ask < 0) {
                    guessed = 1;
                    break;
                }
                node *p = secret->next;
                node *o = ops->next;
                for (int k = 0; k < ask; k++) {
                    p = p->next;
                    o = o->next;
                }
                if (ask == 0) {
                    printf("\e[0;91m");
                    printf("answer: %s\n\n", p->data);
                    printf("\e[0m");
                    guessed = 1;
                }
                else {
                    printf("\e[0;93m%s: %s\e[0m\n\n", o->data, p->data);
                }
            }
            else {
                printf("\e[1;91mInvalid Option\e[0m\n\n");
            }
        }
        else {
            if (strcmp(input, "quit") == 0) {
                printf("\e[0;91m");
                printf("answer: %s\n\n", secret->next->data);
                printf("\e[0m");
                guessed = 1;
            }
            else if ((use_case ? strcmp(input, secret->next->data) : strcasecmp(input, secret->next->data)) == 0) {
                printf("\e[1;92mCorrect!\nYou guessed it in %d attempts!\e[0m\n", attempts);
                guessed = 1;
            }
            else {
                printf("\e[1;91mIncorrect\e[0m\n\n");
            }
        }
    }
    if (attempts == max_attempts) {
        printf("\e[1;91mYou ran out of attempts\e[0m\n");
    }
}

void cleanup(struct Node *ops, struct Node *secret, FILE *file) {

    for (node *p = ops, *q = ops->next; p; p = q) {
        free(p->data);
        q = p->next;
        free(p);
    }
    for (node *p = secret, *q = secret->next; p; p = q) {
        free(p->data);
        q = p->next;
        free(p);
    }
    fclose(file);

}
