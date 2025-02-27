#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

const char* user_answers[] = {"Yes", "No", "All", "Quit"};

int validate_Argv(int argc, char **argv) {
    fflush(stderr);
    // Check for sufficient arguments.
    if (argc < 4) {
        fprintf(stderr, "========================================\n");
        fprintf(stderr, "ERROR: Insufficient arguments provided.\n");
        fprintf(stderr, "----------------------------------------\n");
        if (argc == 0) {
            fprintf(stderr, "No arguments provided. Please run the program correctly.\n");
        } else if (argc == 1) {
            fprintf(stderr, "Missing command, word, and filename.\n\n");
        } else if (argc == 2) {
            fprintf(stderr, "Missing word and filename.\n\n");
        } else if (argc == 3) {
            fprintf(stderr, "Missing filename.\n");
        }
        fprintf(stderr, "Usage: %s <command> <word> <filename>\n\n", argv[0]);
        fprintf(stderr, "========================================\n");
        fflush(stderr); 
        return 3;
    }
    
    // Validate command (argv[1]).
    if (strcmp(argv[1], "RC") != 0 &&
        strcmp(argv[1], "RI") != 0 &&
        strcmp(argv[1], "UK") != 0 &&
        strcmp(argv[1], "UM") != 0) {
        fprintf(stderr, "========================================\n");
        fprintf(stderr, "ERROR: Invalid command '%s'.\n", argv[1]);
        fprintf(stderr, "Valid commands are: RK, RI, UK, UM.\n");
        fprintf(stderr, "Usage: %s <command> <word> <filename>\n", argv[0]);
        fprintf(stderr, "========================================\n");
        fflush(stderr);
        return 3;
    }
    
    // Validate word (argv[2]).
    if (argv[2] == NULL || argv[2][0] == '\0') {
        fprintf(stderr, "========================================\n");
        fprintf(stderr, "ERROR: Invalid word. Please provide a non-empty word.\n");
        fprintf(stderr, "Usage: %s <command> <word> <filename>\n", argv[0]);
        fprintf(stderr, "========================================\n");
        fflush(stderr);
        return 3;
    }
    
    // Validate file (argv[3]) by attempting to open it.
    FILE *file = fopen(argv[3], "r+");
    if (file == NULL) {
        fprintf(stderr, "========================================\n");
        fprintf(stderr, "ERROR: Failed to open file '%s'.\n", argv[3]);
        fprintf(stderr, "Ensure the file exists and you have proper permissions.\n");
        fprintf(stderr, "Usage: %s <command> <word> <filename>\n", argv[0]);
        fprintf(stderr, "========================================\n");
        fflush(stderr);
        return 3;
    }
    fclose(file);
    
    return 1; // Validation successful.
}


int confirm_User_Changes(char* old, int oldNb, char* new, int newNb){
    char answer[10];

    while (1) {
        
        printf("========================================\n");
        printf("Line %d Change:\n", oldNb);
        printf("----------------------------------------\n");
        printf("Original: %s\n", old);
        printf("Modified: %s\n", new);
        printf("========================================\n");
        printf("Confirm (Yes, No, All, Quit): ");
        
        
        if (fgets(answer, sizeof(answer), stdin) == NULL) {
            fprintf(stderr, "Error reading input. Please try again.\n");
            continue;  // Try again on error.
        }
        
        // Remove trailing newline, if any.
        answer[strcspn(answer, "\n")] = '\0';
        
        // Convert the answer to lowercase.
        for (char *p = answer; *p; ++p) {
            *p = tolower((unsigned char)*p);
        }
        
        
        if (strcmp(answer, "yes") == 0) {
            return 0;  // Confirm change.
        } else if (strcmp(answer, "no") == 0) {
            return 1;  // Reject this change, continue asking.
        } else if (strcmp(answer, "all") == 0) {
            return 2;  // Apply all changes without asking again.
        } else if (strcmp(answer, "quit") == 0) {
            return 3;  // Exit the process.
        } else {
            printf("Invalid input. Please enter Yes, No, All, or Quit.\n");
        }
    }
}

void display_changes(struct file_changes* changes){
    printf("Changed %d word(s) in %d line(s).\n", 
        changes->nb_words_changed, 
        changes->nb_lines_changed);
}