#include "handle.h"

int is_number(int *order, int size){
    int res = 1;
    for(int i = 0; i < size; i++)
        if(!isdigit(order[i]))
            res = 0;
    
    return res;
}

uint32_t calculate_number(int *digits, int size){
    uint64_t res_64 = 0;
    uint32_t res_32 = 0;
    for(int i = 0; i < size; i++)
        res_64 = i == size - 1 ? res_64 + digits[i] - '0' : (res_64 + digits[i] - '0') * 10;

    res_32 = res_64;
    if(res_32 != res_64)
        res_32 = 0;
    
    return res_32;
}

int are_numbers(int *com[], int size[], int k){
    int res = 1;
    for(int i = 1; i <= k; i++)
        if(is_number(com[i], size[i]) == 0)
            res = 0;
    
    return res;
}

int handler(int cnt, int *com[] ,int size[]){
    int res = 0;

    if(cnt == 1 && com[0][0] == 'p' && size[0] == 1)
        res = 1;
    
    else if(cnt == 2 && is_number(com[1], size[1])){
        if(com[0][0] == 'q' && size[0] == 1)
            res = 2;

        else if(com[0][0] == 'f' && size[0] == 1)
            res = 3;
        
        else if(com[0][0] == 'b' && size[0] == 1)
            res = 4;

    }else if(cnt == 4 && are_numbers(com, size, 3)){
        if(com[0][0] == 'm' && size[0] == 1)
            res = 5;
        
        else if(com[0][0] == 'g' && size[0] == 1)
            res = 6;

    }else if(cnt == 5 && are_numbers(com, size, 4)){
        if(com[0][0] == 'B' && size[0] == 1)
            res = 7;
        
        else if(com[0][0] == 'I' && size[0] == 1)
            res = 8;
    }
        
    return res;
}