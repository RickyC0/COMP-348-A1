#ifndef FILEREAD_H
#define FILEREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct file_changes{
    int nb_lines, nb_lines_changed, nbwords, nb_words_changed;

};

static int global_apply_all = 0;

/**
 * @brief Checks if the file is not null
 * 
 * @param file_name 
 * @return FILE* 
 */
FILE* check_file(char* file_name);

/**
 * Processes a given command on the file buffer.
 *
 * This function examines the command specified by the 'command' parameter
 * and applies the corresponding operation on the file's content in 'buffer'.
 * It uses the target word (and its length) to search for occurrences in the buffer,
 * updates the 'changes' structure with statistics (such as the number of words changed
 * and lines modified), and returns an updated pointer to the file_changes structure.
 *
 * @param command The command code that determines the operation to perform.
 * @param buffer  The character buffer containing the file's content.
 * @param read_size The number of characters read into the buffer.
 * @param word_len The length of the target word.
 * @param word    The target word to search for within the buffer.
 * @param changes Pointer to a file_changes structure used to record change statistics.
 * @return A pointer to the updated file_changes structure.
 */
struct file_changes* process_command(int command, char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);

/**
 * Reads a file, searches for the target word, and applies changes based on a specified command.
 *
 * This function opens the file, reads its entire content into a buffer, and then processes
 * the content by searching for occurrences of the target word. It applies the changes
 * as dictated by the command parameter (which may involve prompting the user for confirmation),
 * writes the modified content back to the file, and updates a file_changes structure that tracks
 * statistics such as the number of lines, words changed, and lines modified.
 *
 * @param file    The file pointer to the file to be processed.
 * @param word    The target word to search for within the file.
 * @param command The command code that specifies how changes should be applied.
 * @param changes Pointer to a file_changes structure that will be updated with change statistics.
 * @return A pointer to the updated file_changes structure.
 */
struct file_changes* search_and_change_file(FILE* file, char* word, int command, struct file_changes* changes);

/**
 * change_file_RC - Remove Case Sensitive
 *
 * Scans the file content for all occurrences of the target word and replaces them with
 * asterisks, matching the exact case of the provided word. The search is case-sensitive,
 * meaning only occurrences with identical case will be replaced. The replacement is done
 * in-place, and the function updates the 'changes' structure with the number of words replaced
 * and the number of lines modified.
 *
 * @param buffer     Pointer to the file's content as a character array.
 * @param read_size  Total number of characters read into the buffer.
 * @param word_len   Length of the target word.
 * @param word       The target word to be replaced.
 * @param changes    Pointer to a file_changes structure to record change statistics.
 *
 * @return           Pointer to the updated file_changes structure.
 */
struct file_changes* change_file_RC(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);

/**
 * change_file_RI - Remove Ignore Case
 *
 * Similar to change_file_RC, this function scans the file content for the target word and
 * replaces each occurrence with asterisks. However, the search is performed in a case-insensitive
 * manner: differences in letter case between the target word and the text are ignored. The replacement
 * is done in-place, and the changes structure is updated with the number of words replaced and lines modified.
 *
 * @param buffer     Pointer to the file's content as a character array.
 * @param read_size  Total number of characters in the buffer.
 * @param word_len   Length of the target word.
 * @param word       The target word to be replaced.
 * @param changes    Pointer to a file_changes structure to record change statistics.
 *
 * @return           Pointer to the updated file_changes structure.
 */
struct file_changes* change_file_RI(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);

/**
 * change_file_UK - Un-remove Keep Case
 *
 * Searches the file content for sequences of asterisks that exactly match the length of the target word,
 * indicating that the word may have been previously removed. In "Un-remove Keep Case" mode, the exact
 * target word is restored in place of the asterisks, preserving the original case as provided. The function
 * prompts the user to confirm each restoration before applying the change. The changes structure is updated
 * with the statistics for restored words and modified lines.
 *
 * @param buffer     Pointer to the file's content as a character array.
 * @param read_size  Total number of characters in the buffer.
 * @param word_len   Length of the target word.
 * @param word       The target word to restore.
 * @param changes    Pointer to a file_changes structure to record change statistics.
 *
 * @return           Pointer to the updated file_changes structure.
 */
struct file_changes* change_file_UK(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);

/**
 * change_file_UM - Un-remove Match Case
 *
 * Also searches the file content for sequences of asterisks matching the target word’s length, suggesting
 * that the word might have been removed. In "Un-remove Match Case" mode, the user is prompted for each
 * occurrence to confirm whether the removed word should be restored. This mode typically performs a case-insensitive
 * comparison during the search, but once confirmed, the exact word (as provided) is used for restoration.
 * The replacement is applied in-place, and the changes structure is updated with the corresponding statistics.
 *
 * @param buffer     Pointer to the file's content as a character array.
 * @param read_size  Total number of characters in the buffer.
 * @param word_len   Length of the target word.
 * @param word       The target word to restore.
 * @param changes    Pointer to a file_changes structure to record change statistics.
 *
 * @return           Pointer to the updated file_changes structure.
 */
struct file_changes* change_file_UM(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);

int check_exact_asterisk(size_t word_len, size_t read_size, char* buffer, int index);

#endif