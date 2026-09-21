/* Shakuntala M K (25036A)
Progect on Mp3 tag reader and edit 
An MP3 Tag Reader and Editor is a program that ,
can read and modify the metadata (information) stored inside an MP3 file.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types.h"
#include"view.h"
#include"edit.h"
void print_help()
{
    printf("\n===== MP3 TAG READER HELP MENU =====\n");     //print help menu

    printf("VIEW MP3 TAGS:\n");
    printf("Usage: ./a.out -v <mp3_filename>\n");
    printf("Example: ./a.out -v sample.mp3\n");
    printf("EDIT MP3 TAGS:\n");
    printf("Usage: ./a.out -e <option> \"new_data\" <mp3_filename>\n\n");

    printf("Edit Options:\n");
    printf(" -t  Edit Title\n");
    printf(" -A  Edit Artist\n");
    printf(" -a  Edit Album\n");
    printf(" -y  Edit Year\n");
    printf(" -c  Edit Content type\n");
    printf(" -C  Edit Composser\n");

    printf("Example:\n");
    printf("./a.out -e -t \"New Song\" sample.mp3\n");
    printf("====================================\n");
}

OperationType check_operation_type(char *);
int main(int argc, char *argv[])
{
    if(argc<2)                                              //check the srguments
    {
        print_help();
        return e_failure;
    }
    OperationType op_type = check_operation_type(argv[1]);
    if(op_type == e_view)                       //check the operation type
    {
        FileInfo vInfo;
        if(read_and_validate_view_args(argc,argv,&vInfo)== e_success)   //check the other arguments
        {
            printf("view arguments read sucessfully\n");
        }
        else
        {
            printf("view arguments not read\n");
            return e_failure;
        }
        if(do_view(&vInfo)==e_success)
        {
            printf("Data viewed and printed sucessfully\n");
        }
        else
        {
            printf("data is not viewed and printed\n");
            return e_failure;
        }
    }
    else if(op_type == e_edit)                                      //check operation type
    {
        EditInfo eInfo;
        if(read_and_validate_edit_args(argc,argv,&eInfo) == e_success)      //check other arguments
        {
            printf("edit arguments read sucessfully\n");
        }
        else
        {
            printf("edit argument are not read\n");
            return e_failure;
        }
        if(open_tfiles(&eInfo) == e_success)
        {
            printf("File opened sucessfully\n");
        }
        else
        {
            printf("file not opened\n");
            return e_failure;
        }
        if(do_edit(&eInfo) == e_success)
        {
            printf("do edit completed sucessfully\n");
        }
        else
        {
            printf("edit not done\n");
        }
    }
    else if(op_type == e_help)          //check operation type
    {
        print_help();
        return e_success;
    }
    else
    {
        printf("Invalid operation\n");
        print_help();
        return e_failure;
    }
    return e_success;
}
OperationType check_operation_type(char *symbol)
{
    if(strcmp(symbol,"-v")==0)              //check the symblos
    {
        return e_view;
    }
    else if(strcmp(symbol,"-e")==0)
    {
        return e_edit;
    }
    else if(strcmp(symbol,"-h")==0)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}   
