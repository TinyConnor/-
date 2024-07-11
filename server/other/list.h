#ifndef __LIST_H__
#define __LIST_H__
// #include <head.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct employee
{
    char id[7];
    struct employee* next;
}em_t;

int creat_list(em_t** phead, char* data);
int insert_list(em_t* phead, char* data);
int delete_list(em_t* phead, char* data);
int seek_list(em_t* phead, char* data);

#endif // !__LIST_H__

