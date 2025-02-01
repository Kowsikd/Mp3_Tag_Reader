#include "header.h"

Status read_n_validate_view_args(char *argv[],ViewInfo *vInfo)
{
    if(argv[2] != NULL && strcmp(strstr(argv[2], "."), ".mp3") == 0)
    {
        vInfo->mp3_file_name = argv[2];
    }
    else
    {
        return e_failure;
    }

    return e_success;
}

Status open_files(ViewInfo *vInfo)
{
    printf("INFO: Opening required files\n");

    vInfo->fptr_mp3 = fopen(vInfo->mp3_file_name, "r");
    printf("INFO: Opened %s\n", vInfo->mp3_file_name);
    
    // Do Error handling
    if (vInfo->fptr_mp3 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", vInfo->mp3_file_name);

    	return e_failure;
    }

    return e_success;
}

Status view(ViewInfo *vInfo)
{
    if(open_files(vInfo) == e_success)
    {
        printf("INFO: Done\n");
        printf("INFO: Viewing Contents\n");

        fseek(vInfo->fptr_mp3, 10, SEEK_SET);

        char tag_name[5];
        int tag_data_size;

        for(int i = 0; i < 8; i++)
        {
            fread(tag_name, sizeof(char), 4, vInfo->fptr_mp3);
            tag_name[4] = '\0';

            fseek(vInfo->fptr_mp3, 3, SEEK_CUR);
            fread(&tag_data_size, sizeof(int), 1, vInfo->fptr_mp3);

            char tag_data[tag_data_size];
            fread(tag_data, sizeof(char), tag_data_size - 1, vInfo->fptr_mp3);
            tag_data[tag_data_size - 1] = '\0';
            if(i == 5 || i == 6)
            {
                continue;
            }
            printf("TAG : %s\n", tag_name);
            printf("DATA: %s\n", tag_data);
        }
    }
    else
    {
        printf("INFO: Failed to open files.\n");
        return e_failure;
    }

    return e_success;
}