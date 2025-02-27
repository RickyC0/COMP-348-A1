#include "kode.h"
#include "ui.h"
#include "wreplace.h"
#include "fileread.h"

int get_command_code(const char *cmd_str) {
    if (strcmp(cmd_str, "RC") == 0)
        return CMD_RC;
    else if (strcmp(cmd_str, "RI") == 0)
        return CMD_RI;
    else if (strcmp(cmd_str, "UK") == 0)
        return CMD_UK;
    else if (strcmp(cmd_str, "UM") == 0)
        return CMD_UM;
    else
        return -1; 
}

int main(int argc, char *argv[]){
    char* command_str=argv[1];
    char* word=argv[2];
    

    struct file_changes changes = {0, 0, 0, 0};


    if(validate_Argv(argc,argv)!=1){
        fprintf(stderr,"CLI Argument Error.\nTerminating App.");
        return 3;
    }
    FILE* file;

    if((file = check_file(argv[3])) == NULL){
        fprintf(stderr,"File Error.\nTerminating App.");
        return 3;
    }

    fprintf(stdout,"Reading File\n");

    int command=get_command_code(command_str);
    

    struct file_changes* temp_changes=search_and_change_file(file,word,command,&changes);
    changes=*temp_changes;
    
    display_changes(&changes);

    return 0;
}