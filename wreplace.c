#include "wreplace.h"


void replace_Word(char* buffer, size_t word_len, size_t index, char* replacement) {
    for (size_t j = 0; j < word_len; j++) {
        buffer[index + j] = replacement[j];
    }
}