#ifndef UI_H
#define UI_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fileread.h"

extern const char* user_answers[];

int validate_Argv(int argc, char** argv);

//Returns the index of the answer from user_answers
int confirm_User_Changes(char* old, int oldNb, char* new, int newNb);

void display_changes(struct file_changes* changes);

#endif