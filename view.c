#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types.h"
#include"view.h"

Status read_and_validate_view_args(int argc,char *argv[], FileInfo *vInfo)
{
    /* ./a.out -v sample.mp3.*/
    if(argc<3)                      //check argument count
        return e_failure;
    char *ext;
    if(argv[2] == NULL)
        return e_failure;
    ext = strrchr(argv[2],'.');         //check with extention
    if(ext == NULL || strcasecmp(ext,".mp3")!=0)
        return e_failure;
    vInfo->src_mp3_fname = argv[2];
    return e_success;
}

Status open_files(FileInfo *vInfo)
{
    vInfo->fptr = fopen(vInfo->src_mp3_fname,"rb");     //open file in read mode
    if(vInfo->fptr == NULL)
    {
        return e_failure;
    }
    printf("open file:%s\n",vInfo->src_mp3_fname);      //check file opend or not
    return e_success;
}

Status do_view(FileInfo *vInfo)
{
    printf("Inside do_view\n");
    if(open_files(vInfo) == e_failure)      //check condition for opening files
    {
        printf("Files are not opened\n");
        return e_failure;
    }
    else
        printf("Files opened\n");
    fseek(vInfo->fptr,10,SEEK_SET);         //Skip 10 bytes of header
    for(int i=0;i<6;i++)
    {
        char tag[5] = {0};              //Frame 
        unsigned char size_arr[4];
        int size;
        char data[1000] = {0};          //buffer tag data
        fread(tag,1,4,vInfo->fptr);         //read 4 bytes of tag
        fread(size_arr,1,4,vInfo->fptr);        //read 4 bytes of size
        size = (size_arr[0] << 24) |
       (size_arr[1] << 16) |
       (size_arr[2] << 8)  |
       (size_arr[3]);

        fseek(vInfo->fptr,2,SEEK_CUR);          //skip 2 bytes of flag
        if(size > sizeof(data))
        {
            printf("Frame too large\n");
            return e_failure;
        }
        unsigned char encoding;
        fread(&encoding,1,1,vInfo->fptr);           //Read size bytes data
        fread(data,1,size-1,vInfo->fptr);            
        data[size-1] = '\0';
        printf("%s : ",tag);
        fwrite(data,1,size-1,stdout);
        printf("\n");
    }
    fclose(vInfo->fptr);                            //close the file after usage
    return e_success;
}