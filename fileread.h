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

FILE* check_file(char* file_name);

struct file_changes* process_command(int command, char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);

struct file_changes* change_file_RC(char* buffer, size_t read_size, size_t word_len, char* word, struct file_changes* changes);
struct file_changes* search_and_change_file(FILE* file, char* word, int command, struct file_changes* changes);

#endif