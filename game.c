#include "guess.h"

// Function to choose random or set line from file (not header)
// by positioning the cursor
int choose_line(FILE *file, int setline) {
    // Move cursor to start
    fseek(file, 0, SEEK_SET);

    // Count lines by counting number of newline chars
    int c;
    int lines = 0;
    while ((c = getc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }

    // Move cursor to start
    fseek(file, 0, SEEK_SET);

    // needs at least 2 lines - 1 header and 1 data row
    if (lines < 2) {
        fprintf(stderr, "Error: file has no data rows\n");
        exit(1);
    }

    // Choose random value between 0 and lines
    // choose random value, then clamp it between 0 and lines - 1
    // this means the min is 0 and max lines - 2,
    // making the final range is 0 to lines - 1
    // having lines as the max value is unwanted as then it moves the 
    // cursor to the final newline, with no values after
    int rng = (rand() % (lines - 1)) + 1;

    // If set line was chosen, use that
    if (setline) {
        if (setline > 0 && setline < lines) {
            rng = setline;
        }
        else {
            printf("Line %d is not in file. Using random line.\n", setline);
        }
    }

    // Move cursor forward by rng lines
    lines = 0;
    while ((c = getc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
        if (lines == rng) {
            break;
        }
    }

    // Return line chosen - cursor has been positioned
    return lines;
}

// Function to print the options
void display_options(struct Node *ops, struct Node *secret, int show_all) {
    int i = 0;
    // For all options and corresponding secrets
    for (struct Node *p = secret->next->next, *o = ops->next->next; p && o; p = p->next, o = o->next) {
        if (show_all) {
            // If show all mode is on, show option: value
            printf("\e[0;93m%s: %s\e[0m\n", o->data, p->data);
        }
        else {
            // else increment i by 1 and print i. option
            printf("%d. %s\n", ++i, o->data);
        }
    }
    // If show_all then add special option to quit else just type 0 to quit
    if (show_all) {
        printf("\e[1;91mType 'quit' to quit.\e[0m\n");
    }
    else {
        printf("0. quit\n");
    }
    printf("\e[1;94m=> ");
}

// Function to repeatedly take guesses from the user
void guess_loop(struct Node *ops, struct Node *secret, int nops, int show_all, int use_case, int max_attempts) {
    // Set no of attempts and boolean value guessed to 0
    int guessed = 0;
    int attempts = 0;
    // While not guessed and no. of attempts does not EQUAL max attempts
    // This is because by default, max_attempts is -1 so if no max attempts is set, it will go forever
    while (!guessed && attempts++ != max_attempts) {
        // Display options
        display_options(ops, secret, show_all);

        // Create input buffer for user
        char input[MAXOPL];
        // Read input from stdin
        fgets(input, MAXOPL, stdin);
        input[strcspn(input, "\n")] = '\0';
        // Color is blue because ansi escape code from display_options still persists so reset it
        printf("\e[0m");
        // Try to convert input to int
        char *end;
        long ask = strtol(input, &end, 10);
        // If succes, parse as selected option
        if (*end == '\0' && !show_all) {
            // if valid option
            if (ask < nops && ask >= 0) {
                // set node pointers p and o to the 1st value, because secret and ops are dummy nodes
                struct Node *p = secret->next;
                struct Node *o = ops->next;
                // find the next node ask times (find the (ask)th node)
                for (int k = 0; k < ask; k++) {
                    p = p->next;
                    o = o->next;
                }
                // If the user quit, print the answer and exit
                if (ask == 0) {
                    printf("\e[0;91m");
                    printf("answer: %s\n\n", p->data);
                    printf("\e[0m");
                    guessed = 1;
                }
                // Else, print the corresponding value
                else {
                    printf("\e[0;93m%s: %s\e[0m\n\n", o->data, p->data);
                }
            }
            else {
                // if the user entered an invalid option, 
                // do not count the attempt as a valid attempt
                printf("\e[1;91mInvalid Option\e[0m\n\n");
                attempts--;
            }
        }
        // Else, user must have taken a guess
        else {
            // If they chose to quit
            if (strcmp(input, "quit") == 0) {
                printf("\e[0;91m");
                printf("answer: %s\n\n", secret->next->data);
                printf("\e[0m");
                guessed = 1;
            }
            // else compare to answer
            else if ((use_case ? strcmp(input, secret->next->data) : strcasecmp(input, secret->next->data)) == 0) {
                printf("\e[1;92mCorrect!\nYou guessed it in %d attempts!\e[0m\n", attempts);
                guessed = 1;
            }
            else {
                printf("\e[1;91mIncorrect\e[0m\n\n");
            }
        }
    }
    // if they ran out of attempts
    if (attempts == max_attempts) {
        printf("\e[1;91mYou ran out of attempts\e[0m\n");
    }
}
