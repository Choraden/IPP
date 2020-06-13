#include "batch.h"
#include "interactive.h"

void READ(){
    int i, poz = 0, cnt = 0, ignore = 0, size[6] = {0, 0, 0, 0, 0, 0}, memSize[6];
    int last = ' ', *com[6], first = 0, mode = 0, white = 0;
    unsigned int line = 1;
    gamma_t *g = NULL;

   Malloc(com, memSize);

    while((i = getchar()) != EOF){

        if(isspace(i) && i != '\n')
            white = 1;

        // jesli pierwszy znak to "#"
        if(first == 0 && i == '#')
            ignore = 1;

        if(first == 0 && isspace(i) && i != '\n')
            cnt = 2;
        

        first = 1;

        // Jesli jest to koniec linii sprawdza warunki brzegowe i przekazuje komendy do wykonania.
        if(i == '\n'){
            
            /*printf("LINE: %d ", line);
            for(int k = 0; k < 6; k++){
                for(int l = 0; l < size[k]; l++){
                    printf("%c ", com[k][l]);
                }
                
            }
            printf("\n");
            */
            if((cnt > 5 && ignore == 0) || (white == 1 && cnt == 0)) 
                fprintf(stderr, "ERROR %u\n", line); 

            else if(ignore == 0 && cnt > 0){
                
                int res = handler(cnt, com, size);

                if(res == 0)
                    fprintf(stderr, "ERROR %u\n", line); 

                else if(res == 1 && mode == 1){
                    char *p = gamma_board(g);
                    if(p == NULL)
                        fprintf(stderr, "ERROR %u\n", line);
                    else{
                        printf("%s", p);
                        free(p);
                    }

                }else if((res == 2 || res == 3 || res == 4) && mode == 1){
                    uint32_t player = calculate_number(com[1], size[1]);

                    if(player == 0)
                        fprintf(stderr, "ERROR %u\n", line); 

                    else if(res == 2){
                        printf("%d\n", gamma_golden_possible(g, player));

                    }else if(res == 3){
                        printf("%" PRIu64 "\n", gamma_free_fields(g, player));

                    }else if(res == 4){
                        printf("%" PRIu64 "\n", gamma_busy_fields(g, player));
                        
                    }

                }else if((res == 5 || res == 6) && mode == 1){
                    uint32_t player = calculate_number(com[1], size[1]);
                    uint32_t x = calculate_number(com[2], size[2]);
                    uint32_t y = calculate_number(com[3], size[3]);
                    if(player == 0)
                        fprintf (stderr, "ERROR %u\n", line); 

                    else if(res == 5){ 
                        printf("%d\n", gamma_move(g, player, x, y));
                    
                    }else if(res == 6){
                        printf("%d\n", gamma_golden_move(g, player, x, y));
                
                    }

                }else if((res == 7 || res == 8) && mode == 0){
                    uint32_t width = calculate_number(com[1], size[1]);
                    uint32_t height = calculate_number(com[2], size[2]);
                    uint32_t players = calculate_number(com[3], size[3]);
                    uint32_t areas = calculate_number(com[4], size[4]);
                    g = gamma_new(width, height, players, areas);
                    if(g == NULL)
                        fprintf (stderr, "ERROR %u\n", line); 
                    
                    else if(res == 7){
                        printf("OK %u\n", line);
                        mode = 1;
                    }else if(res == 8){
                        interactive(g);
                        FREE(com);
                        gamma_delete(g);
                        return;
                    }

                }else 
                    fprintf (stderr, "ERROR %u\n", line);

            }

            first = ignore = cnt = poz = white = 0;
            line++;
            ClearALL(com, memSize);
        }
        // Wczytany znak nie jest znakiem białym.
        if(!isspace(i)){

            if(isspace(last)){
                cnt++;
                poz = 0;
            }

            if(cnt < 6){
                if(poz + 1 == memSize[cnt - 1]) 
                    Realloc(&com[cnt - 1], &memSize[cnt - 1], poz);

                com[cnt - 1][poz++] = i;
                size[cnt - 1] = poz;
            }
        }
        last = i;
    }

    // Jesli zakonczyl sie plik jednak buffer jest zajety. 
    if((cnt != 0 || poz != 0 || first != 0) && ignore == 0)
        fprintf(stderr, "ERROR %u\n", line); 

    FREE(com);
    gamma_delete(g);
    return;
}