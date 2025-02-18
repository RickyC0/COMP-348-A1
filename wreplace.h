#ifndef WREPLACE_H
#define WREPLACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Replace a word in the buffer with a replacement character.
void replace_Word(char* buffer, size_t word_len,size_t index, char replacement);

#endif