#include "guess.h"

// Function to create a new node
struct Node *node_alloc(char *data) {
    // Alocate space for node (16 bytes - 8 for char * and 8 for struct Node *)
    struct Node *temp = malloc(sizeof(struct Node));
    // Set next node to null
    temp->next = NULL;
    // Allocate space for text
    temp->data = malloc(sizeof(char) * (strlen(data) + 1)); // + 1 for null byte at end of string
    // Copy text and return pointer to node
    strcpy(temp->data, data);
    return temp;
}

// Function to parse line of csv-style file
int parse_line(struct Node *head, FILE *file, char sepchar) {
    // Init variables
    int c; // Temporary char storage
    int nops = 1; // Number of options - 1 because last option does not end with trailing comma
    int i = 0; // Records place in buffer
    char buf[MAXOPL]; // Buffer to store text before new node is created
    // Set current node to head node for node creation
    struct Node *current = head;
    // Loop over line
    while ((c = getc(file)) != '\n' && c != EOF) {
        // If char is delimiter (new option)
        if (c == sepchar) {
            // End buffer string with null byte
            buf[i] = '\0';
            // Allocate new node for the option
            current->next = node_alloc(buf);
            // Set current optin to the new node
            current = current->next;
            // Go back to the start of the buffer
            i = 0;
            // Increase no. of ops
            nops++;
            // trim spaces before new option
            while((c = getc(file)) == ' ');
            ungetc(c, file);
        }
        else {
            // Push current char to buffer
            if (i < MAXOPL - 1) { // MAXOPL - 1 because null byte is pushed
                buf[i++] = c;
            }
            else {
                buf[MAXOPL - 1] = '\0';
                printf("Length of value exceeds length of buffer. Value truncated at '%s'", buf);
            }
        }
    }
    // Push last option with no trailing comma to new node
    buf[i] = '\0';
    current->next = node_alloc(buf);
    current = current->next;
    // Return no. of ops
    return nops;
}

// Function to free all nodes
void cleanup(struct Node *ops, struct Node *secret) {
    for (struct Node *p = ops, *q; p; p = q) {
        free(p->data);
        q = p->next;
        free(p);
    }
    for (struct Node *p = secret, *q; p; p = q) {
        free(p->data);
        q = p->next;
        free(p);
    }
}
