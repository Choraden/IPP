/** @file
 * Interfejs klasy interactive mode 
 * @author Hubert Grochowski hg417878
 */

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "input_util.h"
#include "gamma.h"
#include "handle.h"
#include <termios.h>
#include <unistd.h>

/** @brief Funkcja wczytuje akcje uzytkownika oraz 
 * na nie reaguje.
 */
void interactive(gamma_t *g);

#endif
