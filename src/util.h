/** @file
 * Interfejs klasy przechowujcej fukncje pomocnicze do gry gamma
 * @author Hubert Grochowski hg417878
 */

#ifndef UTIL_H
#define UTIL_H

#include "find_union.h"

/** @brief Sprawdza czy wykonując ruch nie wyjdziemy poza planszę.
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @param[in] move_x – ruch na @p x,
 * @param[in] move_y - ruch na @p y,
 * @return wartość @p true, jeśli jest to mozliwe, a @p false gdy nie.
 */
bool check_borders_with_move(gamma_t *g, uint32_t x, int move_x, uint32_t y, int move_y);

/** @brief Sprawdza czy pole nie znajduje się poza planszą.  
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @return wartość @p true, jeśli jest to mozliwe, a @p false gdy nie.
 */
bool check_borders(gamma_t *g, uint32_t x, uint32_t y);

/** @brief Sprawdza czy dane agrumenty spełniają warunki zadania.
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @return wartość @p true, jeśli jest to mozliwe, a @p false gdy nie.
 */
bool check_conditions(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Sprawdza czy ruch jest dozwolony, po czym zwraca wartość reprezentanta danego pola.
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch
 * @param[in] x - szerokość pola,
 * @param[in] move_x – ruch na @p x,
 * @param[in] y  – wysokość pola,
 * @param[in] move_y - ruch na @p y,
 * @return wartość reprezentanta danego pola lub 0 gdy warunki nie są spełnione.
 */
uint64_t get_adjoined(gamma_t *g, uint32_t player, uint32_t x, int move_x, uint32_t y, int move_y);

/** @brief Przeszukuję planszę po polach danego gracza.
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @param[in] ignore_x - szerokość ignorowanego pola,
 * @param[in] ignore_y - wysokość ignorowanego pola.
 */
void dfs(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, uint32_t ignore_x, uint32_t ignore_y);

/** @brief Czyści tablice odwiedzonych w dfs
 * @param[in, out] g - wskaźnik na strukturę przechowującą stan gry.
 */
void clear_vis (gamma_t *g);

/** @brief Sprawdza czy jest to mozliwe po czym ustawia obszary dla gracza ktory wykonał ruch.
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @return wartość @p true, jeśli jest to mozliwe, a @p false gdy nie.
 */
bool set_areas(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Przeszukuję planszę po polach danego gracza i ustawia nowych reprezentantow.
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @param[in] new_parent - wartość nowego reprezentanta,
 * @param[in] ignore_x - szerokość ignorowanego pola,
 * @param[in] ignore_y - wysokość ignorowanego pola.
 */
void dfs_set_new_parents(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, uint32_t new_parent, uint32_t ignore_x, uint32_t ignore_y);

/** @brief Ustawia nowych reprezentantow jesli gracz wykonał @ref gamma_golden_move
 * dla graczy ktorych pola sąsiadują z danym polem i ustawia nową liczbę zajmowanych obszarow. 
 * @param[in, out] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola.
 */
void set_new_parents(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief stwierdza czy nowo powstale obszary w skutek @ref gamma_golden_move będą zgodne z zasadami 
 * oraz ewentualnie ustawia obszary oraz @ref player_t possible_moves 
 * @param[in, out] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @return @p true, jeśli jest to moliwe lub @p false jeśli nie. 
 */
bool set_areas_for_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Sprawdza czy pole jest juz w zasiegu danego gracza 
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y – wysokość pola,
 * @return 1 jeśli jest nie jest zajmowane, 0 w innym przypadku.
 */
int is_already_possible(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, int dir);

/** @brief Liczy nową liczbę osiągalnych pol dla danego gracza  
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola,
 * @return zwraca liczbę nowo osiągalnych pol.
 */
int get_possible_moves(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Ustawia nową liczbę osiągalnych pol dla danego gracza  
 * @param[in] g - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player - gracz wykonujący ruch,
 * @param[in] x - szerokość pola,
 * @param[in] y  – wysokość pola.
 */
void set_possible_moves(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);


#endif