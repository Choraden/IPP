#include "gamma.h"

gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas){
    
    if(width <= 0 || height <= 0|| players <= 0 || areas <= 0)
        return NULL;

    gamma_t *new_game = malloc(1 * sizeof(gamma_t));

    new_game->width = width;
    new_game->height = height;
    new_game->areas = areas;
    new_game->players_number = players;
    new_game->taken_fields = 0;

    new_game->board = malloc((height + 1) * sizeof(uint32_t*));
    if(new_game->board == NULL)
        return NULL;

    for(uint32_t i = 1; i <= height; i++){
        new_game->board[i] = calloc(width + 1, sizeof(uint32_t));
        if(new_game->board[i] == NULL)
            return NULL;
    }

    new_game->tools = malloc(1 * sizeof(util_t));
    if(new_game->tools == NULL)
        return NULL;

    new_game->tools->parent = malloc((height + 1) * (width + 1) * sizeof(uint64_t));
    if(new_game->tools->parent == NULL)
        return NULL;

    for(uint32_t i = 1; i <= height; i++){
        for(uint32_t j = 1; j <= width; j++){
            new_game->tools->parent[get_number(new_game, i, j)] = get_number(new_game, i, j);
        }
    }

    new_game->tools->visited = malloc((height + 1) * sizeof(bool*));
    if(new_game->tools->visited == NULL)
        return NULL;
    
    for(uint32_t i = 1; i <= height; i++){
        new_game->tools->visited[i] = malloc((width + 1) * sizeof(bool));
        if(new_game->tools->visited[i] == NULL)
            return NULL;  
    }

    for(uint32_t i = 1; i <= height; i++){
        for(uint32_t j = 1; j <= width; j++){
            new_game->tools->visited[i][j] = false;
        }
    }

    new_game->players = malloc((players + 1) *  sizeof(player_t));
    if(new_game->players == NULL)
        return NULL;

    for(uint32_t i = 1; i <= players; i++){
        new_game->players[i].golden_done = false;
        new_game->players[i].taken_fields = 0;
        new_game->players[i].possible_moves = 0;
        new_game->players[i].taken_areas = 0;
        new_game->players[i].possible_mv_del = false;
        new_game->players[i].divided_fields = 0;
    }

    return new_game;
}


void gamma_delete(gamma_t *g){
    if(g != NULL){
        free(g->players);

        for(uint32_t i = 1; i <= g->height; i++)
            free(g->tools->visited[i]);
        free(g->tools->visited); 

        free(g->tools->parent);

        free(g->tools);

        for(uint32_t i = 1; i <= g->height; i++)
            free(g->board[i]);

    
        free(g->board);
        free(g);
    }
    
    return;
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){

    x = x + 1;
    y = y + 1;

    if(check_conditions(g, player, x, y) == false || g->board[y][x] != 0) 
        return false;    
    
    if(set_areas(g, player, x, y) == true){
        g->board[y][x] = player;
        g->players[player].taken_fields++;
        g->taken_fields++;
        set_possible_moves(g, player, x, y);
    }else 
        return false;

    return true;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player){
    if(g == NULL || player < 1 || player > g->players_number)
        return false;
    
    if(g->taken_fields <= g->players[player].taken_fields)
        return false;

    if(g->players[player].golden_done)
        return false;
        
    return true;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){

    x = x + 1;
    y = y + 1;
    
    if  (!check_conditions(g, player, x, y) 
        || !gamma_golden_possible(g, player)
        || g->board[y][x] == 0 
        || g->board[y][x] == player
        ) 
        return false;
    uint32_t old_player = g->board[y][x];
    uint32_t lost_fields = get_possible_moves(g, old_player, x, y); 
    uint32_t gained_fields = get_possible_moves(g, player, x, y);
    if(set_areas_for_golden_move(g, player, x, y)){
        g->players[old_player].possible_moves -= lost_fields;
        g->players[old_player].taken_fields--;
        g->board[y][x] = player;
        g->players[player].possible_moves += gained_fields;
        g->players[player].taken_fields++;
        g->players[player].golden_done = true;
        clear_vis(g);
    }else{
        clear_vis(g);
        return false;
    }

    return true;
}


uint64_t gamma_busy_fields(gamma_t *g, uint32_t player){

    if(g == NULL || player < 1 || player > g->players_number)
        return 0;
    
    return g->players[player].taken_fields;
}


uint64_t gamma_free_fields(gamma_t *g, uint32_t player){
   
    if(g == NULL || player < 1 || player > g->players_number)
        return 0;
    
    if(g->players[player].taken_areas < g->areas)
       return  g->height * g->width - g->taken_fields;

    else if(g->players[player].taken_areas == g->areas){
        return g->players[player].possible_moves;
    }

    return 0;
}

char* gamma_board(gamma_t *g){
    if(g == NULL)
        return NULL;

    uint64_t board_size = (g->height + 1) * (g->width + 1);
    uint64_t it = 0;
    uint32_t cpy, space, digits = get_digits_number(g->players_number);
    int len;
    char buff[12];
    char *text = malloc((g->height + 1) * (g->width + 1) * digits * sizeof(char));  
    if(text == NULL)
        return NULL;  

    for(uint32_t i = g->height; i >= 1; i--){
        for(uint32_t j = 1; j <= g->width; j++){
            len = 0;
            cpy = g->board[i][j];
            space = digits;

            if(cpy == 0)
                buff[len++] = '.';

            while(cpy != 0){
                buff[len++] = (cpy % 10) + '0';
                cpy /= 10; 
            }
            
            space -= len;
            while(space > 0){
                space--;
                text[it++] = ' ';
            }

            for(int k = len - 1; k >= 0; k--){
                if(it + digits + 1 >= board_size){      
                    board_size *= 2; 
                    text = realloc(text, board_size * sizeof(char));
                    if(text == NULL)
                        return NULL;
                    
                }
                text[it++] = buff[k];
                space--;
            }
        
            if(g->players_number > 9)
                text[it++] = ' '; 
                
        }
        text[it++] = '\n';
    }
    text[it] = '\0';
    return text;
}


