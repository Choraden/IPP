
#include "util.h"

bool check_borders_with_move(gamma_t *g, uint32_t x, int move_x, uint32_t y, int move_y){

    if(x == 1 && move_x == -1)
        return false;
    
    if(x == g->width && move_x == 1)
        return false;

    if(y == 1 && move_y == -1)
        return false;

    if(y == g->height && move_y == 1)
        return false;        

    return true;     
}

bool check_borders(gamma_t *g, uint32_t x, uint32_t y){
    if(x < 1 || x > g->width || y < 1 || y > g->height)
        return false;

    return true;
}

bool check_conditions(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){
    if(g == NULL)
        return false;

    if(player < 1 || player > g->players_number)
        return false;
    
    return check_borders(g, x, y);
}

uint64_t get_adjoined(gamma_t *g, uint32_t player, uint32_t x, int move_x, uint32_t y, int move_y){
    if(check_borders_with_move(g, x, move_x, y, move_y) == false)
        return 0; 

    x += move_x;
    y += move_y;

    if(g->board[y][x] == player) 
        return find(g, get_number(g, y, x));
    else 
        return 0;
}

void dfs(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, uint32_t ignore_x, uint32_t ignore_y){

    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    g->tools->visited[y][x] = player;

    for(int dir = up; dir <= left; dir++){
        if(check_borders_with_move(g, x, x_moves[dir], y, y_moves[dir])){
            uint32_t new_x = x + x_moves[dir];
            uint32_t new_y = y + y_moves[dir];
            if(g->board[new_y][new_x] == player 
            && g->tools->visited[new_y][new_x] == 0
            && (new_x != ignore_x || new_y != ignore_y)
            )
                dfs(g, player, new_x, new_y, ignore_x, ignore_y);            
        }
    }
    return; 
}

void clear_vis (gamma_t *g){
    for(uint32_t i = 1; i <= g->height; i++){
        for(uint32_t j = 1; j <= g->width; j++)
            g->tools->visited[i][j] = 0;
    }
}

bool set_areas(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){

    
    int temp = 0;
    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    uint32_t act_x = x, act_y = y;
    uint64_t act_dir = find(g, get_number(g, y, x)), new_dir;
    
    // Sprawdza czy przyległe pola posiadają tego samego reprezentanta, jeśli nie to je łączy.
    for(int dir = up; dir <= left; dir++){
  
        new_dir = get_adjoined(g, player, x, x_moves[dir], y, y_moves[dir]);
  
        if(new_dir != 0){
            if(act_dir != new_dir){
                Union(g, act_x, act_y, x + x_moves[dir], y + y_moves[dir]);
                temp++;
                act_dir = new_dir;
                act_x = x + x_moves[dir];
                act_y = y + y_moves[dir];
            }
        }
    }

    if(temp == 0 && g->players[player].taken_areas + 1 > g->areas)
        return false;

    else if(temp == 0){
        g->players[player].taken_areas++;

    }else if(temp > 0){
        g->players[player].taken_areas -= (temp - 1);  
    }     

    return true; 
}

void dfs_set_new_parents(gamma_t *g,
                        uint32_t player, 
                        uint32_t x, 
                        uint32_t y, 
                        uint32_t new_parent, 
                        uint32_t ignore_x, 
                        uint32_t ignore_y){

    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    g->tools->visited[y][x] = player;
    g->tools->parent[get_number(g, y, x)] = new_parent;

    for(int dir = up; dir <= left; dir++){
        if(check_borders_with_move(g, x, x_moves[dir], y, y_moves[dir])){
            uint32_t new_x = x + x_moves[dir];
            uint32_t new_y = y + y_moves[dir];
            if(g->board[new_y][new_x] == player
                && g->tools->visited[new_y][new_x] == 0
                && (new_x != ignore_x || new_y != ignore_y))
                dfs_set_new_parents(g, player, new_x, new_y, new_parent, ignore_x, ignore_y);            
        }
    }
    return; 
}
void set_new_parents(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){

    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    uint32_t arr[4], old_player = g->board[y][x];
    bool old_player_adjoined = false;
    g->players[old_player].divided_fields = 0;
    for(int dir = up; dir <= left; dir++){
        arr[dir] = 0;
        if(check_borders_with_move(g, x, x_moves[dir], y, y_moves[dir])){

            uint32_t new_x = x + x_moves[dir];
            uint32_t new_y = y + y_moves[dir];
            uint32_t act_player = g->board[new_y][new_x];
            arr[dir] = act_player;
            if(act_player == old_player)
                old_player_adjoined = true;
            
            if(g->tools->visited[new_y][new_x] == 0 
            && act_player == old_player
            && act_player != player){

                g->players[act_player].divided_fields++;
                dfs_set_new_parents(g,
                                    act_player, 
                                    new_x, new_y, 
                                    get_number(g, new_y, new_x),
                                    x,
                                    y);
                
            }     
        }
    }
    for(int i = up; i <= left; i++){
        if(g->players[arr[i]].divided_fields != 0)
            g->players[arr[i]].taken_areas += (g->players[arr[i]].divided_fields - 1);
        g->players[arr[i]].divided_fields = 0;
    }
    if(!old_player_adjoined)
        g->players[old_player].taken_areas--;
}

bool set_areas_for_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){

    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    uint32_t old_player = g->board[y][x]; 
    // Sprawdza czy przy wyłączeniu pola (x, y) pola nadal są w jednym obszarze. 
    g->players[old_player].divided_fields = 0;
    for(int dir = up; dir <= left; dir++){

        if(check_borders_with_move(g, x, x_moves[dir], y, y_moves[dir])){
            
            uint32_t new_x = x + x_moves[dir];
            uint32_t new_y = y + y_moves[dir];
            uint32_t act_player = g->board[new_y][new_x];
            if(g->tools->visited[new_y][new_x] == 0 && act_player == old_player){

                g->players[act_player].divided_fields++;
                uint32_t curr_areas = g->players[act_player].taken_areas;
                if(curr_areas + g->players[act_player].divided_fields - 1 > g->areas)
                    return false;
                
                dfs(g, act_player, new_x, new_y, x, y);
            }     
        }
    }

    clear_vis(g);

    if(set_areas(g, player, x, y))
        set_new_parents(g, player, x, y);

    else 
        return false;
    
    return true;
}

int is_already_possible(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, int dir){
    int act_dir;
    int counter = 3;
    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    for(int i = previous; i <= next; i++){

        act_dir = dir + i;

        if(act_dir < 0)
            act_dir = 3;

        act_dir %= 4;

        if(check_borders_with_move(g, x, x_moves[act_dir], y, y_moves[act_dir]) == false)
            counter--;
        else if(g->board[y + y_moves[act_dir]][x + x_moves[act_dir]] != player)
            counter--;
    }
    if(counter == 0)
        return 1;
    else 
        return 0;    
}

int get_possible_moves(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){
    
    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    int res = 0;
    for(int dir = up; dir <= left; dir++){
        if(check_borders_with_move(g, x, x_moves[dir], y, y_moves[dir])){
            uint32_t new_x = x + x_moves[dir];
            uint32_t new_y = y + y_moves[dir];
            if(g->board[new_y][new_x] == 0){
                res += is_already_possible(g, player, new_x, new_y, dir);
            }
        }
    }

    return res;
}
void set_possible_moves(gamma_t *g, uint32_t player, uint32_t x, uint32_t y){
    
    const int x_moves[] = {0, 1, 0, -1};
    const int y_moves[] = {-1, 0, 1, 0};
    uint32_t arr[4], act_player;

    for(int dir = up; dir <= left; dir++){
        arr[dir] = 0;
        if(check_borders_with_move(g, x, x_moves[dir], y, y_moves[dir])){
            uint32_t new_x = x + x_moves[dir];
            uint32_t new_y = y + y_moves[dir];
            if(g->board[new_y][new_x] == 0){
                g->players[player].possible_moves += is_already_possible(g, 
                                                                        player, 
                                                                        new_x, 
                                                                        new_y, 
                                                                        dir
                                                                        );
            }else if(g->board[new_y][new_x] != 0){
                act_player = g->board[new_y][new_x];
                if(!g->players[act_player].possible_mv_del){
                    arr[dir] = act_player;
                    g->players[act_player].possible_mv_del = true;
                    g->players[act_player].possible_moves--;
                }
                
            }
        }
    }
    for(int i = up; i <= left; i++)
        if(arr[i] != 0)
            g->players[arr[i]].possible_mv_del = false;

}

uint32_t get_digits_number(uint32_t number){
    int res = 0;
    while(number != 0){
        res++;
        number /= 10;
    }
    return res;
}