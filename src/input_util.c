#include "input_util.h"

void Malloc(int *com[], int size[]){
    for(int i = 0; i < 5; i++){
        size[i] = 32;
        com[i] = malloc(size[i] * sizeof(int));
        if(com[i] == NULL) 
            exit(1);
    }
}
void Realloc(int **tab, int *size, int poz){
    *size *= 2;
    *tab = realloc(*tab, sizeof(int) * (*size));
    if(tab == NULL) 
        exit(1);
        
    for(int i = poz; i < *size; i++)
        (*tab)[i] = 0;
}

void Clear(int *tab, int size){
    for(int i = 0; i < size; i++)
        tab[i] = 0;
}

void ClearALL(int *com[], int size[]){
    for(int i = 0; i < 5; i++){
        Clear(com[i], size[i]);
    }
}

void FREE(int *com[]){
    for(int i = 0; i < 5; i++)
        if(com[i] != NULL)
            free(com[i]);
}