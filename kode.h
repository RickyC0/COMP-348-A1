#ifndef KODE_H
#define KODE_H

#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "fileread.h"
#include "wreplace.h"

enum Command {
    CMD_RC, // Replace-Keep (for example)
    CMD_RI, // Replace-Ignore
    CMD_UK, // Update-Keep
    CMD_UM  // Update-Modify
};

int get_command_code(const char *cmd_str);


#endif 