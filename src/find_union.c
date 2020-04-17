#include "find_union.h"

uint64_t get_number(gamma_t *g, uint32_t height, uint32_t width){
    return (height - 1) * g->width + width; 
} 

uint64_t find(gamma_t *g, uint64_t val){
    if(g->tools->parent[val] != val)
        g->tools->parent[val] = find(g, g->tools->parent[val]);
    
    return g->tools->parent[val];
}

void Union(gamma_t *g, uint32_t x_a, uint32_t y_a, uint32_t x_b, uint32_t y_b){
    uint64_t a_root = find(g, get_number(g, y_a, x_a));
    uint64_t b_root = find(g,get_number(g, y_b, x_b));
    g->tools->parent[a_root] = b_root;
}