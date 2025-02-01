#include "header.h"

Status read_n_validate_edit_args(char *argv[],EditInfo *eInfo)
{
    if(argv[2] != NULL)
    {
        char ch = argv[2][1];
        switch(ch)
        {
            case 't':
                eInfo->edit_tag_name = "TIT2";
                break;
            case 'a':
                eInfo->edit_tag_name = "TPE1";
                break;
            case 'A':
                eInfo->edit_tag_name = "TALB";
                break;
            case 'y':
                eInfo->edit_tag_name = "TYER";
                break;
            case 'm':
                eInfo->edit_tag_name = "TCON";
                break;
            case 'c':
                eInfo->edit_tag_name = "TCOM";
                break;
            default:
                printf("Entered tag %s is incorrect or can't be modified.\n", argv[2]);
                return e_failure;
        }
    }
    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        int len = strlen(argv[3]);
        if(len > 0)
        {
            eInfo->edit_data_size = len + 1;
            eInfo->edit_data = argv[3];
        }
    }
    else
    {
        return e_failure;
        
    }

    if(argv[4] != NULL && strcmp(strstr(argv[4], "."), ".mp3") == 0)
    {
        eInfo->edit_mp3_name = argv[4];
    }
    else
    {
        return e_failure;
    }

    return e_success;
}

Status open_edit_files(EditInfo *eInfo)
{
    printf("INFO: Opening required files\n");

    eInfo->fptr_edit_mp3 = fopen(eInfo->edit_mp3_name, "r");
    printf("INFO: Opened %s\n", eInfo->edit_mp3_name);
    
    // Do Error handling
    if (eInfo->fptr_edit_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", eInfo->edit_mp3_name);

    	return e_failure;
    }

    eInfo->fptr_new_mp3 = fopen(eInfo->new_mp3_name, "w");
    printf("INFO: Opened %s\n", eInfo->new_mp3_name);
    
    // Do Error handling
    if (eInfo->fptr_new_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", eInfo->new_mp3_name);

    	return e_failure;
    }

    return e_success;
}

Status check_tag_name(EditInfo *eInfo, char *tag_name)
{
    if(strcmp(eInfo->edit_tag_name, tag_name) == 0)
    {
        return e_success;
    }

    return e_failure;
}

Status copy_remaining_data(FILE *fptr_edit_mp3, FILE *fptr_new_mp3)
{
    printf("INFO: Copying Left Over Data\n");
    char ch;
    while(fread(&ch, 1, 1, fptr_edit_mp3) > 0)
    {
        fwrite(&ch, 1, 1, fptr_new_mp3);
    }
    return e_success;
}

Status edit(EditInfo *eInfo)
{
    if(open_edit_files(eInfo) == e_success)
    {
        printf("INFO: Done\n");
        printf("INFO: Editing Content\n");

        char header[10];
        fread(header, sizeof(char), 10, eInfo->fptr_edit_mp3);
        fwrite(header, sizeof(char), 10, eInfo->fptr_new_mp3);

        char tag_name[5];
        char flag[4];
        int tag_data_size;
        
        for(int i = 0; i < 8; i++)
        {
            fread(tag_name, sizeof(char), 4, eInfo->fptr_edit_mp3);
            fread(flag, sizeof(char), 3, eInfo->fptr_edit_mp3);
            fread(&tag_data_size, sizeof(int), 1, eInfo->fptr_edit_mp3);
            char tag_data[tag_data_size];
            fread(tag_data, sizeof(char), tag_data_size - 1, eInfo->fptr_edit_mp3);
            tag_data[tag_data_size - 1] = '\0';
            
            if(check_tag_name(eInfo, tag_name) == e_success)
            {
                fwrite(tag_name, sizeof(char), 4, eInfo->fptr_new_mp3);
                fwrite(flag, sizeof(char), 3, eInfo->fptr_new_mp3);
                fwrite(&(eInfo->edit_data_size), sizeof(int), 1, eInfo->fptr_new_mp3);
                fwrite(eInfo->edit_data, sizeof(char), eInfo->edit_data_size + 1, eInfo->fptr_new_mp3);
                // printf("tag---> %s\n", tag_name);
                // printf("data---> %s\n", eInfo->edit_data);
                fseek(eInfo->fptr_new_mp3, -2, SEEK_CUR);
                printf("INFO: Tag Found and edited\n");
                break;
            }
            else
            {
                fwrite(tag_name, sizeof(char), 4, eInfo->fptr_new_mp3);
                fwrite(flag, sizeof(char), 3, eInfo->fptr_new_mp3);
                fwrite(&tag_data_size, sizeof(int), 1, eInfo->fptr_new_mp3);
                fwrite(tag_data, sizeof(char), tag_data_size - 1, eInfo->fptr_new_mp3);
                // printf("tag---> %s\n", tag_name);
                // printf("data---> %s\n", tag_data);
            }
        }

        if(copy_remaining_data(eInfo->fptr_edit_mp3, eInfo->fptr_new_mp3) == e_success)
        {
            printf("INFO: Done\n");
        }
        else
        {
            printf("INFO: Failed to Copy Remaining Data\n");
            return e_failure;
        }
    }
    else
    {
        printf("INFO: Failed to open files\n");
        return e_failure;
    }

    return e_success;
}