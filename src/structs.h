/** @file
 * Interfejs klasy przechowujcej struktury wykorzystywane w grze gamma
 * @author Hubert Grochowski hg417878
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


enum dir {up = 0, right = 1, down = 2, left = 3};
enum steps {previous = -1, current = 0, next = 1};

/** @brief Struktura przechowująca infromacje o graczu.
 */
typedef struct player{
    bool golden_done; ///< bool czy gracz wykonał złoty ruch 
    bool possible_mv_del; ///< pomocnicza zmienna do funkcji @ref set_possible_moves from util.h
    uint64_t taken_fields; ///< liczba zajętych pol przez gracza
    uint64_t possible_moves; ///< liczba dozwolonych ruchow w przypadku gdy nie moze powstac nowy obszar
    uint32_t taken_areas; ///< liczba zajętych obszarow przez gracza 
    uint32_t divided_fields; ///< liczba rozdzielanych pol wykorzystywana w @ref set_areas_for_golden_move from util.h
    
} player_t;

/** @brief Struktura przechowująca narzędzia do funkcji pomocniczych.
 */
typedef struct util{
    bool **visited; ///< wskaźnik na tablice odwiedzonych w dfs
    uint64_t *parent; ///< wskaźnik na reprezentanta danego pola

} util_t;

/** @brief Struktura przechowująca stan gry.
 */
typedef struct gamma{

    uint32_t width; ///< Szerokość planszy.
    uint32_t height; ///< Wysokość planszy.
    uint32_t areas; ///< Liczba obszarow dozwolonych przez jednego gracza.
    uint32_t players_number; ///< Liczba graczy.
    uint32_t **board; ///< Wskaźnik na tablice opisującą stan posiadania pol.
    uint64_t taken_fields; ///< Liczba wszystkich zajętych pol.
    player_t *players; ///< wskaźnik na strukturę opisującą graczy.
    util_t *tools; ///< wskaźnik na strukturę z narzędziami do funkcji pomocniczych

} gamma_t;

#endif