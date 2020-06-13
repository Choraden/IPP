#include "interactive.h"

enum moves {move_up = 279165, move_down, move_right, move_left};

/** @brief Resetuje wypisywanie.
 */ 
static inline void reset(){
    printf("\x1b[0m");
}

/** @brief Czyści okno terminalu.
 */
static inline void clear(){
    printf("\e[1;1H\e[2J");
}

/** @brief Zapisuje pozycje kursora.
 */
static inline void saveCursorPosition(){
    printf("\x1b%d", 7);
}

/** @brief Przywraca pozycje kursora.
 */
static inline void restoreCursorPosition(){
    printf("\x1b%d", 8);
}

/** @brief Wypisuje czerowny tekst na czarnym tle.
 */
static inline void printRed(){
    printf("\x1b[31m\x1b[40m");
}

/** @brief Wypisuje zielony tekst na czarnym tle.
 */
static inline void PrintGreen(){
    printf("\x1b[32m\x1b[40m");
}

/** @brief Ukrywa kursor.
 */  
static inline void hideCursor(){
    printf("\x1B[?25l");
}

/** @brief Ukazuje kursor.
 */
static inline void showCursor(){
    printf("\x1B[?25h"); 
}

/** @brief Podświetla pole.
 */
static inline void highlight(){
    printf("\x1b[30m\x1b[46m");
}

/** @brief Liczy wielkość kroku o jaki mamy przesuać się na planszy.
 * @param[in] players_number - największy numer gracza.
 * @return Długość kroku.
 */ 
static inline uint32_t set_step(uint32_t players_number){
    uint32_t res = 0;
    while(players_number){
        res++;
        players_number /= 10;
    }    
    return res;
}


/** @brief Ustala wspołrzędną początkową.
 * @param[in] x - wysokość lub szerokość planszy.
 * @return pozycja początkowa.
 */
static inline uint32_t set_start_pos(uint32_t x){
    x = (x + 1) / 2;

    if(x == 0)
        x++;
    
    return x;
}

static inline bool check_player(gamma_t *g, uint32_t player){
    int res = 0;
    bool ans = true;

    if(gamma_golden_possible(g, player) == false)
        res++;

    if(gamma_free_fields(g, player) == 0)
        res++;

    if(res == 2)
        ans = false;
    
    return ans;
}

static inline int getche(){
    struct termios oldattr, newattr;
    int ch;
    if(tcgetattr(STDIN_FILENO, &oldattr) != 0){
        clear();
        printf("Program niespodziwanie zakończył proces.\n");
        exit(1);
    }
        

    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO); 
    if(tcsetattr(STDIN_FILENO, TCSANOW, &newattr) != 0){
        clear();
        printf("Program niespodziwanie zakończył proces.\n");
        exit(1);
    }
        

    system("stty -echo"); 
    ch = getchar();
    system("stty echo");


    if(tcsetattr(STDIN_FILENO, TCSANOW, &oldattr) != 0){
        clear();
        printf("Program niespodziwanie zakończył proces.\n");
        exit(1);
    }
        

    return ch;
}

static inline void print_player_info(gamma_t *g, uint32_t player){
    if(player != 0){
        printf("\nRUCH WYKONUJE GRACZ:");
        printf("%" PRIu32 "\n", player);
        printf("ZAJĘTE POLA: ");
        printf("%" PRIu64 "\n", gamma_busy_fields(g, player));
        printf("DOSTĘPNE POLA:");
        printf("%" PRIu64 "\n", gamma_free_fields(g, player));
        bool temp = gamma_golden_possible(g, player);
        if(temp){
            PrintGreen();
            printf("ZŁOTY RUCH DOSTĘPNY");
            reset();
        }
        else{
            printRed();
            printf("ZŁOTY RUCH NIEDOSTĘPNY");
            reset();
        }
            
    }
}

static void print(gamma_t *g, uint32_t x, uint32_t y, uint32_t player, uint32_t step){
    clear();
    char *p = gamma_board(g);
    uint32_t y_console = g->height - y + 1;
    uint32_t x_console = step == 1 ? x : x * step + x - 1;
    uint32_t temp = g->board[y][x] / 10;
    while(temp > 0){
        temp /= 10;
        x_console--;
    }

    printf("%s", p);
    free(p);
    print_player_info(g, player);
    printf("\x1b[%d;%dH", y_console, x_console);
    saveCursorPosition();
    highlight();
    if(g->board[y][x] == 0)
        printf(".");
    else 
        printf("%" PRIu32 "", g->board[y][x]);
    
    
    reset();
    hideCursor();
    fflush(stdout);
    restoreCursorPosition();
    
}

void print_winners(gamma_t *g){
    clear();
    char *p = gamma_board(g);
    printf("%s", p);
    free(p);
    for(uint32_t i = 1; i <= g->players_number; i++){
        printf("GRACZ:");
        printf("%" PRIu32 " ", i);
        printf("ZAJĘTE POLA: ");
        printf("%" PRIu64 "\n", gamma_busy_fields(g, i));
    }
    showCursor();
}

/** @brief Sprawdza czy okno terminalu jest odpowiednio duze
 *  do przeprowadzenia rozgrywki.
 * @param[in] height - wysokość planszy,
 * @param[in] width - szerokość planszy,
 * @param[in] players - największy numer gracza,
 * @param[in] row - wysokość terminalu,
 * @param[in] col - szerokość terminal
 * @return true jeśli okno jest wystarczająco duze,
 * false w przeciwnym przypadku.
 */
static bool check_terminal_size(int height, int width, int players, int row, int col){
    int res = true;
    if(height + 5 > row)
        res = false;
    
    int step = set_step(players);
    int length = width * (step == 1 ? 1 : step + 1);

    if(length > col)
        res = false;

    return res;
}
void interactive(gamma_t *g){
    uint32_t x = set_start_pos(g->width), y = set_start_pos(g->height);
    uint32_t eliminated = 0;
    uint32_t step = set_step(g->players_number);
    int player_is_moving, k, playing = 1;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if(check_terminal_size(g->height, g->width, g->players_number, w.ws_row, w.ws_col) == false){
        printf("Okno terminalu jest zbyt małe aby przeprowadzić rozgrywkę.\n");
        exit(1);
    }
    
    while(playing){
        for(uint32_t i = 1; i <= g->players_number; i++){
            if(check_player(g, i)){
                print(g, x, y, i, step);
                player_is_moving = 1;
                while(player_is_moving && playing){
                    k = getche();
                    if(k == '\033'){
                        k = getche();
                        if(k == '['){
                            k = getche();
                            if(k == 'A' && y < g->height){
                                y++;
                                print(g, x, y, i, step);
                            }else if(k == 'B' && y > 1){
                                y--;
                                print(g, x, y, i, step);
                            }else if(k == 'C' && x < g->width){
                                x++;
                                print(g, x, y, i, step);
                            }else if(k == 'D' && x > 1){
                                x--;
                                print(g, x, y, i, step);
                            }
                        }
                    }else if(k == ' '){
                        bool res = gamma_move(g, i, x - 1, y - 1);
                        if(res == 1){
                            player_is_moving = 0;
                        }

                    }else if(k == 'g' || k == 'G'){
                        bool res = gamma_golden_move(g, i, x - 1, y - 1);
                        if(res == 1){
                            player_is_moving = 0;
                        }

                    }else if(k == 'c' || k == 'C')
                        player_is_moving = 0;

                    else if(k == 4){
                        player_is_moving = 0;
                        playing = 0;
                    }
                }
            }else
                eliminated++;
        }
        if(eliminated == g->players_number)
            playing = 0;

        eliminated = 0;
    }
    print_winners(g);
    return;
}