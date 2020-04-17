/** @file
 * Interfejs klasy przechowujcej narzedzia do find and union
 * @author Hubert Grochowski hg417878
 */

#ifndef FIND_UNION_H
#define FIND_UNION_H

#include "structs.h"

/** @brief Zwraca numer danego pola.
 * @param[in] g – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] height  – wysokość planszy, liczba dodatnia,
 * @param[in] width - szerokość planszy, liczba dodatnia,
 * @return wartość pola o danej wysokości i szerokości.
 */
uint64_t get_number(gamma_t *g, uint32_t height, uint32_t width);

/** @brief Zwraca reprezantanta danego pola
 * @param[in] g – wskaźnik na strukture przechowujaca stan gry
 * @param[in] val – wartosc opisujaca pole na planszy
 * @return wartosc reprezentujaca dane pole 
 */
extern uint64_t find(gamma_t *g, uint64_t val);

/** @brief Laczy dwa pola do jednej grupy
 * @param[in] g – wskaźnik na strukture przechowujaca stan gry
 * @param[in] x_a - pozycja opisujaca szerokosc na planszy pola a
 * @param[in] y_a - pozycja opisujaca wysokosc na planszy pola a
 * @param[in] x_b - pozycja opisujaca szerokosc na planszy pola b
 * @param[in] y_b - pozycja opisujaca wysokosc na planszy pola b
 */
extern void Union(gamma_t *g, uint32_t x_a, uint32_t y_a, uint32_t x_b, uint32_t y_b);

#endif