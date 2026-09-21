#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include"types.h"
typedef struct _FileInfo
{
    char *src_mp3_fname;
    FILE *fptr;

}FileInfo;

/* View function prototype */

/* Read and validate View args from argv */
Status read_and_validate_view_args(int argc,char *argv[], FileInfo *vInfo);

/* Perform the view */
Status do_view(FileInfo *vInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(FileInfo *vInfo);