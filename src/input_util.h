/** @file
 * Interfejs klasy przechowującej narzędzia pomocnicze do wczytywania danych
 * @author Hubert Grochowski hg417878
 */

#ifndef INPUT_UTIL_H
#define INPUT_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/** @brief Alokuje pamięć potrzebną do wczytywania poleceń.
 * @param[in, out] com[] – tablica wskaźnikow na wczytywane polecenia.
 * @param[in, out] size[] – tablica rozmiaru zaalokowanej pamięci dla
 * poszczegolnych komorek @p com[], początkowy rozmiar 32. 
 */
void Malloc(int *com[], int size[]);

/** @brief Alokuje dwa razy więcej pamięci dla wybranego wskaźnika na daną 
 * komorkę w tablicy do przechowywania poleceń.
 * @param[in, out] tab - wskaźnik na komorkę tablicy @p com[] z funkcji Malloc.
 * @param[in, out] size - wskaźnik na rozmiar danej komorki. 
 * @param[in] poz - pozycja od ktorej nalezy wyzerować komorki.  
 */
void Realloc(int **tab, int *size, int poz);

/** @brief Zeruje wartości danej tablicy.
 * @param[in, out] tab.
 * @param[in] size - rozmiar danej tablicy @p tab.
*/
void Clear(int *tab, int size);

/** @brief Zeruje wszystkie wskazywane wartosci w tablicy.
 * @param[in, out] com[] - tablica wskaźnikow do wyzerowania.
 * @param[in] size[] - tablica rozmiarow wskaznikow z tablicy @p com[].
 */
void ClearALL(int *com[], int size[]);

/** @brief Zwalnia pamięć wszystkich wskaznikow w tablicy.
 * @param[in, out] com[] - tablica wskaźnikow.
 */
void FREE(int *com[]);

#endif