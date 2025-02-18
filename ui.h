#ifndef UI_H
#define UI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fileread.h"

extern const char* user_answers[];

/**
 * @brief Validates initial user input.
 *
 * This function checks that the required command-line arguments are provided and valid.
 * It ensures that a command, a word, and a filename have been specified, and prints detailed
 * error messages if any argument is missing or invalid.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return An integer status code (1 for success, 3 for error).
 */
int validate_Argv(int argc, char** argv);

/**
 * @brief Prompts the user to confirm a change.
 *
 * Displays the original and modified versions of a text segment (e.g., a line or a word occurrence)
 * and asks the user to confirm the change. The user's answer is interpreted as an index based on
 * a pre-defined set of responses (e.g., "Yes", "No", "All", "Quit").
 *
 * @param old The original text segment before the change.
 * @param oldNb The line number or identifier associated with the original text.
 * @param new The text segment after the proposed change.
 * @param newNb The line number or identifier associated with the modified text.
 * @return An integer representing the user's decision:
 *         0 - Confirm change;
 *         1 - Reject change;
 *         2 - Apply change to all further occurrences;
 *         3 - Quit the process.
 */
int confirm_User_Changes(char* old, int oldNb, char* new, int newNb);

/**
 * @brief Displays the final changes made to the file by the program.
 *
 * Outputs a summary of the modifications made by the program to the file. The summary includes
 * the total number of words replaced and the number of lines that were modified.
 *
 * @param changes A pointer to a file_changes structure that holds statistics about the changes.
 */
void display_changes(struct file_changes* changes);

#endif