#include <stdio.h>
#include <stdlib.h>
#include "fileread.h"
#include "wreplace.h"
#include "kode.h"
#include <string.h>
#include <ctype.h>


FILE* check_file(char* file_name) {
    FILE* file = fopen(file_name, "r+");
    if (file == NULL) {
        perror("fopen");
        return NULL;
    }
    return file;
}

struct file_changes* process_command(int command, char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes) {
    switch (command) {
        case CMD_RC:
            printf("Processing command RC.\n");
            changes = change_file_RC(buffer, read_size, word_len, word, changes);
            break;
        case CMD_RI:
            printf("Processing command RI.\n");
            changes = change_file_RI(buffer, read_size, word_len, word, changes);
            break;
        case CMD_UK:
            printf("Processing command UK.\n");
            changes = change_file_UK(buffer, read_size, word_len, word, changes);
            break;
        case CMD_UM:
            printf("Processing command UM.\n");
            changes = change_file_UM(buffer, read_size, word_len, word, changes);
            break;
        default:
            fprintf(stderr, "Invalid command code: %d\n", command);
            return NULL;
    }
    return changes;
}


struct file_changes* search_and_change_file(FILE* file, char* word, int command,struct file_changes* changes) {
    size_t word_len = strlen(word);

    // Determine file size.
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek");
        exit(EXIT_FAILURE);
    }
    long file_size = ftell(file);
    if (file_size < 0) {
        perror("ftell");
        exit(EXIT_FAILURE);
    }
    rewind(file);

    // Allocate a buffer for the entire file (plus a null terminator).
    char *buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Read the entire file into memory.
    size_t read_size = fread(buffer, 1, file_size, file);
    buffer[read_size] = '\0';

    // Count total words in the file.
    int total_words = 0;
    int in_word = 0;
    for (size_t i = 0; i < read_size; i++) {
        if (isspace((unsigned char)buffer[i])) {
            in_word = 0;
        } else if (!in_word) {
            total_words++;
            in_word = 1;
        }
    }
    changes->nbwords = total_words;

    // Process the buffer: perform replacements and count changes.
    changes=process_command(command,buffer,read_size,word_len,word,changes);

    // Write the modified buffer back to the file.
    rewind(file);
    if (fwrite(buffer, 1, read_size, file) != read_size) {
        perror("fwrite");
        free(buffer);
        exit(EXIT_FAILURE);
    }
    fflush(file);

    free(buffer);
    return changes;
}

struct file_changes* change_file_RC(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes) {
    int line_changed = 0;

    char asterisk_word[word_len + 1];

    for(int i=0;i<word_len;i++){
        asterisk_word[i]='*';
    }

    asterisk_word[word_len]='\0';

    for (size_t i = 0; i < read_size; i++) {
        // Count lines: every newline marks the end of a line.
        if (buffer[i] == '\n') {
            changes->nb_lines++;
            line_changed = 0; // Reset for the new line.
        }
        
        // Check if the target word occurs at this position.
        if (i <= read_size - word_len && strncmp(&buffer[i], word, word_len) == 0) {
            // Determine the start of the current line.
            size_t line_start = i;
            while (line_start > 0 && buffer[line_start - 1] != '\n') {
                line_start--;
            }
            // Determine the end of the current line.
            size_t line_end = i;
            while (line_end < read_size && buffer[line_end] != '\n') {
                line_end++;
            }
            size_t line_length = line_end - line_start;
            
            // Create temporary buffers for the full original line and the modified line.
            char old_line[line_length + 1];
            char new_line[line_length + 1];
            memcpy(old_line, &buffer[line_start], line_length);
            old_line[line_length] = '\0';
            memcpy(new_line, old_line, line_length + 1);  // Copy the entire line.
            
            // Replace the occurrence in new_line with asterisks.
            size_t pos_in_line = i - line_start;  // position relative to the beginning of the line.
            for (size_t j = 0; j < word_len; j++) {
                new_line[pos_in_line + j] = '*';
            }
            
            // Ask the user for confirmation using the full line.
            // (Here we use changes->nb_lines + 1 to represent the current line number.)
            if(global_apply_all==0){
                int decision = confirm_User_Changes(old_line, changes->nb_lines + 1, new_line, changes->nb_lines + 1);
                if (decision == 3) { // Quit.
                    return changes;
                } else if (decision == 1) { // Reject this occurrence.
                    i += word_len - 1;
                    continue;
                } else if (decision == 2) { // Apply all changes without further prompting.
                    global_apply_all = 1;
                }
            }
            // If decision is 0 (Yes) or 2 (All), proceed with replacement.
            

            replace_Word(buffer, word_len, i, asterisk_word);
            changes->nb_words_changed++;
            if (!line_changed) {
                changes->nb_lines_changed++;
                line_changed = 1;
            }
            // Skip past the replaced word.
            i += word_len - 1;
        }
    }
    
    // If the file does not end with a newline, count the last line.
    if (read_size > 0 && buffer[read_size - 1] != '\n') {
        changes->nb_lines++;
    }
    return changes;
}

struct file_changes* change_file_RI(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes){
    int line_changed = 0;

    char asterisk_word[word_len + 1];

    for(int i=0;i<word_len;i++){
        asterisk_word[i]='*';
    }

    asterisk_word[word_len]='\0';

    for (size_t i = 0; i < read_size; i++) {
        // Count lines: every newline marks the end of a line.
        if (buffer[i] == '\n') {
            changes->nb_lines++;
            line_changed = 0; // Reset for the new line.
        }

        char lower_case_word[word_len + 1];
        strncpy(lower_case_word, &buffer[i], word_len);
        lower_case_word[word_len] = '\0'; // Ensure null termination.
        strlwr(lower_case_word);

        // Check if the target word occurs at this position.
        if (i <= read_size - word_len && strncmp(lower_case_word, word, word_len) == 0) {
            // Determine the start of the current line.
            size_t line_start = i;
            while (line_start > 0 && buffer[line_start - 1] != '\n') {
                line_start--;
            }
            // Determine the end of the current line.
            size_t line_end = i;
            while (line_end < read_size && buffer[line_end] != '\n') {
                line_end++;
            }
            size_t line_length = line_end - line_start;
            
            // Create temporary buffers for the full original line and the modified line.
            char old_line[line_length + 1];
            char new_line[line_length + 1];
            memcpy(old_line, &buffer[line_start], line_length);
            old_line[line_length] = '\0';
            memcpy(new_line, old_line, line_length + 1);  // Copy the entire line.
            
            // Replace the occurrence in new_line with asterisks.
            size_t pos_in_line = i - line_start;  // position relative to the beginning of the line.
            for (size_t j = 0; j < word_len; j++) {
                new_line[pos_in_line + j] = '*';
            }
            
            // Ask the user for confirmation using the full line.
            // (Here we use changes->nb_lines + 1 to represent the current line number.)
            if(global_apply_all==0){
                int decision = confirm_User_Changes(old_line, changes->nb_lines + 1, new_line, changes->nb_lines + 1);
                if (decision == 3) { // Quit.
                    return changes;
                } else if (decision == 1) { // Reject this occurrence.
                    i += word_len - 1;
                    continue;
                } else if (decision == 2) { // Apply all changes without further prompting.
                    global_apply_all = 1;
                }
            }
            // If decision is 0 (Yes) or 2 (All), proceed with replacement.
            replace_Word(buffer, word_len, i, asterisk_word);
            changes->nb_words_changed++;
            if (!line_changed) {
                changes->nb_lines_changed++;
                line_changed = 1;
            }
            // Skip past the replaced word.
            i += word_len - 1;
        }
    }

     // If the file does not end with a newline, count the last line.
     if (read_size > 0 && buffer[read_size - 1] != '\n') {
        changes->nb_lines++;
     }
    
    return changes;
}

struct file_changes* change_file_UK(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes) {
    int line_changed = 0;

    // Build the asterisk sequence of length word_len.
    char asterisk_seq[word_len + 1];
    for (size_t k = 0; k < word_len; k++) {
        asterisk_seq[k] = '*';
    }
    asterisk_seq[word_len] = '\0';

    for (size_t i = 0; i < read_size; i++) {
        // Count newlines.
        if (buffer[i] == '\n') {
            changes->nb_lines++;
            line_changed = 0; // Reset for the new line.
        }

        // Ensure there are enough characters left for a full match.
        if (i > read_size - word_len) {
            break;
        }

        int is_exact_asterisks = check_exact_asterisk(word_len,read_size,buffer,i);
        for (size_t j = 0; j < word_len; j++) {
            if (buffer[i + j] != '*') {
                is_exact_asterisks = 0;
                break;
            }
        }

        // Verify that the sequence is not part of a longer asterisk run.
        if (is_exact_asterisks) {
            // Check the character before the sequence (if any)
            if (i > 0 && buffer[i - 1] == '*') {
                is_exact_asterisks = 0;
            }
            // Check the character after the sequence (if any)
            if (i + word_len < read_size && buffer[i + word_len] == '*') {
                is_exact_asterisks = 0;
            }
        }

        if (is_exact_asterisks) {
            // Determine the start of the current line.
            size_t line_start = i;
            while (line_start > 0 && buffer[line_start - 1] != '\n') {
                line_start--;
            }
            // Determine the end of the current line.
            size_t line_end = i;
            while (line_end < read_size && buffer[line_end] != '\n') {
                line_end++;
            }
            size_t line_length = line_end - line_start;
            
            // Copy the current line for display.
            char old_line[line_length + 1];
            char new_line[line_length + 1];
            memcpy(old_line, &buffer[line_start], line_length);
            old_line[line_length] = '\0';
            memcpy(new_line, old_line, line_length + 1);

            // Replace the occurrence in new_line with asterisks.
            size_t pos_in_line = i - line_start;  
            for (size_t j = 0; j < word_len; j++) {
                new_line[pos_in_line + j] = word[j];
            }

            // Confirm the change with the user.
            if (global_apply_all == 0) {
                int decision = confirm_User_Changes(old_line, changes->nb_lines + 1, new_line, changes->nb_lines + 1);
                if (decision == 3) { // Quit.
                    return changes;
                } else if (decision == 1) { // Reject this occurrence.
                    i += word_len - 1;
                    continue;
                } else if (decision == 2) { // Apply all without further prompting.
                    global_apply_all = 1;
                }
            }
            
            // Replace the asterisks with the exact word.
            replace_Word(buffer, word_len, i, word);
            changes->nb_words_changed++;
            if (!line_changed) {
                changes->nb_lines_changed++;
                line_changed = 1;
            }
            // Skip past the replaced word.
            i += word_len - 1;
        }
    }

    // If the file does not end with a newline, count the last line.
    if (read_size > 0 && buffer[read_size - 1] != '\n') {
        changes->nb_lines++;
    }

    return changes;
}

struct file_changes* change_file_UM(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes){
    int line_changed = 0;

    for (size_t i = 0; i < read_size; i++) {
        // Count newlines.
        if (buffer[i] == '\n') {
            changes->nb_lines++;
            line_changed = 0; // Reset for the new line
        }

        // Ensure there are enough characters left for a full match
        if (i > read_size - word_len) {
            break;
        }

        int is_exact_asterisks = check_exact_asterisk(word_len,read_size,buffer,i);

        if (is_exact_asterisks) {
            // Determine the start and end of the current line
            size_t line_start = i;
            while (line_start > 0 && buffer[line_start - 1] != '\n') {
                line_start--;
            }
            size_t line_end = i;
            while (line_end < read_size && buffer[line_end] != '\n') {
                line_end++;
            }
            size_t line_length = line_end - line_start;
            
            // Create temporary buffers for the current line
            char old_line[line_length + 1];
            char new_line[line_length + 1];
            memcpy(old_line, &buffer[line_start], line_length);
            old_line[line_length] = '\0';
            memcpy(new_line, old_line, line_length + 1);

            // Determine a reference letter for case conversion
            char ref_letter = '\0';
            // Prefer the previous letter if it exists and is alphabetic
            if (i > 0 && isalpha(buffer[i - 1])) {
                ref_letter = buffer[i - 1];
            }
            // Otherwise, check the letter immediately following the asterisk sequence
            else if (i + word_len < read_size && isalpha(buffer[i + word_len])) {
                ref_letter = buffer[i + word_len];
            }
            
            // Create an adjusted copy of the word.
            char adjusted_word[word_len + 1];
            strncpy(adjusted_word, word, word_len);
            adjusted_word[word_len] = '\0';

            // Adjust the case of adjusted_word based on the reference letter
            if (ref_letter != '\0') {
                if (isupper(ref_letter)) {
                    for (size_t k = 0; k < word_len; k++) {
                        adjusted_word[k] = toupper(adjusted_word[k]);
                    }
                } else if (islower(ref_letter)) {
                    for (size_t k = 0; k < word_len; k++) {
                        adjusted_word[k] = tolower(adjusted_word[k]);
                    }
                }
            }
            
            // Replace the occurrence in new_line for confirmation display
            size_t pos_in_line = i - line_start;  
            for (size_t j = 0; j < word_len; j++) {
                new_line[pos_in_line + j] = adjusted_word[j];
            }

            // Ask the user for confirmation.
            if (global_apply_all == 0) {
                int decision = confirm_User_Changes(old_line, changes->nb_lines + 1, new_line, changes->nb_lines + 1);
                if (decision == 3) { // Quit.
                    return changes;
                } else if (decision == 1) { // Reject this occurrence
                    i += word_len - 1;
                    continue;
                } else if (decision == 2) { // Apply all changes without further prompting
                    global_apply_all = 1;
                }
            }
            
            // Replace the asterisks in the main buffer with the adjusted word
            replace_Word(buffer, word_len, i, adjusted_word);
            changes->nb_words_changed++;
            if (!line_changed) {
                changes->nb_lines_changed++;
                line_changed = 1;
            }
            // Skip past the replaced word.
            i += word_len - 1;
        }
    }

    // If the file does not end with a newline, count the last line
    if (read_size > 0 && buffer[read_size - 1] != '\n') {
        changes->nb_lines++;
    }

    return changes;
}

int check_exact_asterisk(size_t word_len, size_t read_size, char* buffer, int index){
    int is_exact_asterisks=1;
    for (size_t j = 0; j < word_len; j++) {
        if (buffer[index + j] != '*') {
            is_exact_asterisks = 0;
            break;
        }
    }

    // Verify that the sequence is not part of a longer asterisk run.
    if (is_exact_asterisks) {
        // Check the character before the sequence (if any)
        if (index > 0 && buffer[index - 1] == '*') {
            is_exact_asterisks = 0;
        }
        // Check the character after the sequence (if any)
        if (index + word_len < read_size && buffer[index + word_len] == '*') {
            is_exact_asterisks = 0;
        }
    }
    return is_exact_asterisks;
}