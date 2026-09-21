#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types.h"

typedef struct _EditInfo
{
    char *src_mp3_fname;
    char *dest_mp3_fname;
    FILE *fptr_src;
    FILE *fptr_dest;

    char *tag;
    char *new_data;
}EditInfo;

//Edit function prototype

/* read and validate edit args from argv*/
Status read_and_validate_edit_args(int argc,char *argv[], EditInfo *eInfo);

/* Perform the view */
Status do_edit(EditInfo *eInfo);

/* Get File pointers for i/p and o/p files */
Status open_tfiles(EditInfo *eInfo);