#include "mysystem.h"

int creat_list(em_t** phead, char* data)
{
    printf("%p\n", phead);
    if (phead == NULL) {

        printf("list blank\n");
        return -1;
    }
    *phead = (em_t*) malloc(sizeof(em_t));
    if (*phead == NULL) {
        printf("malloc error\n");
        return -1;
    }
    (*phead)->next = NULL;
    strcpy((*phead)->id, data);
    return 0;
}
int insert_list(em_t* phead, char* data)
{
    if (phead == NULL) {
        printf("list blank\n");
        return -1;
    }
    em_t* pnew = NULL;
    creat_list(&pnew, data);
    pnew->next = phead->next;
    phead->next = pnew;
    return 0;
}
int delete_list(em_t* phead, char* data)
{
    if (phead == NULL) {
        printf("list blank\n");
        return -1;
    }
    em_t* ptemp = phead, * pdel = NULL;
    while (ptemp->next != NULL) {
        if (0 == strcmp(ptemp->id, data)) {
            pdel = ptemp;
            if (ptemp->next != NULL)
                ptemp = ptemp->next;
            ptemp = NULL;
            free(pdel);
            pdel = NULL;
            return 1;

        }
        ptemp = ptemp->next;
    }
    return 0;
}
int seek_list(em_t* phead, char* data)
{
    if (phead == NULL) {
        printf("list blank\n");
        return -1;
    }
    em_t* ptemp = phead;
    while (ptemp->next != NULL) {
        if (0 == strcmp(ptemp->id, data))
            return 1;
        ptemp = ptemp->next;
    }
    return 0;
}