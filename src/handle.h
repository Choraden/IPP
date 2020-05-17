/** @file
 * Interfejs klasy do interpretowania wprowadzonych poleceń 
 * @author Hubert Grochowski hg417878
 */

#ifndef HANDLE_H
#define HANDLE_H

#include <stdint.h>
#include <ctype.h>

/** @brief Funkcja przyjmuje wprowadzone polecenia po czym 
 * sprawdza poprawność i zwraca odpowiedni kod.
 * @param[in] cnt - liczba poleceń.
 * @param[in] com[] - tablica wskaźnikow na polecenia.
 * @param[in] size[] - tablica wielkosci poszczegolnych poleceń.
 * @return 0 - error,
 *         1 - argumenty odpowiadają funkcji gamma board,
 *         2 - argumenty odpowiadają funkcji gamma_golden_possible,
 *         3 - argumenty odpowiadają funkcji gamma_free_fields,
 *         4 - argumenty odpowiadają funkcji gamma_busy_fields,
 *         5 - argumenty odpowiadają funkcji gamma_move,
 *         6 - argumenty odpowiadają funkcji gamma_golden_move,
 *         7 - argumenty odpowiadają poleceniu trybu wsadowego,
 *         8 - argumenty odpowiadają poleceniu trybu interaktywnego.
 */
int handler(int cnt, int *com[] ,int size[]);

/** @brief Sprawdza czy dana tablica składa się z cyfr.
 * @param[in] order  - wskaźnik na kandydatow na cyfry,
 * @param[in] size - liczba elementow do sprawdzenia,
 * @return 1 jeśli ciąg znakow składa się z cyfr,
 * 0 w przeciwnym przypadku.
*/
int is_number(int *order, int size);

/** @brief Sprawdza czy dane ciągi znakow składają się z cyfr.
 * @param[in] com[] - tablica wskaźnikow na ciągi znakow,
 * @param[in] size[] - tablica z długością poszczegolnych ciągow,
 * @param[in] k - liczba ciągow,
 * @return 1 jeśli ciągi znakow składają się z cyfr,
 * 0 w przeciwnym przypadku.
*/
int are_numbers(int *com[], int size[], int k);

/** @brief Zamienia tablice cyfr na liczbę.
 * @param[in] digits - wskaźnik na ciąg cyfr,
 * @param[in] size - liczba cyfr w ciągu,
 * @return Obliczoną liczbę, jeśli mieści się w zakresie uint32_t,
 * w przeciwynym przypadku 0.
 */
uint32_t calculate_number(int *digits, int size);

#endif