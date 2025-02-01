#include "header.h"

int main(int argc, char *argv[])
{
    if(check_operation_type(argv) == e_view)
    {
        ViewInfo vInfo;
        vInfo.fptr_mp3 = NULL;
        
        if(read_n_validate_view_args(argv, &vInfo) == e_success)
        {
            if(view(&vInfo) == e_success)
            {
                printf("INFO: View Successful\n");
            }
            else
            {
                printf("INFO: View Failed\n");
            }
        }
        else
        {
            printf("INFO: Error in Validating Arguments\n");
        }

        if(vInfo.fptr_mp3 != NULL)
        {
            fclose(vInfo.fptr_mp3);
        }
    }
    else if(check_operation_type(argv) == e_edit)
    {
        EditInfo eInfo;
        eInfo.fptr_edit_mp3 = NULL;
        eInfo.new_mp3_name = "new.mp3";
        
        if(read_n_validate_edit_args(argv, &eInfo) == e_success)
        {
            if(edit(&eInfo) == e_success)
            {
                printf("INFO: Edit Successful\n");
                remove(eInfo.edit_mp3_name);
            }
            else
            {
                printf("INFO: Edit Failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("INFO: Error in Validating Arguments\n");
            return e_failure;
        }

        if(eInfo.fptr_edit_mp3 != NULL)
        {
            fclose(eInfo.fptr_edit_mp3);
        }

        if(eInfo.fptr_new_mp3 != NULL)
        {
            fclose(eInfo.fptr_new_mp3);
            rename("new.mp3", "sample.mp3");
        }
    }
    else if(check_operation_type(argv) == e_help)
    {
        printf("In view, -v represents user wish to see the contents of the mp3_file.\n");
        printf("In edit, -e represents user wish to edit the contents of the mp3_file.\n");
        printf("In edit, -t after -e represents the title tag\n");
        printf("In edit, -a after -e represents the artist(s) tag\n");
        printf("In edit, -A after -e represents the album tag\n");
        printf("In edit, -m after -e represents the mediator tag\n");
        printf("In edit, -y after -e represents the year tag\n");
        printf("In edit, -c after -e represents the composer tag\n");
    }
    else
    {
        printf("INVALID ARGUMENTS\nUSAGE :\n");
        printf("To view, pass --> ./a.out -v mp3_file\n");
        printf("To edit, pass --> ./a.out -e -t/-a/-A/-m/-y/-c replacement_data mp3_file\n");
        printf("To get help, pass --> ./a.out -h\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(argv[1] != NULL && strcmp(argv[1], "-v") == 0)
    {
        return e_view;
    }
    else if(argv[1] != NULL && strcmp(argv[1], "-e") == 0)
    {
        return e_edit;
    }
    else if(argv[1] != NULL && strcmp(argv[1], "-h") == 0)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}