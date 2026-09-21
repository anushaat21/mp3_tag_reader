#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types.h"
#include"edit.h"

Status read_and_validate_edit_args(int argc,char *argv[], EditInfo *eInfo)
{
    /* ./a.out -e <tag> <new data> sample.mp3 */
    if(argc<5)
        return e_failure;
    
    if(strcmp(argv[2],"-t")==0 ||                   //validate tag data
       strcmp(argv[2],"-A")==0 ||
       strcmp(argv[2],"-a")==0 ||
       strcmp(argv[2],"-y")==0 ||
       strcmp(argv[2],"-c")==0 ||
       strcmp(argv[2],"-C")==0)
    {
        eInfo->tag = argv[2];
    }
    else
    {
        return e_failure;
    }

    eInfo->new_data = argv[3];                      //validate new data

    char *ext = strrchr(argv[4], '.');              //check extension
    if(ext == NULL || strcmp(ext, ".mp3") != 0)
    {
        printf("Invalid file format.\n");
        return e_failure;
    }
    eInfo->src_mp3_fname = argv[4];                 //check all argv
    return e_success;
}

Status open_tfiles(EditInfo *eInfo)
{
    eInfo->fptr_src = fopen(eInfo->src_mp3_fname, "rb");        //open source file in read mode
    if(eInfo->fptr_src == NULL)
    {
        printf("Unable to open source file\n");                 //check it is opened or not
        return e_failure;
    }

    eInfo->dest_mp3_fname = "temp.mp3";                     //create temp file for output 

    eInfo->fptr_dest = fopen(eInfo->dest_mp3_fname, "wb");      //open temp file in write mode
    if(eInfo->fptr_dest == NULL)
    {
        printf("Error: Unable to create destination file\n");       //check it is opened or not
        return e_failure;
    }
    return e_success;
}

Status do_edit(EditInfo *eInfo)
{
    char header[10];
    if(fread(header,1,10,eInfo->fptr_src) != 10)        //read 10 bytes of header
        return e_failure;
    if(strncmp(header,"ID3",3) != 0)                  //check header is in ID3 or not
    {
        printf("No ID3 tag found\n");
        return e_failure;
    }
    fwrite(header,1,10,eInfo->fptr_dest);               //write it in temp file

    char frame_id[5];
    frame_id[4] = '\0';

    char target_frame[5];

    if(strcmp(eInfo->tag, "-t") == 0)                   //convert option into frame id
        strcpy(target_frame, "TIT2");
    else if(strcmp(eInfo->tag, "-A") == 0)
        strcpy(target_frame, "TPE1");
    else if(strcmp(eInfo->tag, "-a") == 0)
        strcpy(target_frame, "TALB");
    else if(strcmp(eInfo->tag, "-y") == 0)
        strcpy(target_frame, "TYER");
    else if(strcmp(eInfo->tag, "-c") == 0)
        strcpy(target_frame, "TCON");
    else if(strcmp(eInfo->tag, "-C") == 0)
        strcpy(target_frame, "COMM");

    while(fread(frame_id,1,4,eInfo->fptr_src) == 4)     //read 4 bytes of flag data from src file
    {
        char size_bytes[4];
        char flags[2];

        if(fread(size_bytes,1,4,eInfo->fptr_src) != 4)
            break;
        if(fread(flags,1,2,eInfo->fptr_src) != 2)
            break;

        int size = ((unsigned char)size_bytes[0] << 24) | ((unsigned char)size_bytes[1] << 16) | 
                    ((unsigned char)size_bytes[2] << 8)  | (unsigned char)size_bytes[3];

        if(strcmp(frame_id, target_frame) == 0)
        {
            fwrite(frame_id,1,4,eInfo->fptr_dest);              //Write in frame id

            int new_size = strlen(eInfo->new_data) + 1;

            char new_size_bytes[4];
            new_size_bytes[0] = (new_size >> 24) & 0xFF;
            new_size_bytes[1] = (new_size >> 16) & 0xFF;
            new_size_bytes[2] = (new_size >> 8)  & 0xFF;
            new_size_bytes[3] = new_size & 0xFF;

            fwrite(new_size_bytes,1,4,eInfo->fptr_dest);        //write 4 bytes in temp file ot output file
            fwrite(flags,1,2,eInfo->fptr_dest);

            char encoding = 0x00;
            fwrite(&encoding,1,1,eInfo->fptr_dest);
            fwrite(eInfo->new_data, 1,
                   strlen(eInfo->new_data),
                   eInfo->fptr_dest);

            fseek(eInfo->fptr_src,size,SEEK_CUR);           //skip old data
            
            char buffer2[1024];                             //copy remaining audio data
            size_t bytes;
            while((bytes = fread(buffer2,1,1024,eInfo->fptr_src)) > 0)  //read from src file and store it in output file
            {
                fwrite(buffer2,1,bytes,eInfo->fptr_dest);
            }
            break;
        }
        else
        {
            fwrite(frame_id,1,4,eInfo->fptr_dest);          //copy the frame as it is
            fwrite(size_bytes,1,4,eInfo->fptr_dest);
            fwrite(flags,1,2,eInfo->fptr_dest);

            char *buffer = malloc(size);                    //allocate memory dynamically for buffer
            if(buffer == NULL)
                return e_failure;
            if(fread(buffer,1,size,eInfo->fptr_src) != size)    //read from src file
                return e_failure;
            fwrite(buffer,1,size,eInfo->fptr_dest);         //store in output file
            free(buffer);
        }
    }
    fclose(eInfo->fptr_src);                //close the file src file after usage
    fclose(eInfo->fptr_dest);               //close the output file after usage

    remove(eInfo->src_mp3_fname);
    rename("temp.mp3", eInfo->src_mp3_fname);       //rename temp.mp3 file with sample.mp3
    return e_success;
}