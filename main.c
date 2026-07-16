#include "guess.h"

int main(int argc, char *argv[]) {
    // Create variables for command line flags
    char sepchar = ',';
    unsigned int show_all = 0, use_case = 0, setline = 0, seed = time(NULL) ^ getpid();
    int max_attempts = -1;
    int opt;
    char *end = NULL;

    // Parse flags
    while ((opt = getopt(argc, argv, "acd:hl:n:s:")) != -1) {
        switch (opt) {
            case 'a': show_all = 1; break;
            case 'c': use_case = 1; break;
            case 'd': sepchar = optarg[0]; break;
            case 'h': usage(); return 0;
            case 'l': setline = strtol(optarg, &end, 10); break;
            case 'n': max_attempts = strtol(optarg, &end, 10); break;
            case 's': seed = strtol(optarg, &end, 10); break;
            case '?': usage(); return 1;
        }
    }

    if (end && *end != '\0') {
        printf("Expected integer but recieved string.\n");
        return 1;
    }
    if (max_attempts < -1) {
        printf("Max attempts must be a positive number.\n");
        return 1;
    }
    if (setline < 0) {
        printf("Line number must be positive.\n");
        return 1;
    }

    srand(seed);

    char *filename = argv[optind];
    // if there are no non-flag arguments, return 1
    if (optind >= argc) {
        printf("No file input. Please enter the file path of the dataset.\n");
        char input[MAXOPL];
        fgets(input, MAXOPL, stdin);
        input[strcspn(input, "\n")] = '\0';
        filename = input;
    }

    // open the first non-flag argument
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    // Parse first line for a linked list of header options
    struct Node *ops = node_alloc("");
    int nops = parse_line(ops, file, sepchar);

    // Choose line from file to be the secret
    int line = choose_line(file, setline);

    // parse the secret line
    struct Node *secret = node_alloc("");
    // if the length of the secret line is not the same as the header line,
    // throw error
    if (nops != parse_line(secret, file, sepchar)) {
        printf("Malformed line at line %d\n", line);
        cleanup(ops, secret);
        return 1;
    }

    // close file - no longer needed
    fclose(file);

    // let user try to guess secret item
    guess_loop(ops, secret, nops, show_all, use_case, max_attempts);
    // free both linked lists
    cleanup(ops, secret);
}

// Function to show help message
void usage() {
    printf(
        "Usage:\n"
        "  guess [-a] [-c] [-d CHAR] [-h] [-l LINE] [-n MAX] file\n"
        "\n"
        "  -a        show all fields at once\n"
        "  -c        case-sensitive answer matching (default: insensitive)\n"
        "  -d CHAR   field delimiter (default: ',')\n"
        "  -h        show this help message\n"
        "  -l LINE   use the nth data row after the header instead of a random one\n"
        "  -n MAX    limit number of guesses before revealing the answer\n"
        "  -s SEED   set a set seed for the random number generation\n"
    );
}

