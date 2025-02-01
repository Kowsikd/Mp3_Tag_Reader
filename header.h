#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include "types.h"

typedef struct viewInfo
{
    FILE *fptr_mp3;
    char *mp3_file_name;
}ViewInfo;

typedef struct editInfo
{
    FILE *fptr_edit_mp3;
    FILE *fptr_new_mp3;
    const char *edit_mp3_name;
    const char *new_mp3_name;
    char *edit_tag_name;
    int edit_data_size;
    char *edit_data;
}EditInfo;

OperationType check_operation_type(char *argv[]);

Status read_n_validate_view_args(char *argv[],ViewInfo *vInfo);

Status open_files(ViewInfo *vInfo);

Status view(ViewInfo *vInfo);

Status read_n_validate_edit_args(char *argv[],EditInfo *eInfo);

Status open_edit_files(EditInfo *eInfo);

Status check_tag_name(EditInfo *eInfo, char *tag_name);

Status edit(EditInfo *eInfo);

#endif