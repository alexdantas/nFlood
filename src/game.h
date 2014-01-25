/* nFlood - Flood-it puzzle game on the terminal.
 * Copyright (C) 2011-2014  Alexandre Dantas <eu@alexdantas.net>
 * Copyright (C) 2014 Yu-Jie Lin <livibetter@gmail.com>
 *
 * nFlood is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://nflood.alexdantas.net
 */

#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <stdbool.h>
#include "color.h"
#include "board.h"

/** Initializes the `board` with random colors for
 *  all cells, cleaning the `flooded` state.
 */
void game_init(struct game_board_t *board);

/** Tells if the board is completely flooded.
 */
bool game_is_over(struct game_board_t *board);

/** Returns a random integer between the limits
 *  passed as parameters.
 */
int random_int_between(int upper, int lower);

#endif /* GAME_H_DEFINED */

